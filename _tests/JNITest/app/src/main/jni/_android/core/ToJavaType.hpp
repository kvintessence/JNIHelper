/*!
   \file ToJavaType.hpp
   \brief
   \author Denis Sorokin
   \date 21.02.2016
 */

#ifndef JH_TO_JAVA_TYPE_HPP
#define JH_TO_JAVA_TYPE_HPP

#include <string>
#include <jni.h>

namespace jh
{
    template<class T>
    struct ToJavaType
    {
        using Type = jobject;
        using CallReturnType = jobject;

        static std::string signature()
        {
            return T::signature();
        }

        static std::string className()
        {
            return T::className();
        }
    };

    template<>
    struct ToJavaType<void>
    {
        using Type = void;
        using CallReturnType = void;

        static std::string signature()
        {
            return "V";
        }
    };

    template<>
    struct ToJavaType<bool>
    {
        using Type = jboolean;
        using CallReturnType = jboolean;

        static std::string signature()
        {
            return "Z";
        }
    };

    template<>
    struct ToJavaType<int>
    {
        using Type = jint;
        using CallReturnType = jint;

        static std::string signature()
        {
            return "I";
        }
    };

    template<>
    struct ToJavaType<long>
    {
        using Type = jlong;
        using CallReturnType = jlong;

        static std::string signature()
        {
            return "J";
        }
    };

    template<>
    struct ToJavaType<float>
    {
        using Type = jfloat;
        using CallReturnType = jfloat;

        static std::string signature()
        {
            return "F";
        }
    };

    template<>
    struct ToJavaType<double>
    {
        using Type = jdouble;
        using CallReturnType = jdouble;

        static std::string signature()
        {
            return "D";
        }
    };

    /**
    * Different jobject-like pointers classes
    */

    template<class JavaType>
    struct JPointerLike
    {
        using Type = JavaType;
        using CallReturnType = jobject;
    };

    template<>
    struct ToJavaType<jobject> : public JPointerLike<jobject>
    {
        static std::string className()
        {
            return "java/lang/Object";
        }

        static std::string signature()
        {
            return "L" + className() + ";";
        }
    };

    template<>
    struct ToJavaType<jstring> : public JPointerLike<jstring>
    {
        static std::string className()
        {
            return "java/lang/String";
        }

        static std::string signature()
        {
            return "L" + className() + ";";
        }
    };

    /**
    * Arrays
    */

    template<class JavaElementType>
    struct JavaArray
    {
        using ElementType = JavaElementType;

        static std::string signature()
        {
            return "[" + ToJavaType<JavaElementType>::signature();
        }
    };

    template<>
    template<class JavaType>
    struct ToJavaType<JavaArray<JavaType>> : public JavaArray<JavaType>
    {
        using Type = jobjectArray;
        using CallReturnType = jobjectArray;
    };

    template<>
    struct ToJavaType<jbooleanArray> : public JavaArray<jboolean>, public JPointerLike<jbooleanArray>
    { };

    template<>
    struct ToJavaType<jintArray> : public JavaArray<jint>, public JPointerLike<jintArray>
    { };

    template<>
    struct ToJavaType<jlongArray> : public JavaArray<jlong>, public JPointerLike<jlongArray>
    { };

    template<>
    struct ToJavaType<jfloatArray> : public JavaArray<jfloat>, public JPointerLike<jfloatArray>
    { };

    template<>
    struct ToJavaType<jdoubleArray> : public JavaArray<jdouble>, public JPointerLike<jdoubleArray>
    { };

    template<>
    struct ToJavaType<jobjectArray> : public JavaArray<jobject>, public JPointerLike<jobjectArray>
    { };
}

#endif
