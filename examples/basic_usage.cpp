/*
 * Basic Usage Example
 *
 * Demonstrates basic client initialization and simple operations
 */

#include <iomanip>
#include <iostream>

#include "hlquery/client.h"

int main()
{
     try
     {
          /* Initialize client */

          hlquery::Client client("http://localhost:9200");

          /* Health check */

          auto health = client.health();
          std::cout << "Health: " << health.getStatusCode() << std::endl;

          /* List collections */

          auto collections = client.listCollections(0, 10);
          if (collections.isSuccess())
          {
               auto body = collections.getBody();
               if (body.contains("collections") && body["collections"].is_array())
               {
                    std::cout << "Found " << body["collections"].size() << " collections" << std::endl;
               }
          }
     }
     catch (const std::exception& e)
     {
          std::cerr << "Error: " << e.what() << std::endl;
          return 1;
     }

     return 0;
}
