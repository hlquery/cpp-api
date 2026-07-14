/*
 * SQL Example
 *
 * Demonstrates collection-scoped SQL and global SQL helpers.
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
     stream << "cpp_sql_demo_" << std::time(nullptr);
     return stream.str();
}

void printResponse(const std::string& label, const hlquery::Response& response)
{
     std::cout << label << ": HTTP " << response.getStatusCode() << std::endl;
     std::cout << response.getBody().dump(2) << std::endl;
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
                    {{"name", "category"}, {"type", "string"}},
                    {{"name", "price"}, {"type", "int"}}
               })}
          };

          auto create = collections->create(collection_name, schema);
          printResponse("Create collection", create);
          if (!create.isSuccess())
          {
               return 1;
          }

          documents->add(collection_name, {
               {"id", "sku_1"},
               {"title", "Trail running shoes"},
               {"category", "footwear"},
               {"price", 129}
          });

          documents->add(collection_name, {
               {"id", "sku_2"},
               {"title", "Waterproof jacket"},
               {"category", "outerwear"},
               {"price", 189}
          });

          auto collection_select = client.sql(
               collection_name,
               "SELECT id, title, price FROM " + collection_name + " ORDER BY price DESC LIMIT 5;"
          );
          printResponse("Collection SQL SELECT", collection_select);

          auto show_collections = client.system()->sql("SHOW COLLECTIONS;");
          printResponse("Global SQL SHOW COLLECTIONS", show_collections);

          printResponse("Cleanup", collections->remove(collection_name));
     }
     catch (const std::exception& e)
     {
          std::cerr << "Error: " << e.what() << std::endl;
          return 1;
     }

     return 0;
}
