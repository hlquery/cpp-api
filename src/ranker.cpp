/*
 * hlquery C++ Client - Ranker Utilities Implementation
 * https://www.hlquery.com
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 * You are free to redistribute and/or modify this software
 * under the terms of the BSD License.
 * For more details, please visit: https://docs.hlquery.com
 */

#include <cmath>

#include "hlquery/ranker.h"

namespace hlquery
{
static const std::map<std::string, double> DEFAULT_WEIGHTS = {
     {"popularity_log", 1.15},
     {"hit_log", 0.95},
     {"popularity_sqrt", 0.25},
     {"hit_log_sqrt", 0.15},
};

double Ranker::ComputeRankSignal(double popularity,
                                 double hitLog,
                                 const std::map<std::string, double>* overrides)
{
     std::map<std::string, double> weights = DEFAULT_WEIGHTS;

     if (overrides != nullptr)
     {
          for (const auto& [key, value] : *overrides)
          {
               if (weights.count(key) > 0)
               {
                    weights[key] = value;
               }
          }
     }

     return std::log(popularity + 1.0) * weights["popularity_log"] + std::log(hitLog + 1.0) * weights["hit_log"] + std::sqrt(popularity) * weights["popularity_sqrt"] + std::sqrt(hitLog) * weights["hit_log_sqrt"];
}

void Ranker::AttachRankSort(std::map<std::string, std::string>& params,
                            const std::string& field,
                            const std::string& direction)
{
     if (params.empty())
     {
          params["sort_by"] = field + ":" + direction;
          return;
     }

     std::string dir = "desc";
     if (direction == "asc")
     {
          dir = "asc";
     }

     std::string sort_instruction = field + ":" + dir;

     auto it = params.find("sort_by");
     if (it != params.end() && !it->second.empty())
     {
          it->second += "," + sort_instruction;
     }
     else
     {
          params["sort_by"] = sort_instruction;
     }
}
}
