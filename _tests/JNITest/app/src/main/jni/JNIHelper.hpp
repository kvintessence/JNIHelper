/*!
    \file JavaHelper.hpp
    \brief A small utility library to hide all the horrors of JNI API.
    \author Denis Sorokin
    \date 24.01.2016

    Changelog:

    Version 1.0.1:
    * JavaObjectWrapper class

    Version 1.0.0:
    * Basic functionality to call java methods from C++

    Cheat sheet:

    @code{.cpp}

    // Declaring some custom Java class:
    JH_JAVA_CUSTOM_CLASS(Example, "com/class/path/Example");

    // Declaring smart Java object pointer:
    jh::JavaObjectPointer obj;

    // Creating new Java object with/without arguments:
    obj = jh::createNewObject<>(ExampleClass::name());
    obj = jh::createNewObject<bool, long>(ExampleClass::name(), true, 10L);

    // Calling static method with/without arguments:
    jh::callStaticMethod<void>(ExampleClass::name(), "voidMethodName");
    int sum = jh::callStaticMethod<int, int, int>(ExampleClass::name(), "sumMethod", 4, 5);

    // Calling common methods with/without arguments:
    jh::callMethod<void>(obj, "voidMethodName");
    int sum = jh::callMethod<int, int, int>(obj, "sumMethod", 4, 5);

    // Calling methods that returns some custom Java object:
    jobject newObject = jh::callMethod<Example, double>(obj, "factoryMethodName", 3.1415);

    @endcode

    @code{.java}

    package com.some.path;

    public class Example
    {
        ...
        protected native void someVoidMethod();
        protected native int sumTwo(int x, int y);
        ...
    }

    @endcode

    @code{.cpp}

    JH_JAVA_CUSTOM_CLASS(JavaExample, "com/some/path/Example");

    // Now declare our wrapper:
    class ExampleWrapper : public jh::JavaObjectWrapper<JavaExampleClass, ExampleWrapper>
    {
        ExampleWrapper(...) : jh::JavaObjectWrapper<JavaExampleClass, ExampleWrapper>(this) { ... }

        void linkJavaNativeMethods() override
        {
            registerNativeMethod<1, void>("someVoidMethod", &ExampleWrapper::interstitialPressed);
            registerNativeMethod<2, int, int, int>("sumTwo", &ExampleWrapper::interstitialClosed);
        }

        jobject initializeJavaObject() override
        {
            return jh::createNewObject<>(ExampleClass::name());
        }

        void someVoidMethod() { log("Example", "Look, I'm logging something!"); }
        int sumTwo(int x, int y) { return x + y; }
    }

    @endcode
 */

#ifndef JH_JAVA_HELPER_HPP
#define JH_JAVA_HELPER_HPP

#include "_android/arrays/ArrayBuilder.hpp"

#include "_android/calls/StaticCaller.hpp"
#include "_android/calls/InstanceCaller.hpp"
#include "_android/calls/ObjectCreation.hpp"

#include "_android/core/JavaCustomClass.hpp"
#include "_android/core/JavaMethodSignature.hpp"
#include "_android/core/JNIEnvironment.hpp"

#include "_android/native/JavaObjectWrapper.hpp"
#include "_android/native/JavaNativeMethod.hpp"

#include "_android/utils/LocalReferenceFrame.hpp"
#include "_android/utils/JavaObjectPointer.hpp"
#include "_android/utils/JStringUtils.hpp"

#endif
