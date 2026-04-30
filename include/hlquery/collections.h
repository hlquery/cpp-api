/*
 * hlquery C++ Client - Collections API
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

#include <memory>
#include <map>
#include <string>
#include <vector>

#include "request.h"
#include "response.h"
#include "searchresult.h"

namespace hlquery
{

class Collections
{
   public:

     explicit Collections(std::shared_ptr<Request> request);

     Response list(int offset = 0, int limit = 10);
     Response get(const std::string& name);
     Response create(const std::string& name, const nlohmann::json& schema);
     Response remove(const std::string& name);
     Response update(const std::string& name, const nlohmann::json& schema);
     Response getFields(const std::string& name);
     Response search(const std::string& name, const std::map<std::string, std::string>& params = {});
     SearchResult searchStructured(const std::string& name, const std::map<std::string, std::string>& params = {});
     Response sql(const std::string& name, const std::string& sql,
                  const std::map<std::string, std::string>& params = {});
     Response vectorSearch(const std::string& name, const std::map<std::string, std::string>& params = {});
     SearchResult vectorSearchStructured(const std::string& name, const std::map<std::string, std::string>& params = {});

   private:

     std::shared_ptr<Request> request_;

     std::string buildQueryString(const std::map<std::string, std::string>& params) const;
};

}
