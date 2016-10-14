#if !defined(__cplusplus) || !defined(__GNUC__) 
#error "Wrong Compiler!" 
#endif

#include <iostream>

#include "Warehouse.hpp"
#include "District.hpp"
#include "Item.hpp"
#include "NewOrder.hpp"
#include "Stock.hpp"


int main()
{
  std::cout << "Main Memory Database System\n" << std::endl;

  //USAGE
  Warehouse w{"./task1/tpcc_warehouse.tbl"};
  District d{"./task1/tpcc_district.tbl"};
  Item i{"./task1/tpcc_item.tbl"};
  NewOrder no{"./task1/tpcc_neworder.tbl"};
  Stock s{"./task1/tpcc_stock.tbl"};
  
  
  std::cout << "Warhouse has " << w.rows.size() << " rows!" << std::endl;
  std::cout << "District has " << d.rows.size() << " rows!" << std::endl;
  std::cout << "Item has " << i.rows.size() << " rows!" << std::endl;
  std::cout << "NewOrder has " << no.rows.size() << " rows!" << std::endl;
  std::cout << "Stock has " << s.rows.size() << " rows!" << std::endl;
  
  
  return 0;
}
