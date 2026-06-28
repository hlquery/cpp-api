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
#include "resources.h"
#include "response.h"
#include "search.h"
#include "system.h"
#include "utils/Config.h"

namespace hlquery
{

class Client
{
   public:

     Client(const std::string& base_url = "", const std::map<std::string, std::string>& options = {});

     void setAuthToken(const std::string& token, const std::string& method = "bearer");
     void clearAuth();

     std::shared_ptr<System> system();
     std::shared_ptr<Aliases> aliases();
     std::shared_ptr<Overrides> overrides();
     std::shared_ptr<Synonyms> synonyms();
     std::shared_ptr<Stopwords> stopwords();
     std::shared_ptr<Keys> keys();
     std::shared_ptr<Users> users();
     std::shared_ptr<Modules> modules();
     std::shared_ptr<Presets> presets();
     std::shared_ptr<Analytics> analytics();

     /* System APIs */

     Response health();
     Response ready();
     Response status();
     Response query();
     Response startup();
     Response bootStatus();
     Response stats();
     Response metrics();
     Response metricsJson();
     Response metricsHistory();
     Response connections();
     Response rocksdb();
     Response rocksdbInternal();
     Response docTotal();
     Response etc();
     Response searchConfig();
     Response cache();
     Response updateCounters(const std::map<std::string, std::string>& params = {});
     Response updateCountersPost(const nlohmann::json& body = nlohmann::json::object());
     Response debugCounters();
     Response repair(const std::map<std::string, std::string>& params = {});
     Response repairPost(const nlohmann::json& body = nlohmann::json::object());
     Response info();
     Response flush();
     Response ping();
     Response integrity();
     Response consistency();
     Response selfCheck();
     Response storageStatus();

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
     Response getCollectionLanguage(const std::string& name);
     Response copyCollection(const std::string& source_name, const std::string& target_name, int batch_size = 500);

     /* Documents API */

     std::shared_ptr<Documents> documents();
     Response listDocuments(const std::string& collection_name, const std::map<std::string, std::string>& params = {});
     Response getDocument(const std::string& collection_name, const std::string& document_id);
     Response exportDocuments(const std::string& collection_name, const std::map<std::string, std::string>& params = {});
     Response facetCounts(const std::string& collection_name, const std::map<std::string, std::string>& params = {});
     Response maybe(const std::string& collection_name, const std::map<std::string, std::string>& params = {});
     Response documentContext(const std::string& collection_name, const std::string& document_id,
                              const std::map<std::string, std::string>& params = {});

     /* Search API */

     [[deprecated("Use client.collections()->search(...) for collection-scoped searches.")]]
     std::shared_ptr<Search> searchApi();
     [[deprecated("Use client.collections()->search(...) for collection-scoped searches.")]]
     Response search(const std::string& collection_name, const std::map<std::string, std::string>& params = {});
     Response searchPost(const std::string& collection_name, const nlohmann::json& body);
     [[deprecated("Use client.sql(collection_name, sql, ...) for collection-scoped SQL searches.")]]
     Response sqlSearch(const std::string& collection_name, const std::string& sql,
                        const std::map<std::string, std::string>& params = {});
     Response sql(const std::string& collection_name, const std::string& sql,
                  const std::map<std::string, std::string>& params = {});
     Response sql(const std::string& sql, const std::map<std::string, std::string>& query_params = {});
     Response execSql(const std::string& sql);
     [[deprecated("Use client.collections()->vectorSearch(...) for collection-scoped vector searches.")]]
     Response vectorSearch(const std::string& collection_name, const std::map<std::string, std::string>& params = {});
     Response vectorSearchPost(const std::string& collection_name, const nlohmann::json& body);
     Response multiSearch(const std::vector<nlohmann::json>& searches);
     Response multiSearch(const std::map<std::string, std::string>& params);
     MultiSearchResult multiSearchStructured(const std::vector<nlohmann::json>& searches);
     Response globalSearch(const std::map<std::string, std::string>& params = {});
     Response globalSearchPost(const nlohmann::json& body);

     /* Execute arbitrary request */

     Response executeRequest(const std::string& method, const std::string& path,
                             const nlohmann::json& body = nullptr,
                             const std::map<std::string, std::string>& query_params = {});

   private:

     std::shared_ptr<Request> request_;
     std::shared_ptr<Collections> collections_;
     std::shared_ptr<Documents> documents_;
     std::shared_ptr<Search> search_;
     std::shared_ptr<System> system_;
     std::shared_ptr<Aliases> aliases_;
     std::shared_ptr<Overrides> overrides_;
     std::shared_ptr<Synonyms> synonyms_;
     std::shared_ptr<Stopwords> stopwords_;
     std::shared_ptr<Keys> keys_;
     std::shared_ptr<Users> users_;
     std::shared_ptr<Modules> modules_;
     std::shared_ptr<Presets> presets_;
     std::shared_ptr<Analytics> analytics_;
};

}
