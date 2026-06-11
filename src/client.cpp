/*
 * hlquery C++ Client - Main Client Class Implementation
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

#include "hlquery/client.h"
#include "hlquery/utils/Config.h"

namespace hlquery
{

Client::Client(const std::string& base_url, const std::map<std::string, std::string>& options)
{
     utils::ConfigOptions config = utils::mergeDefaults(options);

     std::string url = base_url.empty() ? config.base_url : base_url;
     url = utils::normalizeUrl(url);

     if (!utils::isValidUrl(url))
     {
          throw std::invalid_argument("Invalid base URL: " + url);
     }

     request_ = std::make_shared<Request>(url, config.timeout, config.token, config.auth_method, config.tls_verify);
     collections_ = std::make_shared<Collections>(request_);
     documents_ = std::make_shared<Documents>(request_);
     search_ = std::make_shared<Search>(request_, collections_);
     system_ = std::make_shared<System>(request_);
}

void Client::setAuthToken(const std::string& token, const std::string& method)
{
     request_->setAuthToken(token, method);
}

void Client::clearAuth()
{
     request_->clearAuth();
}

Response Client::health()
{
     return system_->health();
}

Response Client::status()
{
     return system_->status();
}

Response Client::startup()
{
     return system_->startup();
}

Response Client::bootStatus()
{
     return system_->bootStatus();
}

Response Client::stats()
{
     return system_->stats();
}

Response Client::metrics()
{
     return system_->metrics();
}

Response Client::metricsJson()
{
     return system_->metricsJson();
}

Response Client::connections()
{
     return system_->connections();
}

Response Client::rocksdb()
{
     return system_->rocksdb();
}

Response Client::rocksdbInternal()
{
     return system_->rocksdbInternal();
}

Response Client::docTotal()
{
     return system_->docTotal();
}

Response Client::etc()
{
     return system_->etc();
}

Response Client::info()
{
     return system_->info();
}

Response Client::flush()
{
     return request_->execute("POST", "/flush");
}

Response Client::ping()
{
     return system_->ping();
}

Response Client::integrity()
{
     return system_->integrity();
}

Response Client::consistency()
{
     return system_->consistency();
}

Response Client::selfCheck()
{
     return system_->selfCheck();
}

Response Client::storageStatus()
{
     return system_->storageStatus();
}

Response Client::links()
{
     return request_->execute("GET", "/links");
}

Response Client::linksPing()
{
     return request_->execute("GET", "/links/ping");
}

Response Client::linksConnect(const std::string& endpoint_or_host, int port)
{
     nlohmann::json body;
     if (port > 0)
     {
          body["host"] = endpoint_or_host;
          body["port"] = port;
     }
     else
     {
          body["endpoint"] = endpoint_or_host;
     }
     return request_->execute("POST", "/links/connect", body);
}

Response Client::linksDisconnect(const std::string& endpoint_or_host, int port)
{
     nlohmann::json body;
     if (port > 0)
     {
          body["host"] = endpoint_or_host;
          body["port"] = port;
     }
     else
     {
          body["endpoint"] = endpoint_or_host;
     }
     return request_->execute("POST", "/links/disconnect", body);
}

std::shared_ptr<Collections> Client::collections()
{
     return collections_;
}

std::shared_ptr<System> Client::system()
{
     return system_;
}

Response Client::listCollections(int offset, int limit)
{
     return collections_->list(offset, limit);
}

Response Client::listCollectionsDistributed()
{
     return request_->execute("GET", "/collections/distributed");
}

Response Client::getCollection(const std::string& name)
{
     return collections_->get(name);
}

Response Client::getCollectionFields(const std::string& name)
{
     return collections_->getFields(name);
}

Response Client::copyCollection(const std::string& source_name, const std::string& target_name, int batch_size)
{
     return collections_->copy(source_name, target_name, batch_size);
}

std::shared_ptr<Documents> Client::documents()
{
     return documents_;
}

Response Client::listDocuments(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     return documents_->list(collection_name, params);
}

Response Client::getDocument(const std::string& collection_name, const std::string& document_id)
{
     return documents_->get(collection_name, document_id);
}

std::shared_ptr<Search> Client::searchApi()
{
     return search_;
}

Response Client::search(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     return collections_->search(collection_name, params);
}

Response Client::sqlSearch(const std::string& collection_name, const std::string& sql,
                           const std::map<std::string, std::string>& params)
{
     return this->sql(collection_name, sql, params);
}

Response Client::sql(const std::string& collection_name, const std::string& sql,
                     const std::map<std::string, std::string>& params)
{
     return collections_->sql(collection_name, sql, params);
}

Response Client::sql(const std::string& sql, const std::map<std::string, std::string>& query_params)
{
     return system_->sql(sql, query_params);
}

Response Client::execSql(const std::string& sql)
{
     return system_->execSql(sql);
}

Response Client::vectorSearch(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     return collections_->vectorSearch(collection_name, params);
}

Response Client::multiSearch(const std::vector<nlohmann::json>& searches)
{
     return search_->multiSearch(searches);
}

MultiSearchResult Client::multiSearchStructured(const std::vector<nlohmann::json>& searches)
{
     return search_->multiSearchStructured(searches);
}

Response Client::executeRequest(const std::string& method, const std::string& path,
                                const nlohmann::json& body, const std::map<std::string, std::string>& query_params)
{
     return request_->execute(method, path, body, query_params);
}

}
