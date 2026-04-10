/*
 * Flush Example
 *
 * Demonstrates the flush operation:
 * 1. Create a fake collection
 * 2. Create a fake document
 * 3. Check collection count
 * 4. Flush all data
 * 5. Re-check collection count (should be 0)
 */

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "hlquery/client.h"

int main()
{
     try
     {
          /* Initialize client */

          hlquery::Client client("http://localhost:9200");

          std::cout << std::string(70, '=') << std::endl;
          std::cout << "FLUSH EXAMPLE" << std::endl;
          std::cout << std::string(70, '=') << std::endl;
          std::cout << std::endl;

          /* Step 1: Create a fake collection */

          std::cout << "Step 1: Creating a fake collection..." << std::endl;
          std::ostringstream oss;
          oss << "flush_test_collection_" << std::time(nullptr);
          std::string collection_name = oss.str();

          nlohmann::json schema = {
               {"fields", nlohmann::json::array({{{"name", "title"}, {"type", "string"}},
                                                 {{"name", "content"}, {"type", "string"}},
                                                 {{"name", "value"}, {"type", "int"}}})}};

          auto create_result = client.collections()->create(collection_name, schema);
          if (create_result.isSuccess())
          {
               std::cout << "  ✓ Collection '" << collection_name << "' created successfully" << std::endl;
          }
          else
          {
               std::cout << "  ✗ Failed to create collection: " << create_result.getStatusCode() << std::endl;
               std::cout << "  Error: " << create_result.getBody().dump(2) << std::endl;
               return 1;
          }

          std::cout << std::endl;

          /* Step 2: Create a fake document */

          std::cout << "Step 2: Creating a fake document..." << std::endl;
          std::ostringstream doc_oss;
          doc_oss << "flush_test_doc_" << std::time(nullptr);
          std::string doc_id = doc_oss.str();

          nlohmann::json doc = {
               {"id", doc_id},
               {"title", "Flush Test Document"},
               {"content", "This is a test document for flush example"},
               {"value", 42}};

          auto add_result = client.documents()->add(collection_name, doc);
          if (add_result.isSuccess())
          {
               std::cout << "  ✓ Document '" << doc_id << "' added successfully" << std::endl;
          }
          else
          {
               std::cout << "  ✗ Failed to add document: " << add_result.getStatusCode() << std::endl;
               std::cout << "  Error: " << add_result.getBody().dump(2) << std::endl;
          }

          std::cout << std::endl;

          /* Step 3: Check collection count before flush */

          std::cout << "Step 3: Checking collection count before flush..." << std::endl;
          auto collections_before = client.listCollections(0, 1000);
          int count_before = 0;
          if (collections_before.isSuccess())
          {
               auto body = collections_before.getBody();
               if (body.contains("collections") && body["collections"].is_array())
               {
                    count_before = body["collections"].size();
                    std::cout << "  Collections before flush: " << count_before << std::endl;
                    if (count_before == 0)
                    {
                         std::cout << "  ⚠ Warning: No collections found before flush" << std::endl;
                    }
               }
          }
          else
          {
               std::cout << "  ✗ Failed to list collections: " << collections_before.getStatusCode() << std::endl;
          }

          std::cout << std::endl;

          /* Step 4: Flush all data */

          std::cout << "Step 4: Flushing all data..." << std::endl;
          auto flush_result = client.flush();
          if (flush_result.isSuccess())
          {
               auto body = flush_result.getBody();
               int collections_deleted = 0;
               if (body.contains("collections_deleted") && body["collections_deleted"].is_number())
               {
                    collections_deleted = body["collections_deleted"];
               }
               std::cout << "  ✓ Flush completed successfully" << std::endl;
               std::cout << "  Collections deleted: " << collections_deleted << std::endl;
               std::string message = "N/A";
               if (body.contains("message") && body["message"].is_string())
               {
                    message = body["message"];
               }
               std::cout << "  Message: " << message << std::endl;
          }
          else
          {
               std::cout << "  ✗ Flush failed: " << flush_result.getStatusCode() << std::endl;
               std::cout << "  Error: " << flush_result.getBody().dump(2) << std::endl;
               return 1;
          }

          std::cout << std::endl;

          /* Step 5: Re-check collection count after flush */

          std::cout << "Step 5: Checking collection count after flush..." << std::endl;
          auto collections_after = client.listCollections(0, 1000);
          int count_after = -1;
          if (collections_after.isSuccess())
          {
               auto body = collections_after.getBody();
               if (body.contains("collections") && body["collections"].is_array())
               {
                    count_after = body["collections"].size();
                    std::cout << "  Collections after flush: " << count_after << std::endl;

                    if (count_after == 0)
                    {
                         std::cout << "  ✓ SUCCESS: All collections have been flushed" << std::endl;
                    }
                    else
                    {
                         std::cout << "  ⚠ Warning: Expected 0 collections, but found " << count_after << std::endl;
                    }
               }
          }
          else
          {
               std::cout << "  ✗ Failed to list collections: " << collections_after.getStatusCode() << std::endl;
          }

          std::cout << std::endl;
          std::cout << std::string(70, '=') << std::endl;
          std::cout << "FLUSH EXAMPLE COMPLETED" << std::endl;
          std::cout << std::string(70, '=') << std::endl;
          std::cout << "Summary:" << std::endl;
          std::cout << "  Collections before flush: " << count_before << std::endl;
          std::cout << "  Collections after flush: " << count_after << std::endl;
          std::cout << std::endl;
     }
     catch (const std::exception& e)
     {
          std::cerr << "Error: " << e.what() << std::endl;
          return 1;
     }

     return 0;
}
