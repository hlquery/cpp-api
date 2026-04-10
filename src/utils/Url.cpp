/*
 * hlquery C++ Client - URL Utilities Implementation
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 */

#include <cctype>
#include <iomanip>
#include <sstream>

#include "hlquery/utils/Url.h"

namespace hlquery
{
namespace utils
{

std::string urlEncode(const std::string& str)
{
     std::ostringstream escaped;
     escaped.fill('0');
     escaped << std::hex;

     for (char c : str)
     {
          if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
          {
               escaped << c;
          }
          else
          {
               escaped << '%' << std::setw(2) << static_cast<int>(static_cast<unsigned char>(c));
          }
     }

     return escaped.str();
}

}
}
