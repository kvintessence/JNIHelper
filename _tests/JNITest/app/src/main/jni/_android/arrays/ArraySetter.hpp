/*!
   \file JavaArraySetter.hpp
   \brief
   \author Denis Sorokin
   \date 22.02.2016
 */

#ifndef JH_JAVA_ARRAY_SETTER_HPP
#define JH_JAVA_ARRAY_SETTER_HPP

#include <jni.h>
#include "../core/JNIEnvironment.hpp"

namespace jh
{
    /**
    * Implementations for setting values into java arrays.
    */
    template<class JavaArrayType>
    struct JavaArraySetter;

    template<>
    struct JavaArraySetter<jbooleanArray>
    {
        static void set(JNIEnv* env, jbooleanArray array, jsize size, jboolean* elements)
        {
            env->SetBooleanArrayRegion(array, 0, size, elements);
        }
    };

    template<>
    struct JavaArraySetter<jintArray>
    {
        static void set(JNIEnv* env, jintArray array, jsize size, jint* elements)
        {
            env->SetIntArrayRegion(array, 0, size, elements);
        }
    };

    template<>
    struct JavaArraySetter<jlongArray>
    {
        static void set(JNIEnv* env, jlongArray array, jsize size, jlong* elements)
        {
            env->SetLongArrayRegion(array, 0, size, elements);
        }
    };

    template<>
    struct JavaArraySetter<jfloatArray>
    {
        static void set(JNIEnv* env, jfloatArray array, jsize size, jfloat* elements)
        {
            env->SetFloatArrayRegion(array, 0, size, elements);
        }
    };

    template<>
    struct JavaArraySetter<jdoubleArray>
    {
        static void set(JNIEnv* env, jdoubleArray array, jsize size, jdouble* elements)
        {
            env->SetDoubleArrayRegion(array, 0, size, elements);
        }
    };

    template<>
    struct JavaArraySetter<jobjectArray>
    {
        static void set(JNIEnv* env, jobjectArray array, jsize size, jobject* elements)
        {
            for (int i = 0; i < size; ++i) {
                env->SetObjectArrayElement(array, i, elements[i]);
            }
        }
    };
}

#endif
