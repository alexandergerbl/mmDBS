#if !defined(__cplusplus) || !defined(__GNUC__) 
#error "Wrong Compiler!" 
#endif

#include <iostream>

#include "Customer.hpp"
#include "District.hpp"
#include "History.hpp"
#include "Item.hpp"
#include "NewOrder.hpp"
#include "Order.hpp"
#include "OrderLine.hpp"
#include "Stock.hpp"
#include "Warehouse.hpp"

int main()
{
  std::cout << "Main Memory Database System\n" << std::endl;

  //USAGE
  Customer c{"./task1/tpcc_customer.tbl"};
  District d{"./task1/tpcc_district.tbl"};
  History h{"./task1/tpcc_history.tbl"};
  Item i{"./task1/tpcc_item.tbl"};
  NewOrder no{"./task1/tpcc_neworder.tbl"};
  Order o{"./task1/tpcc_order.tbl"};
  OrderLine ol{"./task1/tpcc_orderline.tbl"};
  Stock s{"./task1/tpcc_stock.tbl"};
  Warehouse w{"./task1/tpcc_warehouse.tbl"};
  
  std::cout << "Customer has " << c.rows.size() << " rows!" << std::endl;
  std::cout << "District has " << d.rows.size() << " rows!" << std::endl;
  std::cout << "History has " << h.rows.size() << " rows!" << std::endl;
  std::cout << "Item has " << i.rows.size() << " rows!" << std::endl;
  std::cout << "NewOrder has " << no.rows.size() << " rows!" << std::endl;
  std::cout << "Order has " << o.rows.size() << " rows!" << std::endl;
  std::cout << "Orderline has " << ol.rows.size() << " rows!" << std::endl;
  std::cout << "Stock has " << s.rows.size() << " rows!" << std::endl;
  std::cout << "Warhouse has " << w.rows.size() << " rows!" << std::endl;
  
  return 0;
}
