/*
 * hlquery C++ Client - Configuration Utilities
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 */

#pragma once

#include <map>
#include <string>

namespace hlquery
{
namespace utils
{

struct ConfigOptions
{
     int timeout = 30;
     std::string base_url = "http://localhost:9200";
     std::string auth_method = "bearer";
     std::string token = "";
     bool tls_verify = true;
};

ConfigOptions mergeDefaults(const std::map<std::string, std::string>& user_options);
bool isValidUrl(const std::string& url);
std::string normalizeUrl(const std::string& url);

}
}
