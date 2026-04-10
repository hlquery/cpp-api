/*
 * hlquery C++ Client - Search Result API
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 */

#pragma once

#include <json.hpp>
#include <map>
#include <string>
#include <vector>

#include "response.h"

namespace hlquery
{

/**
 * @brief Represents a single search hit.
 */
class Hit
{
   public:

     Hit(const nlohmann::json& hit_json);

     /**
     * @brief Returns the document data as JSON.
     */
     nlohmann::json getDocument() const;

     /**
     * @brief Returns the match score.
     */
     float getScore() const;

     /**
     * @brief Returns Snippet highlights for the hit.
     */
     std::map<std::string, std::string> getHighlights() const;

   private:

     nlohmann::json document_;
     float text_match_;
     float vector_score_;
     float hybrid_score_;
     std::map<std::string, std::string> highlights_;
};

/**
 * @brief Represents a comprehensive search result.
 */
class SearchResult
{
   public:

     SearchResult() = default;
     SearchResult(const Response& response);
     SearchResult(const nlohmann::json& response_json);

     /**
     * @brief Returns the list of hits.
     */
     const std::vector<Hit>& getHits() const;

     /**
     * @brief Returns the total number of documents matching the query.
     */
     int getFoundCount() const;

     /**
     * @brief Returns the processing time in milliseconds.
     */
     float getSearchTime() const;

     /**
     * @brief Returns calculated facet counts.
     */
     nlohmann::json getFacets() const;

     /**
     * @brief Checks if there is a next page of results.
     */
     bool hasNextPage() const;

     /**
     * @brief Returns the total number of pages.
     */
     int getTotalPages() const;

     /**
     * @brief Returns the current page number.
     */
     int getPage() const;

     /**
     * @brief Returns the number of results per page.
     */
     int getPerPage() const;

   private:

     std::vector<Hit> hits_;
     int found_ = 0;
     int out_of_ = 0;
     int page_ = 1;
     int per_page_ = 10;
     float search_time_ms_ = 0.0f;
     nlohmann::json facets_;
};

/**
 * @brief Represents results from a multi-search operation.
 */
class MultiSearchResult
{
   public:

     MultiSearchResult(const Response& response);

     /**
     * @brief Returns the list of search results.
     */
     const std::vector<SearchResult>& getResults() const;

   private:

     std::vector<SearchResult> results_;
};

}
