/*!
   \file JavaMethodSignature.hpp
   \brief A utility to deduce java method signatures.
   \author Denis Sorokin
   \date January 24 2016
   \copyright Zeptolab, 2016
 */

#ifndef JH_JAVA_METHOD_SIGNATURE_HPP
#define JH_JAVA_METHOD_SIGNATURE_HPP

#include <string>

namespace jh
{
    /**
    * Internal Java method signature deduction. Each struct represents different
    * argument type of Java method.
    */
    namespace // internal method signature deduction
    {
        template<class FirstArgumentType, class ... OtherArgumentTypes>
        struct Signature
        {
            static std::string string()
            {
                return Signature<FirstArgumentType>::string() + Signature<OtherArgumentTypes...>::string();
            }
        };

        template<>
        struct Signature<void>
        {
            static std::string string()
            {
                return "V";
            }
        };

        template<>
        struct Signature<bool>
        {
            static std::string string()
            {
                return "Z";
            }
        };

        template<>
        struct Signature<int>
        {
            static std::string string()
            {
                return "I";
            }
        };

        template<>
        struct Signature<long>
        {
            static std::string string()
            {
                return "J";
            }
        };

        template<>
        struct Signature<float>
        {
            static std::string string()
            {
                return "F";
            }
        };

        template<>
        struct Signature<double>
        {
            static std::string string()
            {
                return "D";
            }
        };

        template<>
        struct Signature<jobject>
        {
            static std::string string()
            {
                return "Ljava/lang/Object;";
            }
        };

        template<>
        struct Signature<jstring>
        {
            static std::string string()
            {
                return "Ljava/lang/String;";
            }
        };

        template<>
        struct Signature<jobjectArray>
        {
            static std::string string()
            {
                return std::string("[") + Signature<jobject>::string();
            }
        };

        template<>
        struct Signature<jbooleanArray>
        {
            static std::string string()
            {
                return std::string("[") + Signature<bool>::string();
            }
        };

        template<>
        struct Signature<jintArray>
        {
            static std::string string()
            {
                return std::string("[") + Signature<int>::string();
            }
        };

        template<>
        struct Signature<jlongArray>
        {
            static std::string string()
            {
                return std::string("[") + Signature<long>::string();
            }
        };

        template<>
        struct Signature<jfloatArray>
        {
            static std::string string()
            {
                return std::string("[") + Signature<float>::string();
            }
        };

        template<>
        struct Signature<jdoubleArray>
        {
            static std::string string()
            {
                return std::string("[") + Signature<double>::string();
            }
        };
    }

    /**
    * Method that returns full java method signature based on the return type
    * and argument types passed via template arguments. This function is used
    * for Java methods with more than one argument.
    *
    * @return Java method signature as a string.
    */
    template<class ReturnType, class FirstType, class ... ArgumentTypes>
    std::string getJavaMethodSignature()
    {
        return std::string("(")
             + Signature<FirstType, ArgumentTypes...>::string()
             + std::string(")")
             + Signature<ReturnType>::string();
    }

    /**
    * Method that returns full java method signature based on the return type
    * and argument types passed via template arguments. This function is used
    * for Java methods without arguments.
    *
    * @return Java method signature as a string.
    */
    template<class ReturnType>
    std::string getJavaMethodSignature()
    {
        return std::string("()")
             + Signature<ReturnType>::string();
    }
}

#endif
