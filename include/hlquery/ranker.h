/*
 * hlquery C++ Client - Ranker Utilities
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
#include <string>

namespace hlquery
{
class Ranker
{
   public:

     static double ComputeRankSignal(double popularity,
                                     double hitLog,
                                     const std::map<std::string, double>* overrides = nullptr);

     static void AttachRankSort(std::map<std::string, std::string>& params,
                                const std::string& field = "rank_signal",
                                const std::string& direction = "desc");
};
}
