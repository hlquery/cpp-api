/*
 * Hits API Usage Example
 *
 * Demonstrates SearchResult and Hit helpers on top of the collection search API.
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
     stream << "cpp_hits_demo_" << std::time(nullptr);
     return stream.str();
}

}

int main()
{
     const std::string collection_name = makeCollectionName();

     try
     {
          hlquery::Client client("http://localhost:9200");
          auto collections = client.collections();
          auto documents = client.documents();

          nlohmann::json schema = {
               {"fields", nlohmann::json::array({
                    {{"name", "title"}, {"type", "string"}},
                    {{"name", "description"}, {"type", "string"}},
                    {{"name", "category"}, {"type", "string"}},
                    {{"name", "price"}, {"type", "int"}}
               })}
          };

          auto create = collections->create(collection_name, schema);
          if (!create.isSuccess())
          {
               std::cerr << "Create collection failed: " << create.getBody().dump(2) << std::endl;
               return 1;
          }

          documents->add(collection_name, {
               {"id", "sku_1"},
               {"title", "Laptop stand"},
               {"description", "Aluminum stand for a laptop"},
               {"category", "office"},
               {"price", 49}
          });

          documents->add(collection_name, {
               {"id", "sku_2"},
               {"title", "Wireless keyboard"},
               {"description", "Compact keyboard for desk setups"},
               {"category", "office"},
               {"price", 79}
          });

          std::map<std::string, std::string> params = {
               {"q", "laptop"},
               {"query_by", "title,description"},
               {"per_page", "5"}
          };

          auto search_result = collections->searchStructured(collection_name, params);

          std::cout << "Search found " << search_result.getFoundCount() << " results" << std::endl;
          std::cout << "Search time: " << search_result.getSearchTime() << "ms" << std::endl;

          for (const auto& hit : search_result.getHits())
          {
               std::cout << "---" << std::endl;
               std::cout << "Score: " << hit.getScore() << std::endl;

               auto doc = hit.getDocument();
               std::cout << "ID: " << doc.value("id", "") << std::endl;
               std::cout << "Title: " << doc.value("title", "") << std::endl;
               std::cout << "Category: " << doc.value("category", "") << std::endl;

               auto highlights = hit.getHighlights();
               if (!highlights.empty())
               {
                    std::cout << "Highlights:" << std::endl;
                    for (const auto& entry : highlights)
                    {
                         std::cout << "  " << entry.first << ": " << entry.second << std::endl;
                    }
               }
          }

          collections->remove(collection_name);
     }
     catch (const std::exception& e)
     {
          std::cerr << "Error: " << e.what() << std::endl;
          return 1;
     }

     return 0;
}
