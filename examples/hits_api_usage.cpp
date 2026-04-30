/*
 * Hits API Usage Example
 *
 * Demonstrates the structured Hits API for search results.
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

          std::map<std::string, std::string> params = {
               {"like", "laptop"},
               {"query_by", "title,description"},
               {"per_page", "5"}};

          /* Use the structured Hits API through the collection handle */

          auto search_result = client.collections()->searchStructured("products", params);

          std::cout << "Search found " << search_result.getFoundCount() << " results" << std::endl;
          std::cout << "Search time: " << search_result.getSearchTime() << "ms" << std::endl;

          for (const auto& hit : search_result.getHits())
          {
               std::cout << "---" << std::endl;
               std::cout << "Score: " << hit.getScore() << std::endl;

               auto doc = hit.getDocument();
               if (doc.contains("title"))
               {
                    std::cout << "Title: " << doc.value("title", "") << std::endl;
               }

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

          if (search_result.hasNextPage())
          {
               std::cout << "---" << std::endl;
               std::cout << "More results available on next page." << std::endl;
          }
     }
     catch (const std::exception& e)
     {
          std::cerr << "Error: " << e.what() << std::endl;
          /* In a real example, we might not have the server running */
     }

     return 0;
}
