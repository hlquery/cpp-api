<div align="center">
  <img src="https://docs.hlquery.com/img/hlquery/2.png" alt="hlquery logo" width="200">
</div>


# hlquery C++ API Client

A C++ client library for hlquery, designed with a familiar and intuitive API structure.

## Features

-  **Modular Architecture**: Clean separation of concerns with organized classes
-  **Intuitive API**: Familiar and easy-to-use structure
-  **Authentication Support**: Bearer token and X-API-Key authentication
-  **HTTPS Support**: Optional OpenSSL support for secure connections
-  **Type-safe Responses**: Response objects with helper methods
-  **Comprehensive Validation**: Input validation for all operations
-  **Minimal Dependencies**: Uses nlohmann/json (included) and standard C++ libraries

## Requirements

- C++17 or later
- nlohmann/json (included in `vendor/json/json.hpp`)
- OpenSSL (optional, for HTTPS support)

## Building

### Using Make

```bash
cd etc/api/cpp
make
```

This will build:
- `build/libhlqueryclient.a` - Static library
- `build/basic_usage` - Example executable

All build artifacts are placed in the `build/` directory.

To clean build artifacts:
```bash
make clean
```

This removes the entire `build/` directory.

## Quick Start

```cpp
#include "hlquery/client.h"
#include <iostream>

int main() {
    try {
        // Initialize client
        hlquery::Client client("http://localhost:9200");
        
        // Health check
        auto health = client.health();
        std::cout << "Status: " << health.getStatusCode() << std::endl;
        
        // List collections
        auto collections = client.listCollections(0, 10);
        if (collections.isSuccess()) {
            auto body = collections.getBody();
            // Process collections...
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

## Directory Structure

```
cpp/
├── include/
│   └── hlquery/
│       ├── client.h
│       ├── request.h
│       ├── response.h
│       ├── exceptions.h
│       ├── collections.h
│       ├── documents.h
│       ├── search.h
│       └── utils/
│           ├── Config.h
│           ├── Validator.h
│           └── Url.h
├── src/
│   ├── client.cpp
│   ├── request.cpp
│   ├── response.cpp
│   ├── collections.cpp
│   ├── documents.cpp
│   ├── search.cpp
│   └── utils/
│       ├── Config.cpp
│       ├── Validator.cpp
│       └── Url.cpp
├── examples/
│   └── basic_usage.cpp
├── build/
│   ├── libhlqueryclient.a
│   ├── basic_usage
│   └── obj/
├── Makefile
└── README.md
```

## API Usage

### Client Initialization

```cpp
// Basic initialization
hlquery::Client client("http://localhost:9200");

// With options
std::map<std::string, std::string> options;
options["timeout"] = "60";
options["token"] = "your_token";
options["auth_method"] = "bearer";
options["tls_verify"] = "true";
hlquery::Client client("http://localhost:9200", options);
```

### TLS Verification

When OpenSSL is available and HTTPS is used, the client verifies TLS certificates
and the hostname by default. For local/dev environments where this is not
possible, you can explicitly opt out:

```cpp
std::map<std::string, std::string> options;
options["tls_verify"] = "false"; // intentionally unsafe
hlquery::Client client("https://localhost:9200", options);
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

### Collections API

```cpp
// List collections
auto collections = client.listCollections(0, 10);

// Get collection
auto collection = client.getCollection("my_collection");

// Get collection fields
auto fields = client.getCollectionFields("my_collection");

// Using Collections object directly
auto collections_api = client.collections();
auto result = collections_api->create("new_collection", schema);
```

### Documents API

```cpp
// List documents
std::map<std::string, std::string> params;
params["offset"] = "0";
params["limit"] = "10";
auto documents = client.listDocuments("collection", params);

// Get document
auto document = client.getDocument("collection", "doc_id");

// Add document
nlohmann::json doc;
doc["id"] = "doc_1";
doc["title"] = "Test";
auto result = client.documents()->add("collection", doc);
```

### Search API

```cpp
// Simple search
std::map<std::string, std::string> params;
params["q"] = "test";
params["query_by"] = "title,content";
params["limit"] = "10";
auto results = client.search("collection", params);

// Supported query semantics
// Field-specific search
params["q"] = "title:laptop";
auto field_results = client.search("collection", params);

// Boolean OR query
params["q"] = "title:laptop OR title:notebook";
auto or_results = client.search("collection", params);

// Wildcard search
params["q"] = "laptop*";
auto wildcard_results = client.search("collection", params);

// NOT query
params["q"] = "title:laptop NOT title:refurbished";
auto not_results = client.search("collection", params);

// Phrase query
params["q"] = "\"wireless keyboard\"";
params["query_by"] = "title";
auto phrase_results = client.search("collection", params);

// Filter operators belong in filter_by
params["q"] = "*";
params["query_by"] = "title,content";
params["filter_by"] = "price:>100&&category:electronics";
auto filtered_results = client.search("collection", params);

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

### Ranking helpers

`hlquery::Ranker` exposes helpers to recompute `rank_signal` and attach a `sort_by` clause so you can boost by popularity/hit metrics.

```cpp
std::map<std::string, std::string> params;
params["q"] = "guide";
double signal = Ranker::ComputeRankSignal(popularity, hit_log);
params["rank_signal"] = std::to_string(signal);
Ranker::AttachRankSort(params);
auto results = client.search("collection", params);
```

## Response Handling

All methods return a `Response` object:

```cpp
auto response = client.health();

// Check status
if (response.isSuccess()) {
    auto body = response.getBody();
    // Process body...
}

// Get error
if (response.isError()) {
    std::string error = response.getError();
    std::cerr << "Error: " << error << std::endl;
}

// Access JSON body
nlohmann::json body = response.getBody();
```

## Error Handling

The client throws exceptions for errors:

```cpp
try {
    auto result = client.search("collection", params);
} catch (const hlquery::RequestException& e) {
    std::cerr << "Request failed: " << e.what() << std::endl;
    std::cerr << "Status: " << e.getStatusCode() << std::endl;
} catch (const hlquery::AuthenticationException& e) {
    std::cerr << "Auth failed: " << e.what() << std::endl;
} catch (const hlquery::ValidationException& e) {
    std::cerr << "Validation failed: " << e.what() << std::endl;
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

## License

Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>

This software is licensed under the BSD-3-Clause License. See the LICENSE file for details.
