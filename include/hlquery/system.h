/*
 * hlquery C++ Client - System API
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

class System
{
   public:

     explicit System(std::shared_ptr<Request> request);

     Response health();
     Response status();
     Response startup();
     Response bootStatus();
     Response info();
     Response stats();
     Response metrics();
     Response metricsJson();
     Response connections();
     Response rocksdb();
     Response rocksdbInternal();
     Response docTotal();
     Response etc();
     Response sql(const std::string& sql, const std::map<std::string, std::string>& params = {});
     Response execSql(const std::string& sql);
     Response ping();
     Response integrity();
     Response consistency();
     Response selfCheck();
     Response storageStatus();

   private:

     std::shared_ptr<Request> request_;
};

}
