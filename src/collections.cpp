/*
 * hlquery C++ Client - Collections API Implementation
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
#include <set>
#include <stdexcept>

#include "hlquery/collections.h"
#include "hlquery/utils/Url.h"
#include "hlquery/utils/Validator.h"

namespace hlquery
{


nlohmann::json buildCreatePayloadFromCollectionInfo(const nlohmann::json& source_info, const std::string& target_name)
{
     nlohmann::json payload;
     payload["name"] = target_name;

     if (source_info.contains("fields") && source_info["fields"].is_object() && !source_info["fields"].empty())
     {
          payload["fields"] = nlohmann::json::array();

          std::set<std::string> field_names;
          for (auto it = source_info["fields"].begin(); it != source_info["fields"].end(); ++it)
          {
               field_names.insert(it.key());
          }

          for (const auto& field_name : field_names)
          {
               nlohmann::json field;
               field["name"] = field_name;
               field["type"] = source_info["fields"][field_name].is_string() ? source_info["fields"][field_name].get<std::string>() : "string";
               payload["fields"].push_back(field);
          }
     }
     else if (source_info.contains("searchable_fields") && source_info["searchable_fields"].is_array() &&
              !source_info["searchable_fields"].empty())
     {
          payload["searchable_fields"] = source_info["searchable_fields"];
     }
     else
     {
          payload["searchable_fields"] = nlohmann::json::array({"title", "content"});
     }

     if (source_info.contains("metadata") && source_info["metadata"].is_object())
     {
          for (auto it = source_info["metadata"].begin(); it != source_info["metadata"].end(); ++it)
          {
               if (!it.key().empty() && it.key()[0] == '_')
               {
                    payload[it.key()] = it.value();
               }
          }
     }

     return payload;
}

nlohmann::json makeErrorBody(const std::string& error, const std::string& message)
{
     nlohmann::json body;
     body["error"] = error;
     body["message"] = message;
     return body;
}


Collections::Collections(std::shared_ptr<Request> request) : request_(request)
{
}

std::string Collections::buildQueryString(const std::map<std::string, std::string>& params) const
{
     std::ostringstream query;
     bool first = true;

     for (const auto& param : params)
     {
          if (!first)
          {
               query << "&";
          }

          query << utils::urlEncode(param.first) << "=" << utils::urlEncode(param.second);
          first = false;
     }

     return query.str();
}

Response Collections::list(int offset, int limit)
{
     utils::validatePagination(offset, limit);

     std::map<std::string, std::string> params;
     params["offset"] = std::to_string(offset);
     params["limit"] = std::to_string(limit);

     return request_->execute("GET", "/collections", nullptr, params);
}

Response Collections::get(const std::string& name)
{
     utils::validateCollectionName(name);

     std::string path = "/collections/" + utils::urlEncode(name);
     return request_->execute("GET", path);
}

Response Collections::create(const std::string& name, const nlohmann::json& schema)
{
     utils::validateCollectionName(name);

     nlohmann::json payload;
     payload["name"] = name;

     /* Extract fields from schema - API expects 'fields' directly (not nested in 'schema') */

     if (schema.is_object() && schema.contains("fields") && schema["fields"].is_array())
     {
          payload["fields"] = schema["fields"];
     }
     else if (schema.is_object())
     {
          /* If schema is an object, merge its fields into payload */

          for (auto it = schema.begin(); it != schema.end(); ++it)
          {
               if (it.key() != "name")
               {
                    payload[it.key()] = it.value();
               }
          }
     }
     else
     {
          /* If schema is already the fields array or other format, use it directly */

          payload["fields"] = schema;
     }

     return request_->execute("POST", "/collections", payload);
}

Response Collections::remove(const std::string& name)
{
     utils::validateCollectionName(name);

     std::string path = "/collections/" + utils::urlEncode(name);
     return request_->execute("DELETE", path);
}

Response Collections::update(const std::string& name, const nlohmann::json& schema)
{
     utils::validateCollectionName(name);

     std::string path = "/collections/" + utils::urlEncode(name) + "/update";
     return request_->execute("POST", path, schema);
}

