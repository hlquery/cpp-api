/*
 * hlquery C++ Client - URL Utilities Implementation
 * https://www.hlquery.com
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 * You are free to redistribute and/or modify this software
 * under the terms of the BSD License.
 * For more details, please visit: https://docs.hlquery.com
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
