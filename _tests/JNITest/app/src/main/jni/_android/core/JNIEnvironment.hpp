/*!
   \file JNIEnvironment.hpp
   \brief Utilities to get and use JNIEnv* pointer.
   \author Denis Sorokin
   \date January 24 2016
   \copyright Zeptolab, 2016
 */

#ifndef JH_JAVA_ENVIRONMENT_HPP
#define JH_JAVA_ENVIRONMENT_HPP

#include <jni.h>

namespace jh
{
    /**
    * Returns the pointer to the Java Virtual Machine (JVM).
    *
    * @return The pointer to JVM.
    * @warning Right now, it depends on 'jnienv.*' zframework code.
    * @warning Can be nullptr if called before JNI initialization.
    */
    JavaVM* getJavaVM();

    /**
    * Returns the JNI environment pointer for the current thread.
    *
    * @return JNIEnv pointer to the current JNI environment.
    * @warning Can be nullptr if called in non-java or unattached thread.
    */
    JNIEnv* getCurrentJNIEnvironment();

    /**
    * Utility class that ensures that JNI environment pointer exists while the object
    * of this class is alive. Right now, it attaches the current thread to the JVM if
    * it is not attached already (and detaches if it is needed after destruction).
    *
    * @warning Right now, user can't use non-default Java objects in non-Java thread.
    *
    * Possible usage example:
    *
    * @code{.cpp}
    *
    * // lets assume that we have entered some scope and we are in a non-java thread
    * {
    *
    * // attaching this thread to JVM
    * JavaEnvironmentGuarantee javaContext();
    *
    * // Doing something with Java code; for example, creating jstring.
    * ...
    *
    * // leaving the scope; this thread will be detached from JVM
    * }
    *
    * @endcode
    */
    class JNIEnvironmentGuarantee
    {
    public:
        JNIEnvironmentGuarantee();
        ~JNIEnvironmentGuarantee();

    private:
        bool m_threadShouldBeDetached;
    };
}

#endif
