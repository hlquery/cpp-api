<div align="center">
  <img src="https://docs.hlquery.com/img/hlquery/2.png" alt="hlquery logo" width="200">
</div>

<div align="center">

**A modular C++ client library for hlquery, designed with a familiar and intuitive API structure.**

[![Follow hlquery](https://img.shields.io/badge/Follow-%40hlquery-blue?logo=x&logoColor=white&labelColor=000000)](https://x.com/hlquery)
[![cpp-api](https://img.shields.io/badge/GitHub-cpp--api-purple?logo=github&logoColor=white&labelColor=000000)](https://github.com/hlquery/cpp-api/stargazers)
[![GitHub](https://img.shields.io/badge/GitHub-cpp--api-purple?logo=github&logoColor=white)](https://github.com/hlquery/cpp-api/stargazers)
[![License](https://img.shields.io/badge/License-BSD%203--Clause-a35a0f?logo=open-source-initiative&logoColor=white&labelColor=000000)](https://opensource.org/licenses/BSD-3-Clause)

</div>

### What is the hlquery C++ API?

The hlquery C++ API is the official C++ client for [hlquery](https://github.com/hlquery/hlquery). It wraps the server's HTTP/JSON interface in a small typed client with response helpers, auth support, SQL helpers, system helpers, and SAM support.

It is intended for native services, command-line tools, and applications that want direct hlquery access without hand-rolling HTTP calls.

### Why use it?

- Less request and parsing boilerplate than raw HTTP.
- One client entry point for collections, documents, search, SQL, system endpoints, and SAM.
- Optional HTTPS/OpenSSL support.
- Works well for native applications that want a small static client library.

### Why choose it over raw HTTP?

Choose the C++ client over raw HTTP when you want consistent request setup and auth handling, convenience wrappers for common hlquery endpoints, and type-safe response objects with predictable access patterns, while still keeping a raw request escape hatch for custom routes.

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

### SAM

Use the SAM helpers to inspect indexing status and run SAM search:

SAM is separate from vector search. It performs term and intent-style lookup, not vector similarity search.

```cpp
hlquery::Client client("http://localhost:9200");

auto sam = client.sam();
auto status = sam->status("music");
auto history = sam->history("music", 5);
auto results = sam->search("music", "queen of pop", {
    {"limit", "10"}
});

std::cout << status.getBody().dump(2) << std::endl;
std::cout << history.getBody().dump(2) << std::endl;
std::cout << results.getBody().dump(2) << std::endl;
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

### Contributing

We welcome contributions from the community! All contributions must be released under the BSD 3-Clause license.

### How to Contribute

- Check existing [issues](https://github.com/hlquery/hlquery/issues) or create new ones
- Contribute to client libraries (Node.js, Go, Java, Python, PHP, Ruby, Rust, Perl, C++)
- Test and report bugs
- Improve documentation

### Community

- 📖 [Documentation](https://docs.hlquery.com)
- 🐦 [X (Twitter)](https://x.com/hlquery)
- 📦 [GitHub](https://github.com/hlquery/hlquery)

### License

hlquery is licensed under the [BSD 3-Clause License](https://opensource.org/licenses/BSD-3-Clause).
