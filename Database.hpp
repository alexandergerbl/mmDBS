#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "Customer.hpp"
#include "District.hpp"
#include "History.hpp"
#include "Item.hpp"
#include "NewOrder.hpp"
#include "Order.hpp"
#include "OrderLine.hpp"
#include "Stock.hpp"
#include "Warehouse.hpp"

#include <iostream>


class Database
{
public:
  Customer c{"./task1/tpcc_customer.tbl"};
  District d{"./task1/tpcc_district.tbl"};
  History h{"./task1/tpcc_history.tbl"};
  Item i{"./task1/tpcc_item.tbl"};
  NewOrder no{"./task1/tpcc_neworder.tbl"};
  Order o{"./task1/tpcc_order.tbl"};
  OrderLine ol{"./task1/tpcc_orderline.tbl"};
  Stock s{"./task1/tpcc_stock.tbl"};
  Warehouse w{"./task1/tpcc_warehouse.tbl"};
  
  Database(){};
  
  //Transactions
  
  void printInfo() const;
  
  void printTask1Info(int numRepeat);
  
  void newOrder(Integer w_id, Integer d_id, Integer c_id, int32_t items, int32_t* supware, int32_t* itemid, int32_t* qty, Timestamp datetime);
  
  const int32_t warehouses=5;

  int32_t urand(int32_t min,int32_t max);

  int32_t urandexcept(int32_t min,int32_t max,int32_t v) ;

  int32_t nurand(int32_t A,int32_t x,int32_t y) ;

  void newOrderRandom(Timestamp now,int32_t w_id) ;
};

#endif
