/*
 * hlquery C++ Client - Main Client Class
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

#include "collections.h"
#include "documents.h"
#include "request.h"
#include "response.h"
#include "search.h"
#include "utils/Config.h"

namespace hlquery
{

class Client
{
   public:

     Client(const std::string& base_url = "", const std::map<std::string, std::string>& options = {});

     void setAuthToken(const std::string& token, const std::string& method = "bearer");
     void clearAuth();

     /* System APIs */

     Response health();
     Response stats();
     Response etc();
     Response info();
     Response flush();

     /* Cluster links */

     Response links();
     Response linksPing();
     Response linksConnect(const std::string& endpoint_or_host, int port = -1);
     Response linksDisconnect(const std::string& endpoint_or_host, int port = -1);

     /* Collections API */

     std::shared_ptr<Collections> collections();
     Response listCollections(int offset = 0, int limit = 10);
     Response listCollectionsDistributed();
     Response getCollection(const std::string& name);
     Response getCollectionFields(const std::string& name);

     /* Documents API */

     std::shared_ptr<Documents> documents();
     Response listDocuments(const std::string& collection_name, const std::map<std::string, std::string>& params = {});
     Response getDocument(const std::string& collection_name, const std::string& document_id);

     /* Search API */

     std::shared_ptr<Search> searchApi();
     Response search(const std::string& collection_name, const std::map<std::string, std::string>& params = {});
     Response vectorSearch(const std::string& collection_name, const std::map<std::string, std::string>& params = {});

     /* Execute arbitrary request */

     Response executeRequest(const std::string& method, const std::string& path,
                             const nlohmann::json& body = nullptr,
                             const std::map<std::string, std::string>& query_params = {});

   private:

     std::shared_ptr<Request> request_;
     std::shared_ptr<Collections> collections_;
     std::shared_ptr<Documents> documents_;
     std::shared_ptr<Search> search_;
};

}
