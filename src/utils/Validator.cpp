/*
 * hlquery C++ Client - Input Validation Implementation
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 */

#include <algorithm>
#include <regex>

#include "hlquery/utils/Validator.h"

namespace hlquery
{
namespace utils
{

void validateCollectionName(const std::string& name)
{
     if (name.empty())
     {
          throw ValidationException("Collection name must be a non-empty string");
     }

     /* Check name length (matches server validation: 1-64 characters) */

     if (name.length() > 64)
     {
          throw ValidationException("Collection name must be between 1 and 64 characters");
     }

     /* Collection names should be URL-safe */

     std::regex invalid_chars(R"([^a-zA-Z0-9_-])");
     if (std::regex_search(name, invalid_chars))
     {
          throw ValidationException(
               "Collection name contains invalid characters. "
               "Use only letters, numbers, underscores, and hyphens");
     }

     /* Check if name starts with letter or underscore (matches server validation) */

     if (!std::isalpha(name[0]) && name[0] != '_')
     {
          throw ValidationException("Collection name must start with a letter or underscore");
     }
}

void validateDocumentId(const std::string& doc_id)
{
     if (doc_id.empty())
     {
          throw ValidationException("Document ID must be a non-empty string");
     }

     /* Check name length (matches server validation: 1-64 characters) */

     if (doc_id.length() > 64)
     {
          throw ValidationException("Document ID must be between 1 and 64 characters");
     }

     /* Document IDs should be URL-safe: alphanumeric, underscores, and hyphens only */

     std::regex invalid_chars(R"([^a-zA-Z0-9_-])");
     if (std::regex_search(doc_id, invalid_chars))
     {
          throw ValidationException(
               "Document ID contains invalid characters. "
               "Use only letters, numbers, underscores, and hyphens");
     }
}

void validatePagination(int offset, int limit)
{
     if (offset < 0)
     {
          throw ValidationException("Offset must be a non-negative integer");
     }

     if (limit < 1)
     {
          throw ValidationException("Limit must be a positive integer");
     }

     if (limit > 1000)
     {
          throw ValidationException("Limit cannot exceed 1000");
     }
}

void validateSearchParams(const std::map<std::string, std::string>& params)
{
     if (params.find("limit") != params.end())
     {
          try
          {
               int limit = std::stoi(params.at("limit"));
               if (limit < 1)
               {
                    throw ValidationException("Limit must be a positive integer");
               }
          }
          catch (const std::exception&)
          {
               throw ValidationException("Limit must be a positive integer");
          }
     }

     if (params.find("offset") != params.end())
     {
          try
          {
               int offset = std::stoi(params.at("offset"));
               if (offset < 0)
               {
                    throw ValidationException("Offset must be a non-negative integer");
               }
          }
          catch (const std::exception&)
          {
               throw ValidationException("Offset must be a non-negative integer");
          }
     }

     if (params.find("page") != params.end())
     {
          try
          {
               int page = std::stoi(params.at("page"));
               if (page < 1)
               {
                    throw ValidationException("Page must be a positive integer");
               }
          }
          catch (const std::exception&)
          {
               throw ValidationException("Page must be a positive integer");
          }
     }
}

}
}
