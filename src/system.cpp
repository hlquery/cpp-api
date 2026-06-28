/*
 * hlquery C++ Client - System API Implementation
 * https://www.hlquery.com
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 * You are free to redistribute and/or modify this software
 * under the terms of the BSD License.
 * For more details, please visit: https://docs.hlquery.com
 */

#include <stdexcept>

#include "hlquery/system.h"

namespace hlquery
{

System::System(std::shared_ptr<Request> request) : request_(request)
{
}

Response System::health()
{
     return request_->execute("GET", "/health");
}

Response System::ready()
{
     return request_->execute("GET", "/ready");
}

Response System::status()
{
     return request_->execute("GET", "/status");
}

Response System::query()
{
     return request_->execute("GET", "/query");
}

Response System::startup()
{
     return request_->execute("GET", "/startup");
}

Response System::bootStatus()
{
     return request_->execute("GET", "/boot-status");
}

Response System::info()
{
     return request_->execute("GET", "/");
}

Response System::stats()
{
     return request_->execute("GET", "/stats");
}

Response System::metrics()
{
     return request_->execute("GET", "/metrics");
}

Response System::metricsJson()
{
     return request_->execute("GET", "/metrics.json");
}

Response System::metricsHistory()
{
     return request_->execute("GET", "/metrics-history");
}

Response System::metricsHistoryAlias()
{
     return request_->execute("GET", "/metrics/history");
}

Response System::connections()
{
     return request_->execute("GET", "/connections");
}

Response System::rocksdb()
{
     return request_->execute("GET", "/rocksdb");
}

Response System::rocksdbInternal()
{
     return request_->execute("GET", "/_rocksdb");
}

Response System::docTotal()
{
     return request_->execute("GET", "/doctotal");
}

Response System::etc()
{
     return request_->execute("GET", "/etc");
}

Response System::searchConfig()
{
     return request_->execute("GET", "/search-config");
}

Response System::cache()
{
     return request_->execute("GET", "/cache");
}

Response System::updateCounters(const std::map<std::string, std::string>& params)
{
     return request_->execute("GET", "/update-counters", nullptr, params);
}

Response System::updateCountersPost(const nlohmann::json& body)
{
     return request_->execute("POST", "/update-counters", body);
}

Response System::debugCounters()
{
     return request_->execute("GET", "/debug/counters");
}

Response System::repair(const std::map<std::string, std::string>& params)
{
     return request_->execute("GET", "/repair", nullptr, params);
}

Response System::repairPost(const nlohmann::json& body)
{
     return request_->execute("POST", "/repair", body);
}

Response System::sql(const std::string& sql, const std::map<std::string, std::string>& params)
{
     if (sql.empty())
     {
          throw std::invalid_argument("SQL query must be a non-empty string");
     }

     std::map<std::string, std::string> query_params = params;
     query_params["sql"] = sql;
     return request_->execute("GET", "/sql", nullptr, query_params);
}

Response System::execSql(const std::string& sql)
{
     if (sql.empty())
     {
          throw std::invalid_argument("SQL query must be a non-empty string");
     }

     nlohmann::json body;
     body["exec"] = sql;
     return request_->execute("POST", "/sql", body);
}

Response System::ping()
{
     return request_->execute("GET", "/ping");
}

Response System::integrity()
{
     return request_->execute("GET", "/integrity");
}

Response System::consistency()
{
     return request_->execute("GET", "/consistency");
}

Response System::selfCheck()
{
     return request_->execute("GET", "/self-check");
}

Response System::storageStatus()
{
     return request_->execute("GET", "/admin/storage_status");
}

}
