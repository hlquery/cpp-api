/*
 * hlquery C++ Client - Resource APIs
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

#include "request.h"
#include "response.h"

namespace hlquery
{

class Aliases
{
   public:

     explicit Aliases(std::shared_ptr<Request> request);

     Response list();
     Response list(const std::string& collection_name);
     Response get(const std::string& name);
     Response upsert(const std::string& name, const nlohmann::json& body);
     Response create(const std::string& name, const nlohmann::json& body);
     Response update(const std::string& name, const nlohmann::json& body);
     Response remove(const std::string& name);

   private:

     std::shared_ptr<Request> request_;
};

class Synonyms
{
   public:

     explicit Synonyms(std::shared_ptr<Request> request);

     Response list(const std::string& collection_name);
     Response get(const std::string& collection_name, const std::string& id);
     Response upsert(const std::string& collection_name, const std::string& id, const nlohmann::json& body);
     Response create(const std::string& collection_name, const std::string& id, const nlohmann::json& body);
     Response update(const std::string& collection_name, const std::string& id, const nlohmann::json& body);
     Response remove(const std::string& collection_name, const std::string& id);
     Response listSet(const std::string& collection_name);
     Response getSet(const std::string& collection_name, const std::string& id);
     Response createSet(const std::string& collection_name, const std::string& id, const nlohmann::json& body);
     Response updateSet(const std::string& collection_name, const std::string& id, const nlohmann::json& body);
     Response removeSet(const std::string& collection_name, const std::string& id);
     Response listAll();
     Response listGlobal();
     Response getGlobal(const std::string& id);
     Response upsertGlobal(const std::string& id, const nlohmann::json& body);
     Response createGlobal(const std::string& id, const nlohmann::json& body);
     Response updateGlobal(const std::string& id, const nlohmann::json& body);
     Response removeGlobal(const std::string& id);
     Response listAllSets();
     Response listGlobalSet();
     Response getGlobalSet(const std::string& id);
     Response createGlobalSet(const std::string& id, const nlohmann::json& body);
     Response updateGlobalSet(const std::string& id, const nlohmann::json& body);
     Response removeGlobalSet(const std::string& id);
     Response getGlobalSetItem(const std::string& id);
     Response createGlobalSetItem(const std::string& id, const nlohmann::json& body);
     Response updateGlobalSetItem(const std::string& id, const nlohmann::json& body);
     Response removeGlobalSetItem(const std::string& id);

   private:

     std::shared_ptr<Request> request_;
};

class Stopwords
{
   public:

     explicit Stopwords(std::shared_ptr<Request> request);

     Response list(const std::string& collection_name);
     Response create(const std::string& collection_name, const nlohmann::json& body);
     Response remove(const std::string& collection_name, const std::string& word);
     Response listSet(const std::string& collection_name);
     Response createSet(const std::string& collection_name, const nlohmann::json& body);
     Response removeSet(const std::string& collection_name, const std::string& word);
     Response listAll();
     Response listGlobal();
     Response createGlobal(const nlohmann::json& body);
     Response removeGlobal(const std::string& word);
     Response listAllSets();
     Response listGlobalSet();
     Response createGlobalSet(const nlohmann::json& body);
     Response removeGlobalSet(const std::string& word);
     Response removeGlobalSetItem(const std::string& word);

   private:

     std::shared_ptr<Request> request_;
};

class Overrides
{
   public:

     explicit Overrides(std::shared_ptr<Request> request);

     Response list(const std::string& collection_name);
     Response get(const std::string& collection_name, const std::string& id);
     Response upsert(const std::string& collection_name, const std::string& id, const nlohmann::json& body);
     Response create(const std::string& collection_name, const std::string& id, const nlohmann::json& body);
     Response update(const std::string& collection_name, const std::string& id, const nlohmann::json& body);
     Response remove(const std::string& collection_name, const std::string& id);
     Response listCurations(const std::string& collection_name);
     Response getCuration(const std::string& collection_name, const std::string& id);
     Response upsertCuration(const std::string& collection_name, const std::string& id, const nlohmann::json& body);
     Response createCuration(const std::string& collection_name, const std::string& id, const nlohmann::json& body);
     Response updateCuration(const std::string& collection_name, const std::string& id, const nlohmann::json& body);
     Response removeCuration(const std::string& collection_name, const std::string& id);
     Response listCurationSet(const std::string& collection_name);
     Response getCurationSet(const std::string& collection_name, const std::string& id);
     Response upsertCurationSet(const std::string& collection_name, const std::string& id, const nlohmann::json& body);
     Response createCurationSet(const std::string& collection_name, const std::string& id, const nlohmann::json& body);
     Response updateCurationSet(const std::string& collection_name, const std::string& id, const nlohmann::json& body);
     Response removeCurationSet(const std::string& collection_name, const std::string& id);

   private:

     std::shared_ptr<Request> request_;
};

class Keys
{
   public:

     explicit Keys(std::shared_ptr<Request> request);

     Response list();
     Response get(const std::string& id);
     Response create(const nlohmann::json& body);
     Response update(const std::string& id, const nlohmann::json& body);
     Response remove(const std::string& id);

   private:

     std::shared_ptr<Request> request_;
};

class Users
{
   public:

     explicit Users(std::shared_ptr<Request> request);

     Response list();
     Response get(const std::string& id);
     Response create(const nlohmann::json& body);
     Response update(const std::string& id, const nlohmann::json& body);
     Response remove(const std::string& id);

   private:

     std::shared_ptr<Request> request_;
};

class Modules
{
   public:

     explicit Modules(std::shared_ptr<Request> request);

     Response list();
     Response syntax(const std::string& name);
     Response load(const std::string& name, const nlohmann::json& body = nlohmann::json::object());
     Response unload(const std::string& name, const nlohmann::json& body = nlohmann::json::object());
     Response loadViaModules(const std::string& name, const nlohmann::json& body = nlohmann::json::object());
     Response unloadViaModules(const std::string& name, const nlohmann::json& body = nlohmann::json::object());
     Response call(const std::string& method, const std::string& path, const nlohmann::json& body = nullptr,
                   const std::map<std::string, std::string>& query_params = {});

   private:

     std::shared_ptr<Request> request_;
};

class Presets
{
   public:

     explicit Presets(std::shared_ptr<Request> request);

     Response list();
     Response get(const std::string& name);
     Response upsert(const std::string& name, const nlohmann::json& body);
     Response create(const std::string& name, const nlohmann::json& body);
     Response update(const std::string& name, const nlohmann::json& body);
     Response remove(const std::string& name);

   private:

     std::shared_ptr<Request> request_;
};

class Analytics
{
   public:

     explicit Analytics(std::shared_ptr<Request> request);

     Response click(const nlohmann::json& body);

   private:

     std::shared_ptr<Request> request_;
};

}
