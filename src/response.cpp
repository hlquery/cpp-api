/*
 * hlquery C++ Client - Response Handler Implementation
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 */

#include "hlquery/response.h"

namespace hlquery
{

Response::Response(int status_code, const nlohmann::json& body, const std::map<std::string, std::string>& headers)
    : status_code_(status_code), body_(body), headers_(headers)
{
}

Response::Response(int status_code, const std::string& body, const std::map<std::string, std::string>& headers)
    : status_code_(status_code), headers_(headers)
{
     try
     {
          body_ = nlohmann::json::parse(body);
     }
     catch (const nlohmann::json::parse_error&)
     {
          body_ = body;
     }
}

int Response::getStatusCode() const
{
     return status_code_;
}

nlohmann::json Response::getBody() const
{
     return body_;
}

std::map<std::string, std::string> Response::getHeaders() const
{
     return headers_;
}

bool Response::isSuccess() const
{
     return status_code_ >= 200 && status_code_ < 300;
}

bool Response::isError() const
{
     return status_code_ >= 400;
}

std::string Response::getError() const
{
     if (isError() && body_.is_object())
     {
          if (body_.contains("error"))
          {
               return body_["error"].get<std::string>();
          }
          if (body_.contains("message"))
          {
               return body_["message"].get<std::string>();
          }
     }
     return "";
}

nlohmann::json Response::toJson() const
{
     nlohmann::json result;
     result["status"] = status_code_;
     result["body"] = body_;
     return result;
}

}
