/*
 * hlquery C++ Client - Resource APIs Implementation
 * https://www.hlquery.com
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 * You are free to redistribute and/or modify this software
 * under the terms of the BSD License.
 * For more details, please visit: https://docs.hlquery.com
 */

#include "hlquery/resources.h"
#include "hlquery/utils/Url.h"
#include "hlquery/utils/Validator.h"

namespace hlquery
{

namespace
{

void requireNonEmpty(const std::string& value, const std::string& label)
{
     if (value.empty())
     {
          throw ValidationException(label + " must be a non-empty string");
     }
}

std::string collectionPath(const std::string& collection_name, const std::string& suffix)
{
     utils::validateCollectionName(collection_name);
     return "/collections/" + utils::urlEncode(collection_name) + suffix;
}

std::string encodedPath(const std::string& prefix, const std::string& value, const std::string& label)
{
     requireNonEmpty(value, label);
     return prefix + utils::urlEncode(value);
}

}

Aliases::Aliases(std::shared_ptr<Request> request) : request_(request)
{
}

Response Aliases::list()
{
     return request_->execute("GET", "/aliases");
}

Response Aliases::list(const std::string& collection_name)
{
     return request_->execute("GET", collectionPath(collection_name, "/aliases"));
}

Response Aliases::get(const std::string& name)
{
     return request_->execute("GET", encodedPath("/aliases/", name, "Alias name"));
}

Response Aliases::upsert(const std::string& name, const nlohmann::json& body)
{
     return update(name, body);
}

Response Aliases::create(const std::string& name, const nlohmann::json& body)
{
     return request_->execute("POST", encodedPath("/aliases/", name, "Alias name"), body);
}

Response Aliases::update(const std::string& name, const nlohmann::json& body)
{
     return request_->execute("PUT", encodedPath("/aliases/", name, "Alias name"), body);
}

Response Aliases::remove(const std::string& name)
{
     return request_->execute("DELETE", encodedPath("/aliases/", name, "Alias name"));
}

Synonyms::Synonyms(std::shared_ptr<Request> request) : request_(request)
{
}

Response Synonyms::list(const std::string& collection_name)
{
     return request_->execute("GET", collectionPath(collection_name, "/synonyms"));
}

Response Synonyms::get(const std::string& collection_name, const std::string& id)
{
     requireNonEmpty(id, "Synonym id");
     return request_->execute("GET", collectionPath(collection_name, "/synonyms/") + utils::urlEncode(id));
}

Response Synonyms::upsert(const std::string& collection_name, const std::string& id, const nlohmann::json& body)
{
     return update(collection_name, id, body);
}

Response Synonyms::create(const std::string& collection_name, const std::string& id, const nlohmann::json& body)
{
     requireNonEmpty(id, "Synonym id");
     return request_->execute("POST", collectionPath(collection_name, "/synonyms/") + utils::urlEncode(id), body);
}

Response Synonyms::update(const std::string& collection_name, const std::string& id, const nlohmann::json& body)
{
     requireNonEmpty(id, "Synonym id");
     return request_->execute("PUT", collectionPath(collection_name, "/synonyms/") + utils::urlEncode(id), body);
}

Response Synonyms::remove(const std::string& collection_name, const std::string& id)
{
     requireNonEmpty(id, "Synonym id");
     return request_->execute("DELETE", collectionPath(collection_name, "/synonyms/") + utils::urlEncode(id));
}

Response Synonyms::listAll()
{
     return request_->execute("GET", "/synonyms");
}

Response Synonyms::listGlobal()
{
     return request_->execute("GET", "/synonyms/global");
}

Response Synonyms::getGlobal(const std::string& id)
{
     return request_->execute("GET", encodedPath("/synonyms/global/", id, "Synonym id"));
}

Response Synonyms::upsertGlobal(const std::string& id, const nlohmann::json& body)
{
     return updateGlobal(id, body);
}

Response Synonyms::createGlobal(const std::string& id, const nlohmann::json& body)
{
     return request_->execute("POST", encodedPath("/synonyms/global/", id, "Synonym id"), body);
}

Response Synonyms::updateGlobal(const std::string& id, const nlohmann::json& body)
{
     return request_->execute("PUT", encodedPath("/synonyms/global/", id, "Synonym id"), body);
}

Response Synonyms::removeGlobal(const std::string& id)
{
     return request_->execute("DELETE", encodedPath("/synonyms/global/", id, "Synonym id"));
}

Stopwords::Stopwords(std::shared_ptr<Request> request) : request_(request)
{
}

Response Stopwords::list(const std::string& collection_name)
{
     return request_->execute("GET", collectionPath(collection_name, "/stopwords"));
}

Response Stopwords::create(const std::string& collection_name, const nlohmann::json& body)
{
     return request_->execute("POST", collectionPath(collection_name, "/stopwords"), body);
}

Response Stopwords::remove(const std::string& collection_name, const std::string& word)
{
     requireNonEmpty(word, "Stopword");
     return request_->execute("DELETE", collectionPath(collection_name, "/stopwords/") + utils::urlEncode(word));
}

Response Stopwords::listAll()
{
     return request_->execute("GET", "/stopwords");
}

Response Stopwords::listGlobal()
{
     return request_->execute("GET", "/stopwords/global");
}

Response Stopwords::createGlobal(const nlohmann::json& body)
{
     return request_->execute("POST", "/stopwords/global", body);
}

Response Stopwords::removeGlobal(const std::string& word)
{
     return request_->execute("DELETE", encodedPath("/stopwords/global/", word, "Stopword"));
}

Overrides::Overrides(std::shared_ptr<Request> request) : request_(request)
{
}

Response Overrides::list(const std::string& collection_name)
{
     return request_->execute("GET", collectionPath(collection_name, "/overrides"));
}

Response Overrides::get(const std::string& collection_name, const std::string& id)
{
     requireNonEmpty(id, "Override id");
     return request_->execute("GET", collectionPath(collection_name, "/overrides/") + utils::urlEncode(id));
}

Response Overrides::upsert(const std::string& collection_name, const std::string& id, const nlohmann::json& body)
{
     return update(collection_name, id, body);
}

Response Overrides::create(const std::string& collection_name, const std::string& id, const nlohmann::json& body)
{
     requireNonEmpty(id, "Override id");
     return request_->execute("POST", collectionPath(collection_name, "/overrides/") + utils::urlEncode(id), body);
}

Response Overrides::update(const std::string& collection_name, const std::string& id, const nlohmann::json& body)
{
     requireNonEmpty(id, "Override id");
     return request_->execute("PUT", collectionPath(collection_name, "/overrides/") + utils::urlEncode(id), body);
}

Response Overrides::remove(const std::string& collection_name, const std::string& id)
{
     requireNonEmpty(id, "Override id");
     return request_->execute("DELETE", collectionPath(collection_name, "/overrides/") + utils::urlEncode(id));
}

Keys::Keys(std::shared_ptr<Request> request) : request_(request)
{
}

Response Keys::list()
{
     return request_->execute("GET", "/keys");
}

Response Keys::get(const std::string& id)
{
     return request_->execute("GET", encodedPath("/keys/", id, "Key id"));
}

Response Keys::create(const nlohmann::json& body)
{
     return request_->execute("POST", "/keys", body);
}

Response Keys::update(const std::string& id, const nlohmann::json& body)
{
     return request_->execute("PUT", encodedPath("/keys/", id, "Key id"), body);
}

Response Keys::remove(const std::string& id)
{
     return request_->execute("DELETE", encodedPath("/keys/", id, "Key id"));
}

Users::Users(std::shared_ptr<Request> request) : request_(request)
{
}

Response Users::list()
{
     return request_->execute("GET", "/users");
}

Response Users::get(const std::string& id)
{
     return request_->execute("GET", encodedPath("/users/", id, "User id"));
}

Response Users::create(const nlohmann::json& body)
{
     return request_->execute("POST", "/users", body);
}

Response Users::update(const std::string& id, const nlohmann::json& body)
{
     return request_->execute("PUT", encodedPath("/users/", id, "User id"), body);
}

Response Users::remove(const std::string& id)
{
     return request_->execute("DELETE", encodedPath("/users/", id, "User id"));
}

Modules::Modules(std::shared_ptr<Request> request) : request_(request)
{
}

Response Modules::list()
{
     return request_->execute("GET", "/modules");
}

Response Modules::syntax(const std::string& name)
{
     return request_->execute("GET", encodedPath("/modules/", name, "Module name") + "/syntax");
}

Response Modules::load(const std::string& name, const nlohmann::json& body)
{
     return request_->execute("POST", encodedPath("/loadmodule/", name, "Module name"), body);
}

Response Modules::unload(const std::string& name, const nlohmann::json& body)
{
     return request_->execute("POST", encodedPath("/unloadmodule/", name, "Module name"), body);
}

Response Modules::call(const std::string& method, const std::string& path, const nlohmann::json& body,
                       const std::map<std::string, std::string>& query_params)
{
     requireNonEmpty(method, "HTTP method");
     requireNonEmpty(path, "Module path");
     return request_->execute(method, path, body, query_params);
}

Presets::Presets(std::shared_ptr<Request> request) : request_(request)
{
}

Response Presets::list()
{
     return request_->execute("GET", "/presets");
}

Response Presets::get(const std::string& name)
{
     return request_->execute("GET", encodedPath("/presets/", name, "Preset name"));
}

Response Presets::upsert(const std::string& name, const nlohmann::json& body)
{
     return update(name, body);
}

Response Presets::create(const std::string& name, const nlohmann::json& body)
{
     return request_->execute("POST", encodedPath("/presets/", name, "Preset name"), body);
}

Response Presets::update(const std::string& name, const nlohmann::json& body)
{
     return request_->execute("PUT", encodedPath("/presets/", name, "Preset name"), body);
}

Response Presets::remove(const std::string& name)
{
     return request_->execute("DELETE", encodedPath("/presets/", name, "Preset name"));
}

Analytics::Analytics(std::shared_ptr<Request> request) : request_(request)
{
}

Response Analytics::click(const nlohmann::json& body)
{
     return request_->execute("POST", "/analytics/click", body);
}

}
