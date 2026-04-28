<div align="center">
  <img src="https://docs.hlquery.com/img/hlquery/2.png" alt="hlquery logo" width="200">
</div>

<div align="center">

**A C++ API for a high-performance search engine built for modern applications.**

[![Follow hlquery](https://img.shields.io/badge/Follow-%40hlquery-blue?logo=x&logoColor=white)](https://x.com/hlquery)
[![Linux Build](https://github.com/hlquery/cpp-api/workflows/Linux%20build/badge.svg)](https://github.com/hlquery/cpp-api/actions)
[![macOS Build](https://github.com/hlquery/cpp-api/workflows/macOS%20Build/badge.svg)](https://github.com/hlquery/cpp-api/actions)
[![Commit Activity](https://img.shields.io/github/commit-activity/m/hlquery/cpp-api)](https://github.com/hlquery/cpp-api/pulse)
[![cpp-api](https://img.shields.io/badge/GitHub-cpp--api-181717?logo=github&logoColor=white)](https://github.com/hlquery/cpp-api/stargazers)
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

</div>


### hlquery C++ API Client

A C++ client library for hlquery with modular APIs, authentication support, HTTPS support, and type-safe responses.

### Features

-  **Modular Architecture**: Clean separation of concerns with organized classes
-  **Intuitive API**: Familiar and easy-to-use structure
-  **Authentication Support**: Bearer token and X-API-Key authentication
-  **HTTPS Support**: Optional OpenSSL support for secure connections
-  **SQL Support**: Collection-bound SQL selects and top-level `/sql` execution
-  **Type-safe Responses**: Response objects with helper methods
-  **Comprehensive Validation**: Input validation for all operations
-  **Minimal Dependencies**: Uses nlohmann/json (included) and standard C++ libraries

### Requirements

- C++17 or later
- nlohmann/json (included in `vendor/json/json.hpp`)
- OpenSSL (optional, for HTTPS support)

### Installation

Build the client and examples locally:

```bash
$ cd cpp-api
$ make
```

On FreeBSD, use GNU make:

```bash
$ cd cpp-api
$ gmake
```

Build modes:

```bash
# Auto-detect OpenSSL with pkg-config (default)
$ make

# Force HTTP-only build with no OpenSSL dependency
$ make OPENSSL=0

# Require OpenSSL and fail fast if it is not available
$ make OPENSSL=1
```

### Using Make

```bash
$ cd cpp-api
$ make
```

FreeBSD:

```bash
$ cd cpp-api
$ gmake
```

This will build:
- `build/libhlqueryclient.a` - Static library
- `build/basic_usage` - Example executable

All build artifacts are placed in the `build/` directory.

To clean build artifacts:
```bash
$ make clean
```

On FreeBSD:

```bash
$ gmake clean
```

This removes the entire `build/` directory.

On systems where OpenSSL is installed outside the default compiler include path
(for example Homebrew on macOS), the Makefile now pulls both compiler and linker
flags from `pkg-config`.

### Quick Start

```cpp
#include "hlquery/client.h"
#include <iostream>

int main() {
    try {
        // Initialize client
        hlquery::Client client("http://localhost:9200");

        auto collections = client.collections();
        auto documents = client.documents();
        auto search = client.searchApi();
        
        // Health check
        auto health = client.health();
        std::cout << "Status: " << health.getStatusCode() << std::endl;
        
        // List collections
        auto list = collections->list(0, 10);
        if (list.isSuccess()) {
            auto body = list.getBody();
            // Process collections...
        }

        auto results = search->search("products", {
            {"like", "laptop"},
            {"query_by", "title,content"},
            {"limit", "10"}
        });
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

### Authentication

```cpp
// Set token
client.setAuthToken("your_token_here", "bearer");

// Or use API key
client.setAuthToken("your_api_key", "api-key");

// Clear authentication
client.clearAuth();
```

### Search API

```cpp
auto search = client.searchApi();

// Simple search. "like" is accepted as an alias for "q".
std::map<std::string, std::string> params;
params["like"] = "test";
params["query_by"] = "title,content";
params["limit"] = "10";
auto results = search->search("collection", params);

// Supported query semantics
// Field-specific search
params["like"] = "title:laptop";
auto field_results = search->search("collection", params);

// Boolean OR query
params["like"] = "title:laptop OR title:notebook";
auto or_results = search->search("collection", params);

// Wildcard search
params["like"] = "laptop*";
auto wildcard_results = search->search("collection", params);

// NOT query
params["like"] = "title:laptop NOT title:refurbished";
auto not_results = search->search("collection", params);

// Phrase query
params["like"] = "\"wireless keyboard\"";
params["query_by"] = "title";
auto phrase_results = search->search("collection", params);

// Filter operators belong in filter_by
params["like"] = "*";
params["query_by"] = "title,content";
params["filter_by"] = "price:>100&&category:electronics";
auto filtered_results = search->search("collection", params);

// SQL search
auto sql_results = search->sql(
    "collection",
    "SELECT id, title, price FROM collection ORDER BY price DESC LIMIT 5;"
);
std::cout << sql_results.getBody().dump(2) << std::endl;

// Top-level SQL
auto rows = client.sql("SHOW COLLECTIONS;");
auto insert = client.execSql(
    "INSERT INTO collection (id, title, price) VALUES ('sku-9', 'Camp Stove', 89);"
);

// Vector search
std::map<std::string, std::string> vector_params;
vector_params["vector_query"] = "[0.1,0.2,0.3]";
vector_params["limit"] = "5";
auto results = client.vectorSearch("collection", vector_params);

// Advanced vector search (POST JSON body)
nlohmann::json vector_body = {
    {"vector", {0.1, 0.2, 0.3}},
    {"field_name", "embedding"},
    {"topk", 5},
    {"include_distance", true},
    {"query_params", {{"ef", 64}, {"nprobe", 4}, {"is_linear", true}}},
    {"radius", 1.0}
};
auto advanced = client.executeRequest("POST", "/collections/collection/vector_search", vector_body);
```

### Collections And Documents

```cpp
auto collections = client.collections();
auto documents = client.documents();

nlohmann::json schema = {
    {"fields", nlohmann::json::array({
        {{"name", "title"}, {"type", "string"}},
        {{"name", "content"}, {"type", "string"}},
        {{"name", "price"}, {"type", "float"}}
    })}
};

auto created = collections->create("products", schema);
auto updated = collections->update("products", {
    {"add_fields", nlohmann::json::array({
        {{"name", "brand"}, {"type", "string"}}
    })}
});

auto added = documents->add("products", {
    {"id", "sku-1"},
    {"title", "Laptop Computer"},
    {"content", "High-performance laptop with 16GB RAM"},
    {"price", 1299.0}
});

auto changed = documents->update("products", "sku-1", {
    {"price", 1199.0},
    {"brand", "hlquery"}
});

auto removed = documents->remove("products", "sku-1");
```

### SQL

Quick SQL example:

```cpp
#include "hlquery/client.h"
#include <iostream>

int main()
{
    hlquery::Client client("http://localhost:9200");

    auto response = client.sqlSearch(
        "products",
        "SELECT id, title, price FROM products ORDER BY price DESC LIMIT 5;"
    );

    if (!response.isSuccess())
    {
        std::cerr << response.getError() << std::endl;
        return 1;
    }

    std::cout << response.getBody().dump(2) << std::endl;
    return 0;
}
```

Basic SQL example:

```cpp
hlquery::Client client("http://localhost:9200");

auto response = client.sqlSearch(
    "products",
    "SELECT id, title, price FROM products ORDER BY price DESC LIMIT 5;"
);

if (response.isSuccess())
{
    std::cout << response.getBody().dump(2) << std::endl;
}
```

Top-level SQL execution:

```cpp
auto rows = client.sql("SHOW COLLECTIONS;");

auto insert = client.execSql(
    "INSERT INTO products (id, title, price) VALUES ('sku-9', 'Camp Stove', 89);"
);
```
