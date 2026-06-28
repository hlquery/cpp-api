/*
 * hlquery C++ Client - Documents API
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

#include "request.h"
#include "response.h"

namespace hlquery
{

class Documents
{
   public:

     explicit Documents(std::shared_ptr<Request> request);

     Response list(const std::string& collection_name, const std::map<std::string, std::string>& params = {});
     Response get(const std::string& collection_name, const std::string& document_id);
     Response add(const std::string& collection_name, const nlohmann::json& document);
     Response update(const std::string& collection_name, const std::string& document_id, const nlohmann::json& document);
     Response remove(const std::string& collection_name, const std::string& document_id);
     Response importDocuments(const std::string& collection_name, const std::vector<nlohmann::json>& documents);
     Response deleteByFilter(const std::string& collection_name, const std::string& filter);
     Response updateByQuery(const std::string& collection_name, const nlohmann::json& body);
     Response deleteByQuery(const std::string& collection_name, const nlohmann::json& body);
     Response facetCounts(const std::string& collection_name, const std::map<std::string, std::string>& params = {});
     Response exportDocuments(const std::string& collection_name, const std::map<std::string, std::string>& params = {});
     Response maybe(const std::string& collection_name, const std::map<std::string, std::string>& params = {});
     Response context(const std::string& collection_name, const std::string& document_id,
                      const std::map<std::string, std::string>& params = {});

   private:

     std::shared_ptr<Request> request_;
};

}