Response Collections::getFields(const std::string& name)
{
     Response response = get(name);

     if (response.getStatusCode() != 200)
     {
          return response;
     }

     nlohmann::json body = response.getBody();
     nlohmann::json fields = nlohmann::json::array();
     std::map<std::string, std::vector<std::string>> field_types;

     /* Collect searchable fields */

     if (body.contains("searchable_fields") && body["searchable_fields"].is_array())
     {
          for (const auto& field : body["searchable_fields"])
          {
               std::string field_name = field.get<std::string>();
               if (field_types.find(field_name) == field_types.end())
               {
                    field_types[field_name] = std::vector<std::string>();
               }
               field_types[field_name].push_back("searchable");
          }
     }

     /* Collect filterable fields */

     if (body.contains("filterable_fields") && body["filterable_fields"].is_array())
     {
          for (const auto& field : body["filterable_fields"])
          {
               std::string field_name = field.get<std::string>();
               if (field_types.find(field_name) == field_types.end())
               {
                    field_types[field_name] = std::vector<std::string>();
               }
               field_types[field_name].push_back("filterable");
          }
     }

     /* Collect sortable fields */

     if (body.contains("sortable_fields") && body["sortable_fields"].is_array())
     {
          for (const auto& field : body["sortable_fields"])
          {
               std::string field_name = field.get<std::string>();
               if (field_types.find(field_name) == field_types.end())
               {
                    field_types[field_name] = std::vector<std::string>();
               }
               field_types[field_name].push_back("sortable");
          }
     }

     /* Format fields */

     for (const auto& pair : field_types)
     {
          nlohmann::json field;
          field["name"] = pair.first;

          std::ostringstream type_stream;
          bool first = true;
          for (const auto& type : pair.second)
          {
               if (!first)
                    type_stream << ", ";
               type_stream << type;
               first = false;
          }
          field["type"] = type_stream.str();

          fields.push_back(field);
     }

     nlohmann::json result;
     result["collection"] = name;
     result["fields"] = fields;
     result["field_count"] = fields.size();
     result["searchable_fields"] = body.value("searchable_fields", nlohmann::json::array());
     result["filterable_fields"] = body.value("filterable_fields", nlohmann::json::array());
     result["sortable_fields"] = body.value("sortable_fields", nlohmann::json::array());

     return Response(200, result);
}

Response Collections::search(const std::string& name, const std::map<std::string, std::string>& params)
{
     utils::validateCollectionName(name);
     std::map<std::string, std::string> query_params = params;

     if (query_params.find("q") == query_params.end())
     {
          const auto like_it = query_params.find("like");
          if (like_it != query_params.end())
          {
               query_params["q"] = like_it->second;
               query_params.erase(like_it);
          }
     }

     utils::validateSearchParams(query_params);

     if (query_params.find("q") != query_params.end() &&
         query_params.find("query_by") == query_params.end() &&
         !query_params["q"].empty())
     {
          Response collection = get(name);
          if (collection.getStatusCode() == 200)
          {
               nlohmann::json body = collection.getBody();
               if (body.contains("searchable_fields") && body["searchable_fields"].is_array() &&
                   !body["searchable_fields"].empty())
               {
                    std::ostringstream fields;
                    bool first = true;
                    for (const auto& field : body["searchable_fields"])
                    {
                         if (!first)
                         {
                              fields << ",";
                         }
                         fields << field.get<std::string>();
                         first = false;
                    }
                    query_params["query_by"] = fields.str();
               }
          }
     }

     std::string path = "/collections/" + utils::urlEncode(name) + "/documents/search";
     std::string query_string = buildQueryString(query_params);

     if (!query_string.empty())
     {
          path += "?" + query_string;
     }

     return request_->execute("GET", path);
}

SearchResult Collections::searchStructured(const std::string& name, const std::map<std::string, std::string>& params)
{
     return SearchResult(search(name, params));
}

Response Collections::sql(const std::string& name, const std::string& sql,
                          const std::map<std::string, std::string>& params)
{
     utils::validateCollectionName(name);

     if (sql.empty())
     {
          throw std::invalid_argument("SQL query must be a non-empty string");
     }

     std::map<std::string, std::string> query_params = params;
     query_params["sql"] = sql;

     std::string path = "/collections/" + utils::urlEncode(name) + "/documents/search";
     std::string query_string = buildQueryString(query_params);

     if (!query_string.empty())
     {
          path += "?" + query_string;
     }

     return request_->execute("GET", path);
}

