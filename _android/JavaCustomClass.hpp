/*!
   \file JavaCustomClass.hpp
   \brief Allow user to declare custom java classes and deduce method signatures for them.
   \author Denis Sorokin
   \date January 24 2016
   \copyright Zeptolab, 2016
 */

#ifndef JH_JAVA_CUSTOM_CLASS_HPP
#define JH_JAVA_CUSTOM_CLASS_HPP

#include <jni.h>
#include <string>
#include "NonVoidType.hpp"
#include "JavaStaticCaller.hpp"
#include "JavaInstanceCaller.hpp"
#include "JavaMethodSignature.hpp"

/**
* This macro magic tells JavaHelper library about the existance of some android class.
* This allows to perform java method signature deduction and some other stuff to happen.
* After declaring this macro programmer can use the declared class in the JNI calls.
*
* This macro basically declares two things: <JavaClass> type and <JavaClass>Class class.
* in the global namespace and several template specializations in the 'jh' namespace.
*
* All methods that should return the instance of custom Java class would return
* the pointer to Java object (aka jobject). The programmer should carefully track
* the types of Java objects pointers by himself.
*
* The most common use of this macro is:
*
* @code{.cpp}
*
* // Declare custom Java class (named Example):
* JH_JAVA_CUSTOM_CLASS(Example, "com/class/path/Example");
*
* // Get the Java custom class name:
* std::string className = ExampleClass::name();
*
* // Call some static method of this class:
* int result = jh::callStaticMethod<int>(ExampleClass::name(), "methodName");
*
* // Explicitly create the object of this class:
* jobject exampleObject = jh::createNewObject<>(ExampleClass::name());
*
* // Call some method that returns the object of this class:
* jobject exampleObject2 = jh::callStaticMethod<Example>(ExampleClass::name(), "methodName");
*
* // Pass the object of this class as a parameter:
* jh::callMethod<void, Example>(someOtherObject, "methodName", exampleObject);
*
* @endcode
*/
#define JH_JAVA_CUSTOM_CLASS(CLASS_NAME_TOKEN, CLASS_PATH_STRING)               \
class CLASS_NAME_TOKEN##Class : public _jobject                                 \
{                                                                               \
public:                                                                         \
    static std::string name()                                                   \
    {                                                                           \
        return CLASS_PATH_STRING;                                               \
    }                                                                           \
};                                                                              \
typedef CLASS_NAME_TOKEN##Class* CLASS_NAME_TOKEN;                              \
namespace jh                                                                    \
{                                                                               \
    template<>                                                                  \
    struct NonVoidReturnType<CLASS_NAME_TOKEN>                                  \
    {                                                                           \
        using Type = jobject;                                                   \
    };                                                                          \
    template<>                                                                  \
    struct Signature<CLASS_NAME_TOKEN>                                          \
    {                                                                           \
        static std::string string()                                             \
        {                                                                       \
            return "L" CLASS_PATH_STRING ";";                                   \
        }                                                                       \
    };                                                                          \
    template<class ... ArgumentTypes>                                                                           \
    struct StaticCaller<CLASS_NAME_TOKEN, ArgumentTypes...>                                                     \
    {                                                                                                           \
        static jobject call(JNIEnv* env, jclass javaClass, jmethodID javaMethod, ArgumentTypes ... arguments)   \
        {                                                                                                       \
            return env->CallStaticObjectMethod(javaClass, javaMethod, arguments...);                            \
        }                                                                                                       \
    };                                                                                                          \
    template<class ... ArgumentTypes>                                                                           \
    struct InstanceCaller<CLASS_NAME_TOKEN, ArgumentTypes...>                                                   \
    {                                                                                                           \
        static jobject call(JNIEnv* env, jobject instance, jmethodID javaMethod, ArgumentTypes ... arguments)   \
        {                                                                                                       \
            return env->CallObjectMethod(instance, javaMethod, arguments...);                                   \
        }                                                                                                       \
    };                                                                                                          \
}                                                                                                               \

#endif
