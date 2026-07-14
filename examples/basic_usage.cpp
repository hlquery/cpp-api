/*
 * Basic Usage Example
 *
 * Demonstrates the preferred service-object API:
 * - client.system()
 * - client.collections()
 * - client.documents()
 */

#include "hlquery/client.h"

#include <ctime>
#include <iostream>
#include <sstream>

namespace
{

std::string makeCollectionName()
{
     std::ostringstream stream;
     stream << "cpp_basic_demo_" << std::time(nullptr);
     return stream.str();
}

void printResponse(const std::string& label, const hlquery::Response& response)
{
     std::cout << label << ": HTTP " << response.getStatusCode() << std::endl;
     if (!response.isSuccess())
     {
          std::cout << response.getBody().dump(2) << std::endl;
     }
}

}

int main()
{
     const std::string collection_name = makeCollectionName();

     try
     {
          hlquery::Client client("http://localhost:9200");

          auto system = client.system();
          auto collections = client.collections();
          auto documents = client.documents();

          printResponse("Health", system->health());

          nlohmann::json schema = {
               {"fields", nlohmann::json::array({
                    {{"name", "title"}, {"type", "string"}},
                    {{"name", "content"}, {"type", "string"}},
                    {{"name", "category"}, {"type", "string"}}
               })}
          };

          auto create = collections->create(collection_name, schema);
          printResponse("Create collection", create);
          if (!create.isSuccess())
          {
               return 1;
          }

          auto add = documents->add(collection_name, {
               {"id", "doc_1"},
               {"title", "C++ client guide"},
               {"content", "Using hlquery from a native service"},
               {"category", "docs"}
          });
          printResponse("Add document", add);
          if (!add.isSuccess())
          {
               collections->remove(collection_name);
               return 1;
          }

          std::map<std::string, std::string> params = {
               {"q", "client"},
               {"query_by", "title,content"},
               {"per_page", "5"}
          };

          auto search = collections->search(collection_name, params);
          printResponse("Search", search);
          if (search.isSuccess())
          {
               std::cout << search.getBody().dump(2) << std::endl;
          }

          printResponse("Cleanup", collections->remove(collection_name));
     }
     catch (const std::exception& e)
     {
          std::cerr << "Error: " << e.what() << std::endl;
          return 1;
     }

     return 0;
}
