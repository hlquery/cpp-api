/*
 * hlquery C++ Client - Exceptions
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 */

#pragma once

#include <stdexcept>
#include <string>

namespace hlquery
{

class HlqueryException : public std::runtime_error
{
   public:

     explicit HlqueryException(const std::string& message) : std::runtime_error(message)
     {
     }
};

class AuthenticationException : public HlqueryException
{
   public:

     explicit AuthenticationException(const std::string& message) : HlqueryException(message)
     {
     }
};

class RequestException : public HlqueryException
{
   public:

     RequestException(const std::string& message, int status_code = 0, const std::string& response_body = "")
         : HlqueryException(message), status_code_(status_code), response_body_(response_body)
     {
     }

     int getStatusCode() const
     {
          return status_code_;
     }
     std::string getResponseBody() const
     {
          return response_body_;
     }

   private:

     int status_code_;
     std::string response_body_;
};

class ValidationException : public HlqueryException
{
   public:

     explicit ValidationException(const std::string& message) : HlqueryException(message)
     {
     }
};

class CollectionException : public HlqueryException
{
   public:

     explicit CollectionException(const std::string& message) : HlqueryException(message)
     {
     }
};

class DocumentException : public HlqueryException
{
   public:

     explicit DocumentException(const std::string& message) : HlqueryException(message)
     {
     }
};

class SearchException : public HlqueryException
{
   public:

     explicit SearchException(const std::string& message) : HlqueryException(message)
     {
     }
};

}
