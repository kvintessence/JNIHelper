/*!
   \file LocalReferenceFrame.hpp
   \brief
   \author Denis Sorokin
   \date 21.02.2016
 */

#ifndef JH_LOCAL_REFERENCE_FRAME_HPP
#define JH_LOCAL_REFERENCE_FRAME_HPP

#include <jni.h>
#include "JavaEnvironment.hpp"

namespace jh
{
    class LocalReferenceFrame
    {
    public:
        LocalReferenceFrame(int frameSize = 8)
        : m_frameWasCreated(false)
        {
            JNIEnv *env = getCurrentJNIEnvironment();

            if (env->PushLocalFrame(frameSize) == 0) {
                m_frameWasCreated = true;
            }
        }

        ~LocalReferenceFrame()
        {
            if (m_frameWasCreated) {
                JNIEnv *env = getCurrentJNIEnvironment();
                env->PopLocalFrame(nullptr);
            }
        }

    private:
        bool m_frameWasCreated;

        LocalReferenceFrame(const LocalReferenceFrame &) = delete;
        void operator=(const LocalReferenceFrame &) = delete;
    };
}

#endif