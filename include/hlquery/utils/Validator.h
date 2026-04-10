/*
 * hlquery C++ Client - Input Validation
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 */

#pragma once

#include <map>
#include <string>

#include "hlquery/exceptions.h"

namespace hlquery
{
namespace utils
{

void validateCollectionName(const std::string& name);
void validateDocumentId(const std::string& doc_id);
void validatePagination(int offset, int limit);
void validateSearchParams(const std::map<std::string, std::string>& params);

}
}
