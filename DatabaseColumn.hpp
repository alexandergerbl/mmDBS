#ifndef DATABASECOLUMN_HPP
#define DATABASECOLUMN_HPP

#include "CustomerColumn.hpp"
#include "DistrictColumn.hpp"
#include "HistoryColumn.hpp"
#include "ItemColumn.hpp"
#include "NewOrderColumn.hpp"
#include "OrderColumn.hpp"
#include "OrderLineColumn.hpp"
#include "StockColumn.hpp"
#include "WarehouseColumn.hpp"

#include <iostream>


class DatabaseColumn
{
public:
  CustomerColumn c{"./task1/tpcc_customer.tbl"};
  DistrictColumn d{"./task1/tpcc_district.tbl"};
  HistoryColumn h{"./task1/tpcc_history.tbl"};
  ItemColumn i{"./task1/tpcc_item.tbl"};
  NewOrderColumn no{"./task1/tpcc_neworder.tbl"};
  OrderColumn o{"./task1/tpcc_order.tbl"};
  OrderLineColumn ol{"./task1/tpcc_orderline.tbl"};
  StockColumn s{"./task1/tpcc_stock.tbl"};
  WarehouseColumn w{"./task1/tpcc_warehouse.tbl"};
  
  DatabaseColumn(){};
  
  //Transactions
  
  void printInfo() const;
  
  void printTask1Info(int numRepeat);
  
  void newOrder(Integer w_id, Integer d_id, Integer c_id, int32_t items, int32_t* supware, int32_t* itemid, int32_t* qty, Timestamp datetime);
  
  void delivery(Integer w_id, Integer o_carrier_id, Timestamp datetime);
  
  const int32_t warehouses=5;

  int32_t urand(int32_t min,int32_t max);

  int32_t urandexcept(int32_t min,int32_t max,int32_t v) ;

  int32_t nurand(int32_t A,int32_t x,int32_t y) ;

  void newOrderRandom(Timestamp now,int32_t w_id) ;
};

#endif
