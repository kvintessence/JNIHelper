/*!
   \file NonVoidType.hpp
   \brief A dirty hack to workaround 'void' return type.
   \author Denis Sorokin
   \date January 24 2016
   \copyright Zeptolab, 2016
 */

#ifndef JH_NON_VOID_TYPE_HPP
#define JH_NON_VOID_TYPE_HPP

namespace jh
{
    /**
    * Some dummy type that should substitude the void type when returning values.
    */
    struct NotVoid { };

    /**
    * Utility template class to handle the return types of JNI methods. Mainly
    * used to change 'void' return type to some other type, for example, 'NotVoid'.
    * By default, type 'T' is treated as itself (type 'T').
    */
    template<class T>
    struct NonVoidReturnType
    {
        using Type = T;
    };

    /**
    * Utility template class to handle the return types of JNI methods. Mainly
    * used to change 'void' return type to some other type, for example, 'NotVoid'.
    */
    template<>
    struct NonVoidReturnType<void>
    {
        using Type = NotVoid;
    };
}

#endif
