/*
 * hlquery C++ Client - Search API
 * https://www.hlquery.com
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 * You are free to redistribute and/or modify this software
 * under the terms of the BSD License.
 * For more details, please visit: https://docs.hlquery.com
 */

#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "collections.h"
#include "request.h"
#include "response.h"
#include "searchresult.h"

namespace hlquery
{

class Search
{
   public:

     Search(std::shared_ptr<Request> request, std::shared_ptr<Collections> collections);

     Response search(const std::string& collection_name, const std::map<std::string, std::string>& params = {});
     Response searchPost(const std::string& collection_name, const nlohmann::json& body);
     Response sql(const std::string& collection_name, const std::string& sql,
                  const std::map<std::string, std::string>& params = {});
     Response multiSearch(const std::vector<nlohmann::json>& searches);
     Response multiSearch(const std::map<std::string, std::string>& params);
     Response globalSearch(const std::map<std::string, std::string>& params = {});
     Response globalSearchPost(const nlohmann::json& body);
     Response vectorSearch(const std::string& collection_name, const std::map<std::string, std::string>& params = {});
     Response vectorSearchPost(const std::string& collection_name, const nlohmann::json& body);

     SearchResult searchStructured(const std::string& collection_name, const std::map<std::string, std::string>& params = {});
     MultiSearchResult multiSearchStructured(const std::vector<nlohmann::json>& searches);
     SearchResult vectorSearchStructured(const std::string& collection_name, const std::map<std::string, std::string>& params = {});

   private:

     std::shared_ptr<Request> request_;
     std::shared_ptr<Collections> collections_;

     std::string buildQueryString(const std::map<std::string, std::string>& params) const;
};

}
