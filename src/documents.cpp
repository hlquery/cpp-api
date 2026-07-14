/*
 * hlquery C++ Client - Documents API Implementation
 * https://www.hlquery.com
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 * You are free to redistribute and/or modify this software
 * under the terms of the BSD License.
 * For more details, please visit: https://docs.hlquery.com
 */

#include <sstream>

#include "hlquery/documents.h"
#include "hlquery/utils/Url.h"
#include "hlquery/utils/Validator.h"

namespace hlquery
{

Documents::Documents(std::shared_ptr<Request> request) : request_(request)
{
}

Response Documents::list(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     utils::validateCollectionName(collection_name);
     utils::validateSearchParams(params);

     std::map<std::string, std::string> query_params = params;

     int offset = 0;
     if (params.find("offset") != params.end())
     {
          offset = std::stoi(params.at("offset"));
     }
     else if (params.find("from") != params.end())
     {
          offset = std::stoi(params.at("from"));
     }

     int limit = 10;
     if (params.find("limit") != params.end())
     {
          limit = std::stoi(params.at("limit"));
     }
     else if (params.find("size") != params.end())
     {
          limit = std::stoi(params.at("size"));
     }

     query_params["offset"] = std::to_string(offset);
     query_params["limit"] = std::to_string(limit);

     std::string path = "/collections/" + utils::urlEncode(collection_name) + "/documents";
     return request_->execute("GET", path, nullptr, query_params);
}

Response Documents::get(const std::string& collection_name, const std::string& document_id)
{
     utils::validateCollectionName(collection_name);
     utils::validateDocumentId(document_id);

     std::string path = "/collections/" + utils::urlEncode(collection_name) +
                        "/documents/" + utils::urlEncode(document_id);
     return request_->execute("GET", path);
}

Response Documents::add(const std::string& collection_name, const nlohmann::json& document)
{
     utils::validateCollectionName(collection_name);

     std::string path = "/collections/" + utils::urlEncode(collection_name) + "/documents";
     return request_->execute("POST", path, document);
}

Response Documents::update(const std::string& collection_name, const std::string& document_id, const nlohmann::json& document)
{
     utils::validateCollectionName(collection_name);
     utils::validateDocumentId(document_id);

     std::string path = "/collections/" + utils::urlEncode(collection_name) +
                        "/documents/" + utils::urlEncode(document_id);
     return request_->execute("PUT", path, document);
}

Response Documents::remove(const std::string& collection_name, const std::string& document_id)
{
     utils::validateCollectionName(collection_name);
     utils::validateDocumentId(document_id);

     std::string path = "/collections/" + utils::urlEncode(collection_name) +
                        "/documents/" + utils::urlEncode(document_id);
     return request_->execute("DELETE", path);
}

Response Documents::importDocuments(const std::string& collection_name, const std::vector<nlohmann::json>& documents)
{
     utils::validateCollectionName(collection_name);

     nlohmann::json payload;
     payload["documents"] = nlohmann::json::array();
     for (const auto& doc : documents)
     {
          payload["documents"].push_back(doc);
     }

     std::string path = "/collections/" + utils::urlEncode(collection_name) + "/documents/import";
     return request_->execute("POST", path, payload);
}

Response Documents::deleteByFilter(const std::string& collection_name, const std::string& filter)
{
     utils::validateCollectionName(collection_name);

     std::map<std::string, std::string> params;
     params["filter_by"] = filter;

     std::string path = "/collections/" + utils::urlEncode(collection_name) + "/documents";
     return request_->execute("DELETE", path, nullptr, params);
}

Response Documents::updateByQuery(const std::string& collection_name, const nlohmann::json& body)
{
     utils::validateCollectionName(collection_name);

     std::string path = "/collections/" + utils::urlEncode(collection_name) + "/documents/_update_by_query";
     return request_->execute("POST", path, body);
}

Response Documents::deleteByQuery(const std::string& collection_name, const nlohmann::json& body)
{
     utils::validateCollectionName(collection_name);

     std::string path = "/collections/" + utils::urlEncode(collection_name) + "/documents/_delete_by_query";
     return request_->execute("POST", path, body);
}

Response Documents::facetCounts(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     utils::validateCollectionName(collection_name);
     utils::validateSearchParams(params);

     std::string path = "/collections/" + utils::urlEncode(collection_name) + "/documents/facet_counts";
     return request_->execute("GET", path, nullptr, params);
}

Response Documents::facetCountsPost(const std::string& collection_name, const nlohmann::json& body)
{
     utils::validateCollectionName(collection_name);

     std::string path = "/collections/" + utils::urlEncode(collection_name) + "/documents/facet_counts";
     return request_->execute("POST", path, body);
}

Response Documents::exportDocuments(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     utils::validateCollectionName(collection_name);

     std::string path = "/collections/" + utils::urlEncode(collection_name) + "/documents/export";
     return request_->execute("GET", path, nullptr, params);
}

Response Documents::exportDocumentsPost(const std::string& collection_name, const nlohmann::json& body)
{
     utils::validateCollectionName(collection_name);

     std::string path = "/collections/" + utils::urlEncode(collection_name) + "/documents/export";
     return request_->execute("POST", path, body);
}

Response Documents::maybe(const std::string& collection_name, const std::map<std::string, std::string>& params)
{
     utils::validateCollectionName(collection_name);
     utils::validateSearchParams(params);

     std::string path = "/collections/" + utils::urlEncode(collection_name) + "/documents/maybe";
     return request_->execute("GET", path, nullptr, params);
}

Response Documents::maybePost(const std::string& collection_name, const nlohmann::json& body)
{
     utils::validateCollectionName(collection_name);

     std::string path = "/collections/" + utils::urlEncode(collection_name) + "/documents/maybe";
     return request_->execute("POST", path, body);
}

Response Documents::context(const std::string& collection_name, const std::string& document_id,
                            const std::map<std::string, std::string>& params)
{
     utils::validateCollectionName(collection_name);
     utils::validateDocumentId(document_id);

     std::string path = "/collections/" + utils::urlEncode(collection_name) +
                        "/documents/" + utils::urlEncode(document_id) + "/context";
     return request_->execute("GET", path, nullptr, params);
}

}
