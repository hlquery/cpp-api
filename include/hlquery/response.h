/*
 * hlquery C++ Client - Response Handler
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 */

#pragma once

#include <json.hpp>
#include <map>
#include <string>

namespace hlquery
{

class Response
{
   public:

     Response(int status_code, const nlohmann::json& body, const std::map<std::string, std::string>& headers = {});
     Response(int status_code, const std::string& body, const std::map<std::string, std::string>& headers = {});

     int getStatusCode() const;
     nlohmann::json getBody() const;
     std::map<std::string, std::string> getHeaders() const;

     bool isSuccess() const;
     bool isError() const;
     std::string getError() const;

     nlohmann::json toJson() const;

   private:

     int status_code_;
     nlohmann::json body_;
     std::map<std::string, std::string> headers_;
};

}
