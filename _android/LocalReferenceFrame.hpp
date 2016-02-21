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
        LocalReferenceFrame(int frameSize = 16)
        : m_frameSize(frameSize)
        , m_framesCount(0)
        {
            push();
        }

        ~LocalReferenceFrame()
        {
            while (pop());
        }

        bool push()
        {
            JNIEnv *env = getCurrentJNIEnvironment();

            if (env->PushLocalFrame(m_frameSize) == 0) {
                ++m_framesCount;
                return true;
            }

            return false;
        }

        bool pop(jobject* jobjectToKeep = nullptr)
        {
            if (m_framesCount) {
                JNIEnv *env = getCurrentJNIEnvironment();

                if (jobjectToKeep) {
                    *jobjectToKeep = env->PopLocalFrame(*jobjectToKeep);
                } else {
                    env->PopLocalFrame(nullptr);
                }

                --m_framesCount;

                return true;
            }

            return false;
        }

    private:
        int m_frameSize;
        int m_framesCount;

        LocalReferenceFrame(const LocalReferenceFrame &) = delete;
        void operator=(const LocalReferenceFrame &) = delete;
    };
}

#endif