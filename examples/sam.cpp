#include "hlquery/client.h"

#include <iostream>

int main()
{
     try
     {
          hlquery::Client client("http://localhost:9200");
          auto sam = client.sam();

          auto status = sam->status("music");
          std::cout << "SAM status:" << std::endl;
          std::cout << status.getBody().dump(2) << std::endl;

          auto history = sam->history("music", 5);
          std::cout << "SAM history:" << std::endl;
          std::cout << history.getBody().dump(2) << std::endl;

          auto results = sam->search("music", "queen of pop", {
               {"limit", "10"},
          });
          std::cout << "SAM search:" << std::endl;
          std::cout << results.getBody().dump(2) << std::endl;
     }
     catch (const std::exception& e)
     {
          std::cerr << "Error: " << e.what() << std::endl;
          return 1;
     }

     return 0;
}
