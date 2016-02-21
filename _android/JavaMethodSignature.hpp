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
#include "ToJavaType.hpp"

namespace jh
{
    /**
    * Internal Java method signature deduction. Each struct represents different
    * argument type of Java method.
    */
    template<class FirstArgumentType, class ... OtherArgumentTypes>
    struct Signature
    {
        static std::string string()
        {
            return ToJavaType<FirstArgumentType>::signature() + Signature<OtherArgumentTypes...>::string();
        }
    };

    template<class ArgumentType>
    struct Signature<ArgumentType>
    {
        static std::string string()
        {
            return ToJavaType<ArgumentType>::signature();
        }
    };

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
