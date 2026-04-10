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
