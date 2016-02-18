/*!
   \file JavaStaticCaller.hpp
   \brief A utility header to call static java methods.
   \author Denis Sorokin
   \date January 24 2016
   \copyright Zeptolab, 2016
 */

#ifndef JH_JAVA_STATIC_CALLER_HPP
#define JH_JAVA_STATIC_CALLER_HPP

#include <jni.h>
#include <string>
#include "NonVoidType.hpp"
#include "ErrorHandler.hpp"
#include "JavaEnvironment.hpp"
#include "JavaCustomClass.hpp"
#include "JavaMethodSignature.hpp"

namespace jh
{
    /**
    * Classes that describe internal implementation for Java static calls.
    * Each class represents different return type of Java method.
    */
    namespace // internal calls to static methods
    {
        template<class ReturnType, class ... ArgumentTypes>
        struct StaticCaller
        {
            static void call(JNIEnv* env, jclass javaClass, jmethodID javaMethod, ArgumentTypes ... arguments)
            {
                // this should fail to compile anyway because this method doesn't return anything
                reportInternalError("can't find exact static java caller");
            }
        };

        template<class ... ArgumentTypes>
        struct StaticCaller<void, ArgumentTypes...>
        {
            static typename NonVoidReturnType<void>::Type call(JNIEnv* env, jclass javaClass, jmethodID javaMethod, ArgumentTypes ... arguments)
            {
                env->CallStaticVoidMethod(javaClass, javaMethod, arguments...);
                return typename NonVoidReturnType<void>::Type();
            }
        };

        template<class ... ArgumentTypes>
        struct StaticCaller<bool, ArgumentTypes...>
        {
            static bool call(JNIEnv* env, jclass javaClass, jmethodID javaMethod, ArgumentTypes ... arguments)
            {
                return env->CallStaticBooleanMethod(javaClass, javaMethod, arguments...);
            }
        };

        template<class ... ArgumentTypes>
        struct StaticCaller<int, ArgumentTypes...>
        {
            static int call(JNIEnv* env, jclass javaClass, jmethodID javaMethod, ArgumentTypes ... arguments)
            {
                return env->CallStaticIntMethod(javaClass, javaMethod, arguments...);
            }
        };

        template<class ... ArgumentTypes>
        struct StaticCaller<long, ArgumentTypes...>
        {
            static long call(JNIEnv* env, jclass javaClass, jmethodID javaMethod, ArgumentTypes ... arguments)
            {
                return env->CallStaticLongMethod(javaClass, javaMethod, arguments...);
            }
        };

        template<class ... ArgumentTypes>
        struct StaticCaller<float, ArgumentTypes...>
        {
            static float call(JNIEnv* env, jclass javaClass, jmethodID javaMethod, ArgumentTypes ... arguments)
            {
                return env->CallStaticFloatMethod(javaClass, javaMethod, arguments...);
            }
        };

        template<class ... ArgumentTypes>
        struct StaticCaller<double, ArgumentTypes...>
        {
            static double call(JNIEnv* env, jclass javaClass, jmethodID javaMethod, ArgumentTypes ... arguments)
            {
                return env->CallStaticDoubleMethod(javaClass, javaMethod, arguments...);
            }
        };

        template<class ... ArgumentTypes>
        struct StaticCaller<jobject, ArgumentTypes...>
        {
            static jobject call(JNIEnv* env, jclass javaClass, jmethodID javaMethod, ArgumentTypes ... arguments)
            {
                return env->CallStaticObjectMethod(javaClass, javaMethod, arguments...);
            }
        };
    }

    /**
    * Calls a static method of some Java class. Programmer should explicitly
    * specify the return type and argument types via template arguments.
    *
    * @param className Java class name as string.
    * @param methodName Method name as string.
    * @param arguments List of arguments to the java method call.
    *
    * Possible usage example:
    *
    * @code{.cpp}
    *
    * // Declaring some custom Java class:
    * JH_JAVA_CUSTOM_CLASS(Example, "com/class/path/Example");
    *
    * // Calling void static method without arguments:
    * jh::callStaticMethod<void>(ExampleClass::name(), "voidMethodName");
    *
    * // Calling int static method with two arguments:
    * int sum = jh::callStaticMethod<int, int, int>(ExampleClass::name(), "sumMethod", 4, 5);
    *
    * // Calling static factory method that returns some custom Java object:
    * jobject newObject = jh::callStaticMethod<Example, double>(ExampleClass::name(), "factoryMethodName", 3.1415);
    *
    * @endcode
    */
    template<class RealReturnType, class ... ArgumentTypes>
    typename NonVoidReturnType<RealReturnType>::Type callStaticMethod(std::string className, std::string methodName, ArgumentTypes ... arguments)
    {
        using FakeReturnType = typename NonVoidReturnType<RealReturnType>::Type;

        JNIEnv* env = getCurrentJNIEnvironment();

        std::string methodSignature = getJavaMethodSignature<RealReturnType, ArgumentTypes...>();

        jclass javaClass = env->FindClass(className.c_str());
        if (javaClass == nullptr) {
            reportInternalError("class not found [" + className + "]");
            return FakeReturnType();
        }

        jmethodID javaMethod = env->GetStaticMethodID(javaClass, methodName.c_str(), methodSignature.c_str());
        if (javaMethod == nullptr) {
            reportInternalError("method [" + methodName + "] for class [" + className + "] not found");
            return FakeReturnType();
        }

        FakeReturnType result = StaticCaller<RealReturnType, ArgumentTypes...>::call(env, javaClass, javaMethod, arguments...);

        env->DeleteLocalRef(javaClass);

        return result;
    }
}

#endif
