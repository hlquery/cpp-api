/*
 * hlquery C++ Client - Configuration Utilities Implementation
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 */

#include <cctype>
#include <regex>
#include <sstream>

#include "hlquery/utils/Config.h"

namespace hlquery
{
namespace utils
{

namespace
{
bool parseBoolOption(const std::string& value)
{
     std::string lowered;
     lowered.reserve(value.size());
     for (char ch : value)
     {
          lowered.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
     }
     return !(lowered == "0" || lowered == "false" || lowered == "no" || lowered == "off");
}
}

ConfigOptions mergeDefaults(const std::map<std::string, std::string>& user_options)
{
     ConfigOptions defaults;

     for (const auto& option : user_options)
     {
          if (option.first == "timeout")
          {
               defaults.timeout = std::stoi(option.second);
          }
          else if (option.first == "base_url")
          {
               defaults.base_url = option.second;
          }
          else if (option.first == "auth_method")
          {
               defaults.auth_method = option.second;
          }
          else if (option.first == "token")
          {
               defaults.token = option.second;
          }
          else if (option.first == "tls_verify")
          {
               defaults.tls_verify = parseBoolOption(option.second);
          }
     }

     return defaults;
}

bool isValidUrl(const std::string& url)
{
     if (url.empty())
          return false;

     /* Simple URL validation */

     std::regex url_pattern(R"(^(https?://)([^\s/$.?#].[^\s]*)$)");
     return std::regex_match(url, url_pattern);
}

std::string normalizeUrl(const std::string& url)
{
     std::string normalized = url;
     if (!normalized.empty() && normalized.back() == '/')
     {
          normalized.pop_back();
     }
     return normalized;
}

}
}
