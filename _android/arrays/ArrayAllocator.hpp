/*!
   \file JavaArrayAllocator.hpp
   \brief
   \author Denis Sorokin
   \date 22.02.2016
 */

#ifndef JH_JAVA_ARRAY_ALLOCATOR_HPP
#define JH_JAVA_ARRAY_ALLOCATOR_HPP

#include <jni.h>
#include "../core/ErrorHandler.hpp"
#include "../core/JNIEnvironment.hpp"

namespace jh
{
    /**
    * Implementations for creating java arrays.
    */
    template<class JavaArrayType, class ElementType>
    struct JavaArrayAllocator;

    template<class ElementType>
    struct JavaArrayAllocator<jbooleanArray, ElementType>
    {
        static jbooleanArray create(JNIEnv* env, jsize size)
        {
            return env->NewBooleanArray(size);
        }
    };

    template<class ElementType>
    struct JavaArrayAllocator<jintArray, ElementType>
    {
        static jintArray create(JNIEnv* env, jsize size)
        {
            return env->NewIntArray(size);
        }
    };

    template<class ElementType>
    struct JavaArrayAllocator<jlongArray, ElementType>
    {
        static jlongArray create(JNIEnv* env, jsize size)
        {
            return env->NewLongArray(size);
        }
    };

    template<class ElementType>
    struct JavaArrayAllocator<jfloatArray, ElementType>
    {
        static jfloatArray create(JNIEnv* env, jsize size)
        {
            return env->NewFloatArray(size);
        }
    };

    template<class ElementType>
    struct JavaArrayAllocator<jdoubleArray, ElementType>
    {
        static jdoubleArray create(JNIEnv* env, jsize size)
        {
            return env->NewDoubleArray(size);
        }
    };

    template<class ElementType>
    struct JavaArrayAllocator<jobjectArray, ElementType>
    {
        static jobjectArray create(JNIEnv* env, jsize size)
        {
            std::string className = ToJavaType<ElementType>::className();

            jclass javaClass = env->FindClass(className.c_str());
            if (javaClass == nullptr) {
                reportInternalError("class not found [" + className + "]");
                return nullptr;
            }

            return env->NewObjectArray(size, javaClass, nullptr);
        }
    };
}

#endif
