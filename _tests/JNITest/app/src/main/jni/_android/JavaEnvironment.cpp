/*!
   \file JavaEnvironment.cpp
   \brief Utilities to get and use JNIEnv* pointer.
   \author Denis Sorokin
   \date January 24 2016
   \copyright Zeptolab, 2016
 */

#include <jni.h>
#include "../zframework/core/_android/jnienv.h"
#include "JavaEnvironment.hpp"
#include "ErrorHandler.hpp"

namespace jh
{
    JavaVM* getJavaVM()
    {
        return JNI::getVM();
    }

    JNIEnv* getCurrentJNIEnvironment()
    {
        JNIEnv* env = nullptr;
        JavaVM* javaVM = getJavaVM();

        javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

        if (env == nullptr) {
            reportInternalError("jni environment not found");
        }

        return env;
    }

    JavaEnvironmentGuarantee::JavaEnvironmentGuarantee()
    : m_threadShouldBeDetached(false)
    {
        JNIEnv* env = nullptr;
        JavaVM* javaVM = getJavaVM();

        int getEnvStatus = javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

        if (getEnvStatus == JNI_EDETACHED) {
            if (javaVM->AttachCurrentThread(&env, nullptr) != 0) {
                reportInternalError("couldn't attach current thread to java VM");
            } else {
                m_threadShouldBeDetached = true;
                // no classes besides the system ones!
                // TODO : do something with this
            }
        }

        if (env == nullptr) {
            reportInternalError("couldn't get jni environment for current thread");
        }
    }

    JavaEnvironmentGuarantee::~JavaEnvironmentGuarantee()
    {
        if (m_threadShouldBeDetached) {
            getJavaVM()->DetachCurrentThread();
        }
    }
}
