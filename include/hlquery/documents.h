/*
 * hlquery C++ Client - Documents API
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
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

   private:

     std::shared_ptr<Request> request_;
};

}
