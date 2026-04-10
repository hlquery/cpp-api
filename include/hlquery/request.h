/*
 * hlquery C++ Client - HTTP Request Handler
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 */

#pragma once

#include <map>
#include <memory>
#include <string>

#include "exceptions.h"
#include "response.h"

namespace hlquery
{

class Request
{
   public:

     Request(const std::string& base_url, int timeout = 30,
             const std::string& auth_token = "", const std::string& auth_method = "bearer",
             bool tls_verify = true);

     void setAuthToken(const std::string& token, const std::string& method = "bearer");
     void clearAuth();

     Response execute(const std::string& method, const std::string& path,
                      const nlohmann::json& body = nullptr,
                      const std::map<std::string, std::string>& query_params = {});

   private:

     std::string base_url;
     int timeout_;
     std::string auth_token;
     std::string auth_method_;
     bool tls_verify_;

     std::string buildUrl(const std::string& path, const std::map<std::string, std::string>& query_params) const;
     Response makeHttpRequest(const std::string& method, const std::string& url, const std::string& body = "") const;
     void parseUrl(const std::string& url, std::string& host, int& port, bool& use_ssl) const;
};

}
