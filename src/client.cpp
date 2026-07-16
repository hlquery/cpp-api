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
     aliases_ = std::make_shared<Aliases>(request_);
     overrides_ = std::make_shared<Overrides>(request_);
     synonyms_ = std::make_shared<Synonyms>(request_);
     stopwords_ = std::make_shared<Stopwords>(request_);
     keys_ = std::make_shared<Keys>(request_);
     users_ = std::make_shared<Users>(request_);
     modules_ = std::make_shared<Modules>(request_);
     presets_ = std::make_shared<Presets>(request_);
     analytics_ = std::make_shared<Analytics>(request_);
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

Response Client::ready()
{
     return system_->ready();
}

Response Client::status()
{
     return system_->status();
}

Response Client::query()
{
     return system_->query();
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

Response Client::metricsHistory()
{
     return system_->metricsHistory();
}

Response Client::metricsHistoryAlias()
{
     return system_->metricsHistoryAlias();
}

Response Client::configFiles()
{
     return system_->configFiles();
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

Response Client::searchConfig()
{
     return system_->searchConfig();
}

Response Client::cache()
{
     return system_->cache();
}

Response Client::updateCounters(const std::map<std::string, std::string>& params)
{
     return system_->updateCounters(params);
}

Response Client::updateCountersPost(const nlohmann::json& body)
{
     return system_->updateCountersPost(body);
}

Response Client::debugCounters()
{
     return system_->debugCounters();
}

Response Client::repair(const std::map<std::string, std::string>& params)
{
     return system_->repair(params);
}

Response Client::repairPost(const nlohmann::json& body)
{
     return system_->repairPost(body);
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

std::shared_ptr<Aliases> Client::aliases()
{
     return aliases_;
}

std::shared_ptr<Overrides> Client::overrides()
{
     return overrides_;
}

std::shared_ptr<Synonyms> Client::synonyms()
{
     return synonyms_;
}

std::shared_ptr<Stopwords> Client::stopwords()
{
     return stopwords_;
}

std::shared_ptr<Keys> Client::keys()
{
     return keys_;
}

std::shared_ptr<Users> Client::users()
{
     return users_;
}

std::shared_ptr<Modules> Client::modules()
{
     return modules_;
}

std::shared_ptr<Presets> Client::presets()
{
     return presets_;
}

std::shared_ptr<Analytics> Client::analytics()
{
     return analytics_;
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

Response Client::createCollection(const std::string& name, const nlohmann::json& schema)
{
     return collections_->create(name, schema);
}

Response Client::updateCollection(const std::string& name, const nlohmann::json& schema)
{
     return collections_->update(name, schema);
}

Response Client::deleteCollection(const std::string& name)
{
     return collections_->remove(name);
}

Response Client::getCollectionFields(const std::string& name)
{
     return collections_->getFields(name);
}

Response Client::getCollectionLanguage(const std::string& name)
{
     return collections_->getLanguage(name);
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

Response Client::addDocument(const std::string& collection_name, const nlohmann::json& document)
{
     return documents_->add(collection_name, document);
}

Response Client::updateDocument(const std::string& collection_name, const std::string& document_id, const nlohmann::json& document)
{
     return documents_->update(collection_name, document_id, document);
}

Response Client::deleteDocument(const std::string& collection_name, const std::string& document_id)
{
     return documents_->remove(collection_name, document_id);
}

Response Client::importDocuments(const std::string& collection_name, const std::vector<nlohmann::json>& documents)
{
     return documents_->importDocuments(collection_name, documents);
}

Response Client::deleteDocumentsByFilter(const std::string& collection_name, const std::string& filter)
{
     return documents_->deleteByFilter(collection_name, filter);
}

Response Client::updateDocumentsByQuery(const std::string& collection_name, const nlohmann::json& body)
{
     return documents_->updateByQuery(collection_name, body);
}

Response Client::deleteDocumentsByQuery(const std::string& collection_name, const nlohmann::json& body)
{
     return documents_->deleteByQuery(collection_name, body);
}

Response Client::exportDocuments(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     return documents_->exportDocuments(collection_name, params);
}

Response Client::exportDocumentsPost(const std::string& collection_name, const nlohmann::json& body)
{
     return documents_->exportDocumentsPost(collection_name, body);
}

Response Client::facetCounts(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     return documents_->facetCounts(collection_name, params);
}

Response Client::facetCountsPost(const std::string& collection_name, const nlohmann::json& body)
{
     return documents_->facetCountsPost(collection_name, body);
}

Response Client::maybe(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     return documents_->maybe(collection_name, params);
}

Response Client::maybePost(const std::string& collection_name, const nlohmann::json& body)
{
     return documents_->maybePost(collection_name, body);
}

Response Client::documentContext(const std::string& collection_name, const std::string& document_id,
                                 const std::map<std::string, std::string>& params)
{
     return documents_->context(collection_name, document_id, params);
}

std::shared_ptr<Search> Client::searchApi()
{
     return search_;
}

Response Client::search(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     return collections_->search(collection_name, params);
}

Response Client::searchPost(const std::string& collection_name, const nlohmann::json& body)
{
     return collections_->searchPost(collection_name, body);
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

Response Client::vectorSearchPost(const std::string& collection_name, const nlohmann::json& body)
{
     return collections_->vectorSearchPost(collection_name, body);
}

Response Client::vectorSearchAlias(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     return collections_->vectorSearchAlias(collection_name, params);
}

Response Client::vectorSearchAliasPost(const std::string& collection_name, const nlohmann::json& body)
{
     return collections_->vectorSearchAliasPost(collection_name, body);
}

Response Client::multiSearch(const std::vector<nlohmann::json>& searches)
{
     return search_->multiSearch(searches);
}

Response Client::multiSearch(const std::map<std::string, std::string>& params)
{
     return search_->multiSearch(params);
}

MultiSearchResult Client::multiSearchStructured(const std::vector<nlohmann::json>& searches)
{
     return search_->multiSearchStructured(searches);
}

Response Client::globalSearch(const std::map<std::string, std::string>& params)
{
     return search_->globalSearch(params);
}

Response Client::globalSearchPost(const nlohmann::json& body)
{
     return search_->globalSearchPost(body);
}

Response Client::searchAll(const std::map<std::string, std::string>& params)
{
     return search_->searchAll(params);
}

Response Client::searchAllPost(const nlohmann::json& body)
{
     return search_->searchAllPost(body);
}

Response Client::executeRequest(const std::string& method, const std::string& path,
                                const nlohmann::json& body, const std::map<std::string, std::string>& query_params)
{
     return request_->execute(method, path, body, query_params);
}

}
