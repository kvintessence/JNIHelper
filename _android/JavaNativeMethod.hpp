/*!
   \file JavaNativeMethod.hpp
   \brief Allocation and registration of java native methods in c++
   \author Denis Sorokin
   \date February 15 2016
   \copyright Zeptolab, 2016
 */

#ifndef JH_JAVA_NATIVE_METHOD_HPP
#define JH_JAVA_NATIVE_METHOD_HPP

#include <jni.h>
#include "ErrorHandler.hpp"
#include "JavaMethodSignature.hpp"

namespace jh
{
    /**
    * Registers local functions as java native methods.
    *
    * @param javaClassName Name of the java class.
    * @param methodCount Total number of methods that should be registered by this call.
    * @param methodDescriptions Pointer to an array of java native method descriptions.
    *
    * @return True if methods were successfully registered or false otherwise.
    *
    * @warning Java native methods should be registered only after JNI initialization, i.e. dont use this method while doing some static-level stuff.
    */
    bool registerJavaNativeMethods(std::string javaClassName, int methodCount, JNINativeMethod* methodDescriptions)
    {
        auto env = getCurrentJNIEnvironment();

        jclass javaClass = env->FindClass(javaClassName.c_str());
        if (javaClass == nullptr) {
            reportInternalError("unable to find class [" + javaClassName + "] for native methods registration");
            return false;
        }

        if (env->RegisterNatives(javaClass, methodDescriptions, methodCount) < 0) {
            reportInternalError("unable to register native methods for class [" + javaClassName + "]");
            return false;
        }

        return true;
    }

    /**
    * This class provides an static method that should be registered as java native method.
    * It should not be used by the programmer itself, but by the JavaObjectWrapper class.
    *
    * @param id Dirty hack to distinguish two native methods with equal signatures. Should be unique for every native method per wrapper class.
    * @param WrapperClass C++ class that wraps the functionality of some Java class.
    * @param ReturnType The return type of java native method.
    * @param Arguments The types of arguments of java native method.
    *
    * @warning One should not forget to set the callback by method 'setCallback(...)' if using this class.
    * @warning Each combination of template parameters should correspond to ONLY ONE native method.
    * @warning Native methods that were implemented by this class should NOT be called inside the java object constructor.
    */
    template<int id, class WrapperClass, class ReturnType, class ... Arguments>
    class JavaNativeMethod
    {
        using MethodImplementationPointer = ReturnType(WrapperClass::*)(Arguments...);

        template <class, class>
        friend class JavaObjectWrapper;

    private:
        static MethodImplementationPointer s_callback;

        static void setCallback(MethodImplementationPointer callback)
        {
            if (s_callback) {
                reportInternalError("redefining callback for java class [" + WrapperClass::JavaClass::name() + "]");
            }

            s_callback = callback;
        }

        static ReturnType rawNativeMethod(JNIEnv*, jobject javaObject, Arguments ... args)
        {
            return WrapperClass::template callCppObjectMethod<ReturnType>(javaObject, [=] (WrapperClass* wrapperInstance) -> ReturnType {
                return (wrapperInstance->*s_callback)(args...);
            });
        }
    };

    template<int id, class WrapperClass, class ReturnType, class ... Arguments>
    typename JavaNativeMethod<id, WrapperClass, ReturnType, Arguments...>::MethodImplementationPointer JavaNativeMethod<id, WrapperClass, ReturnType, Arguments...>::s_callback(nullptr);
}

#endif
