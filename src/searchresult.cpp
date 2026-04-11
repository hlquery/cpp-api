/*
 * hlquery C++ Client - Search Result API Implementation
 * https://www.hlquery.com
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 * You are free to redistribute and/or modify this software
 * under the terms of the BSD License.
 * For more details, please visit: https://docs.hlquery.com
 */

#include "hlquery/searchresult.h"

namespace hlquery
{

/* Hit Implementation */

Hit::Hit(const nlohmann::json& hit_json)
{
     if (hit_json.contains("document"))
     {
          document_ = hit_json["document"];
     }

     text_match_ = hit_json.value("text_match", 0.0f);
     vector_score_ = hit_json.value("vector_score", 0.0f);
     hybrid_score_ = hit_json.value("hybrid_score", 0.0f);

     if (hit_json.contains("highlights") && hit_json["highlights"].is_object())
     {
          for (auto it = hit_json["highlights"].begin(); it != hit_json["highlights"].end(); ++it)
          {
               highlights_[it.key()] = it.value().get<std::string>();
          }
     }
}

nlohmann::json Hit::getDocument() const
{
     return document_;
}

float Hit::getScore() const
{
     if (hybrid_score_ > 0.0f)
          return hybrid_score_;
     if (vector_score_ > 0.0f)
          return vector_score_;
     return text_match_;
}

std::map<std::string, std::string> Hit::getHighlights() const
{
     return highlights_;
}

/* SearchResult Implementation */

SearchResult::SearchResult(const Response& response) : SearchResult(response.getBody())
{
}

SearchResult::SearchResult(const nlohmann::json& response_json)
{
     if (response_json.contains("hits") && response_json["hits"].is_array())
     {
          for (const auto& hit_json : response_json["hits"])
          {
               hits_.emplace_back(hit_json);
          }
     }

     found_ = response_json.value("found", 0);
     out_of_ = response_json.value("out_of", 0);
     page_ = response_json.value("page", 1);
     per_page_ = response_json.value("per_page", 10);
     search_time_ms_ = response_json.value("search_time_ms", 0.0f);

     if (response_json.contains("facets"))
     {
          facets_ = response_json["facets"];
     }
}

const std::vector<Hit>& SearchResult::getHits() const
{
     return hits_;
}

int SearchResult::getFoundCount() const
{
     return found_;
}

float SearchResult::getSearchTime() const
{
     return search_time_ms_;
}

nlohmann::json SearchResult::getFacets() const
{
     return facets_;
}

bool SearchResult::hasNextPage() const
{
     if (per_page_ <= 0)
          return false;
     return (page_ * per_page_) < found_;
}

int SearchResult::getTotalPages() const
{
     if (per_page_ <= 0)
          return 0;
     return (found_ + per_page_ - 1) / per_page_;
}

int SearchResult::getPage() const
{
     return page_;
}

int SearchResult::getPerPage() const
{
     return per_page_;
}

/* MultiSearchResult Implementation */

MultiSearchResult::MultiSearchResult(const Response& response)
{
     nlohmann::json body = response.getBody();
     if (body.contains("results") && body["results"].is_array())
     {
          for (const auto& result_json : body["results"])
          {
               results_.emplace_back(result_json);
          }
     }
}

const std::vector<SearchResult>& MultiSearchResult::getResults() const
{
     return results_;
}

}
