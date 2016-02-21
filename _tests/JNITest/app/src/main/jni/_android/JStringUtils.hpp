/*!
   \file JStringUtils.hpp
   \brief Utility functions to use jstring objects.
   \author Denis Sorokin
   \date January 24 2016
   \copyright Zeptolab, 2016
 */

#ifndef JH_JSTRING_UTILS_HPP
#define JH_JSTRING_UTILS_HPP

#include <jni.h>
#include <string>

namespace jh
{
    jstring createJString(const char*);

    jstring createJString(const std::string);

    std::string jstringToStdString(const jstring);
}

#endif
