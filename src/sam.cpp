/*
 * hlquery C++ Client - SAM API Implementation
 * https://www.hlquery.com
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 * You are free to redistribute and/or modify this software
 * under the terms of the BSD License.
 * For more details, please visit: https://docs.hlquery.com
 */

#include <stdexcept>

#include "hlquery/sam.h"
#include "hlquery/utils/Validator.h"

namespace hlquery
{

Sam::Sam(std::shared_ptr<Request> request) : request_(request)
{
}

Response Sam::search(const std::string& collection_name, const std::string& query,
                     const std::map<std::string, std::string>& params)
{
     utils::validateCollectionName(collection_name);

     if (query.empty())
     {
          throw std::invalid_argument("SAM query must be a non-empty string");
     }

     std::map<std::string, std::string> query_params = params;
     query_params["collection"] = collection_name;
     query_params["q"] = query;

     return request_->execute("GET", "/sam/search", nullptr, query_params);
}

Response Sam::status(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     std::map<std::string, std::string> query_params = params;

     if (!collection_name.empty())
     {
          utils::validateCollectionName(collection_name);
          query_params["collection"] = collection_name;
     }

     return request_->execute("GET", "/sam/status", nullptr, query_params);
}

Response Sam::history(const std::string& collection_name, int limit,
                      const std::map<std::string, std::string>& params)
{
     if (limit < 1)
     {
          throw std::invalid_argument("SAM history limit must be a positive integer");
     }

     std::map<std::string, std::string> query_params = params;
     query_params["limit"] = std::to_string(limit);

     if (!collection_name.empty())
     {
          utils::validateCollectionName(collection_name);
          query_params["collection"] = collection_name;
     }

     return request_->execute("GET", "/sam/history", nullptr, query_params);
}

}