Response Collections::vectorSearch(const std::string& name, const std::map<std::string, std::string>& params)
{
     utils::validateCollectionName(name);

     std::string path = "/collections/" + utils::urlEncode(name) + "/vector_search";
     std::string query_string = buildQueryString(params);

     if (!query_string.empty())
     {
          path += "?" + query_string;
     }

     return request_->execute("GET", path);
}

SearchResult Collections::vectorSearchStructured(const std::string& name, const std::map<std::string, std::string>& params)
{
     return SearchResult(vectorSearch(name, params));
}

Response Collections::copy(const std::string& source_name, const std::string& target_name, int batch_size)
{
     utils::validateCollectionName(source_name);
     utils::validateCollectionName(target_name);

     if (source_name == target_name)
     {
          return Response(400, makeErrorBody("Invalid arguments", "Source and target collection names must differ"));
     }

     if (batch_size <= 0)
     {
          return Response(400, makeErrorBody("Invalid arguments", "Batch size must be a positive integer"));
     }

     Response source_info = get(source_name);
     if (source_info.getStatusCode() != 200)
     {
          return source_info;
     }

     Response target_check = get(target_name);
     if (target_check.getStatusCode() == 200)
     {
          return Response(409, makeErrorBody("Collection exists", "Target collection already exists: " + target_name));
     }
     if (target_check.getStatusCode() != 404)
     {
          /* If we got something other than "not found", bubble it up. */
          return target_check;
     }

     nlohmann::json create_payload = buildCreatePayloadFromCollectionInfo(source_info.getBody(), target_name);
     Response create_response = request_->execute("POST", "/collections", create_payload);
     if (create_response.getStatusCode() != 201)
     {
          return create_response;
     }

     int offset = 0;
     int copied_count = 0;

     while (true)
     {
          std::map<std::string, std::string> params;
          params["offset"] = std::to_string(offset);
          params["limit"] = std::to_string(batch_size);
          params["distributed"] = "off";

          std::string list_path = "/collections/" + utils::urlEncode(source_name) + "/documents";
          Response list_response = request_->execute("GET", list_path, nullptr, params);

          if (list_response.getStatusCode() == 400)
          {
               params.erase("distributed");
               list_response = request_->execute("GET", list_path, nullptr, params);
          }

          if (list_response.getStatusCode() != 200)
          {
               return list_response;
          }

          nlohmann::json list_body = list_response.getBody();
          if (!list_body.is_object() || !list_body.contains("documents") || !list_body["documents"].is_array())
          {
               return Response(502, makeErrorBody("Invalid response", "Server response missing 'documents' array"));
          }

          const nlohmann::json& documents = list_body["documents"];
          if (documents.empty())
          {
               break;
          }

          nlohmann::json import_payload;
          import_payload["documents"] = documents;

          std::string import_path = "/collections/" + utils::urlEncode(target_name) + "/documents/import";
          Response import_response = request_->execute("POST", import_path, import_payload);

          if (import_response.getStatusCode() == 200 || import_response.getStatusCode() == 201)
          {
               copied_count += static_cast<int>(documents.size());
               offset += static_cast<int>(documents.size());
               continue;
          }

          if (import_response.getStatusCode() == 404)
          {
               int imported = 0;
               for (const auto& doc : documents)
               {
                    if (!doc.is_object())
                    {
                         continue;
                    }

                    std::string add_path = "/collections/" + utils::urlEncode(target_name) + "/documents";
                    Response single_response = request_->execute("POST", add_path, doc);
                    if (single_response.getStatusCode() != 201)
                    {
                         return single_response;
                    }

                    imported++;
               }

               copied_count += imported;
               offset += static_cast<int>(documents.size());
               continue;
          }

          return import_response;
     }

     nlohmann::json result;
     result["source"] = source_name;
     result["target"] = target_name;
     result["copied_documents"] = copied_count;
     return Response(200, result);
}

}
