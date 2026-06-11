<div align="center">
  <img src="https://docs.hlquery.com/img/hlquery/2.png" alt="hlquery logo" width="200">
</div>

<div align="center">

**A modular C++ client library for hlquery, designed with a familiar and intuitive API structure.**

[![Follow hlquery](https://img.shields.io/badge/Follow-%40hlquery-blue?logo=x&logoColor=white&labelColor=000000)](https://x.com/hlquery)
[![cpp-api](https://img.shields.io/badge/GitHub-cpp--api-purple?logo=github&logoColor=white&labelColor=000000)](https://github.com/hlquery/cpp-api/)
[![GitHub](https://img.shields.io/badge/GitHub-hlquery-blue?logo=github&logoColor=white&labelColor=000000)](https://github.com/hlquery/hlquery/)
[![License](https://img.shields.io/badge/License-BSD%203--Clause-a35a0f?logo=open-source-initiative&logoColor=white&labelColor=000000)](https://opensource.org/licenses/BSD-3-Clause)

</div>

### What is the hlquery C++ API?

The hlquery C++ API is the official C++ client for [hlquery](https://github.com/hlquery/hlquery). It wraps the server's HTTP/JSON interface in a small typed client with response helpers, auth support, SQL helpers, and system helpers.

It is intended for native services, command-line tools, and applications that want direct hlquery access without hand-rolling HTTP calls.

### Why use it?

Use the C++ client when your application already lives close to hlquery and you want the search layer to feel native instead of like a pile of hand-built HTTP calls. The library keeps request setup, auth, response parsing, and endpoint routing in one place, so application code can work with collections, documents, search, SQL, and system routes through one small client surface.

It is still close to the server API. You get typed helpers for the common paths, but the raw request helper remains available for custom module routes or newer endpoints that have not yet grown a dedicated wrapper. That makes it useful for production services that want a stable integration point without losing access to hlquery's full HTTP surface.

### Install

Build locally:

```bash
$ make
```

On FreeBSD, use:

```bash
$ gmake
```

Build modes:

```bash
$ make OPENSSL=0
$ make OPENSSL=1
```

Artifacts are written to `build/`.

### Quick Start

```cpp
#include "hlquery/client.h"
#include <iostream>

int main() {
    try {
        hlquery::Client client("http://localhost:9200");

        auto health = client.system()->health();
        std::cout << "Status: " << health.getStatusCode() << std::endl;

        auto collections = client.collections();
        auto list = collections->list(0, 10);

        if (list.isSuccess()) {
            std::cout << list.getBody().dump(2) << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

### Auth

```cpp
hlquery::Client client("http://localhost:9200");

client.setAuthToken("your_token_here", "bearer");
client.setAuthToken("your_api_key_here", "api-key");
client.clearAuth();
```

### System

Use the system helper for operational routes that were added after the initial C++ client surface:

```cpp
hlquery::Client client("http://localhost:9200");

auto system = client.system();
auto status = system->status();
auto metrics = system->metricsJson();
auto storage = system->storageStatus();

std::cout << status.getBody().dump(2) << std::endl;
std::cout << metrics.getBody().dump(2) << std::endl;
std::cout << storage.getBody().dump(2) << std::endl;
```

### SQL

```cpp
hlquery::Client client("http://localhost:9200");

auto rows = client.system()->sql("SHOW COLLECTIONS;");
auto products = client.sql(
    "products",
    "SELECT id, title, price FROM products ORDER BY price DESC LIMIT 3;"
);
```

### Copy Collections

Copy a collection (schema + all documents) into a new collection name:

```cpp
hlquery::Client client("http://localhost:9200");
auto result = client.copyCollection("source_collection", "target_collection");
std::cout << result.getBody().dump(2) << std::endl;
```

### Reduce Text Example

Use the raw request helper for custom module routes:

```cpp
hlquery::Client client("http://localhost:9200");

auto response = client.executeRequest(
    "GET",
    "/modules/<name>/<route>",
    "",
    {
        {"q", "example query"}
    }
);
```
