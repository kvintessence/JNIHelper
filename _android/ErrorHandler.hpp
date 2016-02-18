/*!
   \file ErrorHandler.hpp
   \brief Describes how internal errors should be reported.
   \author Denis Sorokin
   \date January 24 2016
   \copyright Zeptolab, 2016
 */

#ifndef JH_ERROR_HANDLER_HPP
#define JH_ERROR_HANDLER_HPP

#include <string>
#include "zframework/debug.h"

namespace jh
{
    /**
    * Reports some error that happened during JNI-related actions to user.
    * This method is intended to be used only by this library.
    *
    * @param errorMessage Message string describing the error.
    */
    inline void reportInternalError(std::string errorMessage)
    {
        _LOGE("ZFJavaHelper", "JavaHelper internal error: '%s'.", errorMessage.c_str());
        // TODO : check java exceptions through JNI
    }
}

#endif
