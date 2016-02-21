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

        static std::string signature()
        {
            return "V";
        }
    };

    template<>
    struct ToJavaType<bool>
    {
        using Type = jboolean;

        static std::string signature()
        {
            return "Z";
        }
    };

    template<>
    struct ToJavaType<int>
    {
        using Type = jint;

        static std::string signature()
        {
            return "I";
        }
    };

    template<>
    struct ToJavaType<long>
    {
        using Type = jlong;

        static std::string signature()
        {
            return "J";
        }
    };

    template<>
    struct ToJavaType<float>
    {
        using Type = jfloat;

        static std::string signature()
        {
            return "F";
        }
    };

    template<>
    struct ToJavaType<double>
    {
        using Type = jdouble;

        static std::string signature()
        {
            return "D";
        }
    };

    template<>
    struct ToJavaType<jobject>
    {
        using Type = jobject;

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
    struct ToJavaType<jstring>
    {
        using Type = jstring;

        static std::string className()
        {
            return "java/lang/String";
        }

        static std::string signature()
        {
            return "L" + className() + ";";
        }
    };
}

#endif
