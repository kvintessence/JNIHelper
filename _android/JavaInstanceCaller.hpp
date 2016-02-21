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
#include "NonVoidType.hpp"
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
    struct InstanceCaller
    {
        static void call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)
        {
            // this should fail to compile anyway because this method doesn't return anything
            reportInternalError("can't find exact java caller");
        }
    };

    template<class ... ArgumentTypes>
    struct InstanceCaller<void, ArgumentTypes...>
    {
        static typename NonVoidReturnType<void>::Type call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)
        {
            env->CallVoidMethod(instance, javaMethod, arguments...);
            return typename NonVoidReturnType<void>::Type();
        }
    };

    template<class ... ArgumentTypes>
    struct InstanceCaller<bool, ArgumentTypes...>
    {
        static bool call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)
        {
            return env->CallBooleanMethod(instance, javaMethod, arguments...);
        }
    };

    template<class ... ArgumentTypes>
    struct InstanceCaller<int, ArgumentTypes...>
    {
        static int call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)
        {
            return env->CallIntMethod(instance, javaMethod, arguments...);
        }
    };

    template<class ... ArgumentTypes>
    struct InstanceCaller<long, ArgumentTypes...>
    {
        static long call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)
        {
            return env->CallLongMethod(instance, javaMethod, arguments...);
        }
    };

    template<class ... ArgumentTypes>
    struct InstanceCaller<float, ArgumentTypes...>
    {
        static float call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)
        {
            return env->CallFloatMethod(instance, javaMethod, arguments...);
        }
    };

    template<class ... ArgumentTypes>
    struct InstanceCaller<double, ArgumentTypes...>
    {
        static double call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)
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
    typename NonVoidReturnType<RealReturnType>::Type callMethod(jobject instance, std::string methodName, typename NonVoidReturnType<ArgumentTypes>::Type ... arguments)
    {
        using FakeReturnType = typename NonVoidReturnType<RealReturnType>::Type;

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

        FakeReturnType result = InstanceCaller<RealReturnType, typename NonVoidReturnType<ArgumentTypes>::Type ...>::call(env, instance, javaMethod, arguments...);

        env->DeleteLocalRef(javaClass);

        return result;
    }
}

#endif
