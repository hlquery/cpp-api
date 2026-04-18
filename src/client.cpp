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
     return request_->execute("GET", "/health");
}

Response Client::stats()
{
     return request_->execute("GET", "/stats");
}

Response Client::etc()
{
     return request_->execute("GET", "/etc");
}

Response Client::info()
{
     return request_->execute("GET", "/");
}

Response Client::flush()
{
     return request_->execute("POST", "/flush");
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
     return search_->search(collection_name, params);
}

Response Client::sqlSearch(const std::string& collection_name, const std::string& sql,
                           const std::map<std::string, std::string>& params)
{
     return search_->sql(collection_name, sql, params);
}

Response Client::sql(const std::string& sql, const std::map<std::string, std::string>& query_params)
{
     if (sql.empty())
     {
          throw std::invalid_argument("SQL query must be a non-empty string");
     }

     std::map<std::string, std::string> params = query_params;
     params["sql"] = sql;
     return request_->execute("GET", "/sql", nullptr, params);
}

Response Client::execSql(const std::string& sql)
{
     if (sql.empty())
     {
          throw std::invalid_argument("SQL query must be a non-empty string");
     }

     nlohmann::json body;
     body["exec"] = sql;
     return request_->execute("POST", "/sql", body);
}

Response Client::vectorSearch(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     return search_->vectorSearch(collection_name, params);
}

Response Client::executeRequest(const std::string& method, const std::string& path,
                                const nlohmann::json& body, const std::map<std::string, std::string>& query_params)
{
     return request_->execute(method, path, body, query_params);
}

}
