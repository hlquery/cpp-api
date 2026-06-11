/*
 * hlquery C++ Client - Search API Implementation
 * https://www.hlquery.com
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 * You are free to redistribute and/or modify this software
 * under the terms of the BSD License.
 * For more details, please visit: https://docs.hlquery.com
 */

#include <algorithm>
#include <sstream>
#include <stdexcept>

#include "hlquery/search.h"
#include "hlquery/utils/Url.h"
#include "hlquery/utils/Validator.h"

namespace hlquery
{

Search::Search(std::shared_ptr<Request> request, std::shared_ptr<Collections> collections)
    : request_(request), collections_(collections)
{
}

std::string Search::buildQueryString(const std::map<std::string, std::string>& params) const
{
     std::ostringstream query;
     bool first = true;

     for (const auto& param : params)
     {
          if (!first)
               query << "&";
          query << utils::urlEncode(param.first) << "=" << utils::urlEncode(param.second);
          first = false;
     }

     return query.str();
}

Response Search::search(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     utils::validateCollectionName(collection_name);
     std::map<std::string, std::string> query_params = params;

     /* Keep parity with other client libraries that expose "like" as the text query alias. */

     if (query_params.find("q") == query_params.end())
     {
          const auto like_it = query_params.find("like");
          if (like_it != query_params.end())
          {
               query_params["q"] = like_it->second;
               query_params.erase(like_it);
          }
     }

     utils::validateSearchParams(query_params);

     /* Auto-detect searchable fields if query_by not specified */

     if (query_params.find("q") != query_params.end() &&
         query_params.find("query_by") == query_params.end() &&
         !query_params["q"].empty())
     {
          Response collection = collections_->get(collection_name);
          if (collection.getStatusCode() == 200)
          {
               nlohmann::json body = collection.getBody();
               if (body.contains("searchable_fields") && body["searchable_fields"].is_array() &&
                   !body["searchable_fields"].empty())
               {
                    std::ostringstream fields;
                    bool first = true;
                    for (const auto& field : body["searchable_fields"])
                    {
                         if (!first)
                              fields << ",";
                         fields << field.get<std::string>();
                         first = false;
                    }
                    query_params["query_by"] = fields.str();
               }
          }
     }

     std::string path = "/collections/" + utils::urlEncode(collection_name) + "/documents/search";
     std::string query_string = buildQueryString(query_params);

     if (!query_string.empty())
     {
          path += "?" + query_string;
     }

     return request_->execute("GET", path);
}

Response Search::sql(const std::string& collection_name, const std::string& sql,
                     const std::map<std::string, std::string>& params)
{
     utils::validateCollectionName(collection_name);

     if (sql.empty())
     {
          throw std::invalid_argument("SQL query must be a non-empty string");
     }

     std::map<std::string, std::string> query_params = params;
     query_params["sql"] = sql;

     std::string path = "/collections/" + utils::urlEncode(collection_name) + "/documents/search";
     std::string query_string = buildQueryString(query_params);

     if (!query_string.empty())
     {
          path += "?" + query_string;
     }

     return request_->execute("GET", path);
}

Response Search::multiSearch(const std::vector<nlohmann::json>& searches)
{
     nlohmann::json payload;
     payload["searches"] = searches;

     return request_->execute("POST", "/multi_search", payload);
}

Response Search::vectorSearch(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     utils::validateCollectionName(collection_name);

     std::map<std::string, std::string> query_params = params;

     std::string path = "/collections/" + utils::urlEncode(collection_name) + "/vector_search";
     std::string query_string = buildQueryString(query_params);

     if (!query_string.empty())
     {
          path += "?" + query_string;
     }

     return request_->execute("GET", path);
}

SearchResult Search::searchStructured(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     return SearchResult(search(collection_name, params));
}

MultiSearchResult Search::multiSearchStructured(const std::vector<nlohmann::json>& searches)
{
     return MultiSearchResult(multiSearch(searches));
}

SearchResult Search::vectorSearchStructured(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     return SearchResult(vectorSearch(collection_name, params));
}

}
