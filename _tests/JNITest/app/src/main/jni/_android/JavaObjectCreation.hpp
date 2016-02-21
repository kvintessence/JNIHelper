/*!
   \file JavaObjectCreation.hpp
   \brief A utility header to create new java objects from c++ code.
   \author Denis Sorokin
   \date January 24 2016
   \copyright Zeptolab, 2016
 */

#ifndef JH_JAVA_OBJECT_CREATION_HPP
#define JH_JAVA_OBJECT_CREATION_HPP

#include <jni.h>
#include <string>
#include "ToJavaType.hpp"
#include "ErrorHandler.hpp"
#include "JavaEnvironment.hpp"
#include "JavaMethodSignature.hpp"

namespace jh
{
    /**
    * Allows to create Java objects via their constructors. Constructor argument
    * types should be explicitly specified via template arguments.
    *
    * @param className Java class name as a string.
    * @param arguments List of arguments for the constructor.
    * @return Create Java object pointer (aka jobject).
    *
    * Possible usage example:
    *
    * @code{.cpp}
    *
    * // Lets declare custom Java class
    * JH_JAVA_CUSTOM_CLASS(Example, "com/class/path/Example");
    *
    * // Create an object via default constructor:
    * jobject first = jh::createNewObject<>(ExampleClass::name());
    *
    * // Create an object via constructor with two arguments:
    * jobject second = jh::createNewObject<Example, int>(ExampleClass::name(), first, 100);
    *
    * @endcode
    */
    template<class ... ArgumentTypes>
    jobject createNewObject(std::string className, typename ToJavaType<ArgumentTypes>::Type ... arguments)
    {
        JNIEnv* env = getCurrentJNIEnvironment();

        std::string methodSignature = getJavaMethodSignature<void, ArgumentTypes...>();

        jclass javaClass = env->FindClass(className.c_str());
        if (javaClass == nullptr) {
            reportInternalError("class not found [" + className + "]");
            return nullptr;
        }

        jmethodID javaConstructor = env->GetMethodID(javaClass, "<init>", methodSignature.c_str());
        if (javaConstructor == nullptr) {
            reportInternalError("constructor for class [" + className + "] not found, tried signature [" + methodSignature + "]");
            return nullptr;
        }

        jobject result = env->NewObject(javaClass, javaConstructor, arguments...);

        env->DeleteLocalRef(javaClass);

        return result;
    }

    template<class NewObjectType, class ... ArgumentTypes>
    jobject createNewObject(typename ToJavaType<ArgumentTypes>::Type ... arguments)
    {
        return createNewObject<ArgumentTypes...>(NewObjectType::className(), arguments...);
    }
}

#endif
