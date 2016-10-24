#if !defined(__cplusplus) || !defined(__GNUC__) 
#error "Wrong Compiler!" 
#endif

#include <iostream>
#include <chrono>

#include "Database.hpp"

#include "DatabaseColumn.hpp"


int main()
{
  std::cout << "Practial Course - Main Memory Database System\n" << std::endl;

  {
    DatabaseColumn db_column;
    db_column.printTask1Info(1'000'000);
    
    Integer w_id = 4;
    Integer d_id = 1;
    Integer c_id = 1;
    int32_t items = 1;
    int32_t supware[items];
        std::fill(supware, supware+items, w_id.value);
    
    int32_t itemid[items];
    auto i_id = 1;
    std::fill(itemid, itemid+items, i_id);
    
    int32_t qty[items]; 
    std::fill(qty, qty+items, 51);
    Timestamp datetime{0};
    
    db_column.newOrder(w_id, d_id, c_id, items, supware, itemid, qty, datetime);
  }
  
  {
    Database db;
    
    db.printTask1Info(1'000'000);
    
    Integer w_id = 4;
    Integer d_id = 1;
    Integer c_id = 1;
    int32_t items = 1;
    int32_t supware[items];
        std::fill(supware, supware+items, w_id.value);
    
    int32_t itemid[items];
    auto i_id = 1;
    std::fill(itemid, itemid+items, i_id);
    
    int32_t qty[items]; 
    std::fill(qty, qty+items, 51);
    Timestamp datetime{0};
    
    db.newOrder(w_id, d_id, c_id, items, supware, itemid, qty, datetime);
  }
    
    
  return 0;
}
