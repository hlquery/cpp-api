#include "hlquery/client.h"

#include <iostream>

int main()
{
     try
     {
          hlquery::Client client("http://localhost:9200");
          const std::string collection_name = "cpp_sql_demo";

          client.collections()->remove(collection_name);

          nlohmann::json schema = {
              {"fields",
               {
                   {{"name", "title"}, {"type", "string"}},
                   {{"name", "category"}, {"type", "string"}},
                   {{"name", "price"}, {"type", "int32"}},
               }},
          };

          auto create = client.collections()->create(collection_name, schema);
          std::cout << "Create collection: " << create.getStatusCode() << std::endl;

          client.documents()->add(collection_name, {
                                                   {"id", "sku_1"},
                                                   {"title", "Trail Running Shoes"},
                                                   {"category", "footwear"},
                                                   {"price", 129},
                                               });

          client.documents()->add(collection_name, {
                                                   {"id", "sku_2"},
                                                   {"title", "Waterproof Jacket"},
                                                   {"category", "outerwear"},
                                                   {"price", 189},
                                               });

          auto select = client.collections()->sql(
              collection_name,
              "SELECT id, title, price FROM " + collection_name + " ORDER BY price DESC LIMIT 5;");
          std::cout << "Collection SQL SELECT:" << std::endl;
          std::cout << select.getBody().dump(2) << std::endl;

          auto show_collections = client.sql("SHOW COLLECTIONS;");
          std::cout << "SHOW COLLECTIONS:" << std::endl;
          std::cout << show_collections.getBody().dump(2) << std::endl;

          auto cleanup = client.collections()->remove(collection_name);
          std::cout << "Cleanup: " << cleanup.getStatusCode() << std::endl;
     }
     catch (const std::exception& e)
     {
          std::cerr << "Error: " << e.what() << std::endl;
          return 1;
     }

     return 0;
}
