/*!
   \file JavaNativeMethod.hpp
   \brief Allocation and registration of java native methods in C++
   \author Denis Sorokin
   \date 15.02.2016
 */

#ifndef JH_JAVA_NATIVE_METHOD_HPP
#define JH_JAVA_NATIVE_METHOD_HPP

#include <jni.h>
#include "../core/ErrorHandler.hpp"
#include "../core/JavaMethodSignature.hpp"

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
    bool registerJavaNativeMethods(std::string javaClassName, int methodCount, JNINativeMethod* methodDescriptions);

    template<class ReturnType, class ... ArgumentTypes>
    bool registerStaticNativeMethod(std::string javaClassName, std::string methodName, ReturnType (*methodPointer)(ArgumentTypes...))
    {
        std::string signature = getJavaMethodSignature<ReturnType, ArgumentTypes...>();

        JNINativeMethod method[1] = {
            methodName.c_str(),
            signature.c_str(),
            (void*)methodPointer
        };

        return registerJavaNativeMethods(javaClassName, 1, method);
    }

    template<class JavaClassType, class ReturnType, class ... ArgumentTypes>
    bool registerStaticNativeMethod(std::string methodName, ReturnType (*methodPointer)(ArgumentTypes...))
    {
        return registerStaticNativeMethod<ReturnType, ArgumentTypes ...>(JavaClassType::className(), methodName, methodPointer);
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
    template<int id, class CppClass, class ReturnType, class ... Arguments>
    class JavaNativeMethod
    {
        using MethodImplementationPointer = ReturnType(CppClass::*)(Arguments...);

        template <class, class>
        friend class JavaObjectWrapper;

    private:
        static MethodImplementationPointer s_callback;

        static void setCallback(MethodImplementationPointer callback)
        {
            if (s_callback) {
                reportInternalError("redefining callback for java class [" + CppClass::JavaClass::className() + "]");
            }

            s_callback = callback;
        }

        static ReturnType rawNativeMethod(JNIEnv*, jobject javaObject, Arguments ... args)
        {
            return CppClass::template callCppObjectMethod<ReturnType>(javaObject, [=] (CppClass* wrapperInstance) -> ReturnType {
                return (wrapperInstance->*s_callback)(args...);
            });
        }
    };

    template<int id, class CppClass, class ReturnType, class ... Arguments>
    typename JavaNativeMethod<id, CppClass, ReturnType, Arguments...>::MethodImplementationPointer JavaNativeMethod<id, CppClass, ReturnType, Arguments...>::s_callback(nullptr);
}

#endif
