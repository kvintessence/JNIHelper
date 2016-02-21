/*!
   \file JavaInstanceCaller.hpp
   \brief A utility to call java methods on a specific objects.
   \author Denis Sorokin
   \date January 24 2016
   \copyright Zeptolab, 2016
 */

#ifndef JH_JAVA_INSTANCE_CALLER_HPP
#define JH_JAVA_INSTANCE_CALLER_HPP

#include <jni.h>
#include <string>
#include "ErrorHandler.hpp"
#include "JavaEnvironment.hpp"
#include "JavaMethodSignature.hpp"

namespace jh
{
    /**
    * Classes that describe internal implementation for Java instance calls.
    * Each class represents different return type of Java method.
    */
    template<class ReturnType, class ... ArgumentTypes>
    struct InstanceCaller;

    template<class ... ArgumentTypes>
    struct InstanceCaller<void, ArgumentTypes...>
    {
        static void call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)
        {
            env->CallVoidMethod(instance, javaMethod, arguments...);
        }
    };

    template<class ... ArgumentTypes>
    struct InstanceCaller<jboolean, ArgumentTypes...>
    {
        static jboolean call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)
        {
            return env->CallBooleanMethod(instance, javaMethod, arguments...);
        }
    };

    template<class ... ArgumentTypes>
    struct InstanceCaller<jint, ArgumentTypes...>
    {
        static jint call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)
        {
            return env->CallIntMethod(instance, javaMethod, arguments...);
        }
    };

    template<class ... ArgumentTypes>
    struct InstanceCaller<jlong, ArgumentTypes...>
    {
        static jlong call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)
        {
            return env->CallLongMethod(instance, javaMethod, arguments...);
        }
    };

    template<class ... ArgumentTypes>
    struct InstanceCaller<jfloat, ArgumentTypes...>
    {
        static jfloat call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)
        {
            return env->CallFloatMethod(instance, javaMethod, arguments...);
        }
    };

    template<class ... ArgumentTypes>
    struct InstanceCaller<jdouble, ArgumentTypes...>
    {
        static jdouble call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)
        {
            return env->CallDoubleMethod(instance, javaMethod, arguments...);
        }
    };

    template<class ... ArgumentTypes>
    struct InstanceCaller<jobject, ArgumentTypes...>
    {
        static jobject call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)
        {
            return env->CallObjectMethod(instance, javaMethod, arguments...);
        }
    };

    template<class ... ArgumentTypes>
    struct InstanceCaller<jstring, ArgumentTypes...>
    {
        static jstring call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)
        {
            return static_cast<jstring>(env->CallObjectMethod(instance, javaMethod, arguments...));
        }
    };

    /**
    * Calls a method on some Java object instance. Programmer should explicitly
    * specify the return type and argument types via template arguments.
    *
    * @param instance Java object (jobject)
    * @param methodName Method name as string.
    * @param arguments List of arguments to the java method call.
    *
    * Possible usage example:
    *
    * @code{.cpp}
    *
    * // Lets assume that we already have some Java object
    * jobject someObject = ...;
    *
    * // Calling void method without arguments:
    * jh::callMethod<void>(someObject, "voidMethodName");
    *
    * // Calling int method with two arguments:
    * int sum = jh::callMethod<int, int, int>(someObject, "sumMethod", 4, 5);
    *
    * // Calling factory method that returns some custom Java object:
    * JH_JAVA_CUSTOM_CLASS(Example, "com/class/path/Example");
    * jobject newObject = jh::callMethod<Example, double>(someObject, "factoryMethodName", 3.1415);
    *
    * @endcode
    */
    template<class RealReturnType, class ... ArgumentTypes>
    typename ToJavaType<RealReturnType>::Type callMethod(jobject instance, std::string methodName, typename ToJavaType<ArgumentTypes>::Type ... arguments)
    {
        using FakeReturnType = typename ToJavaType<RealReturnType>::Type;

        JNIEnv* env = getCurrentJNIEnvironment();

        std::string methodSignature = getJavaMethodSignature<RealReturnType, ArgumentTypes...>();

        jclass javaClass = env->GetObjectClass(instance);
        if (javaClass == nullptr) {
            reportInternalError("class for java object instance not found");
            return FakeReturnType();
        }

        jmethodID javaMethod = env->GetMethodID(javaClass, methodName.c_str(), methodSignature.c_str());
        if (javaMethod == nullptr) {
            reportInternalError("method [" + methodName + "] for java object instance not found, tried signature [" + methodSignature + "]");
            return FakeReturnType();
        }

        return InstanceCaller<FakeReturnType, typename ToJavaType<ArgumentTypes>::Type ...>::call(env, instance, javaMethod, arguments...);
    }
}

#endif
