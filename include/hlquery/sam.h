/*
 * hlquery C++ Client - SAM API
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

class Sam
{
   public:

     explicit Sam(std::shared_ptr<Request> request);

     Response search(const std::string& collection_name, const std::string& query,
                     const std::map<std::string, std::string>& params = {});
     Response status(const std::string& collection_name = "",
                     const std::map<std::string, std::string>& params = {});
     Response history(const std::string& collection_name = "", int limit = 100,
                      const std::map<std::string, std::string>& params = {});

   private:

     std::shared_ptr<Request> request_;
};

}
