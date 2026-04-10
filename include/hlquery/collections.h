/*
 * hlquery C++ Client - Collections API
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 */

#pragma once

#include <memory>
#include <string>

#include "request.h"
#include "response.h"

namespace hlquery
{

class Collections
{
   public:

     explicit Collections(std::shared_ptr<Request> request);

     Response list(int offset = 0, int limit = 10);
     Response get(const std::string& name);
     Response create(const std::string& name, const nlohmann::json& schema);
     Response remove(const std::string& name);
     Response update(const std::string& name, const nlohmann::json& schema);
     Response getFields(const std::string& name);

   private:

     std::shared_ptr<Request> request_;
};

}
