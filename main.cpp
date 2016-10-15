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


class TpccDatabase
{
  Customer c{"./task1/tpcc_customer.tbl"};
  District d{"./task1/tpcc_district.tbl"};
  History h{"./task1/tpcc_history.tbl"};
  Item i{"./task1/tpcc_item.tbl"};
  NewOrder no{"./task1/tpcc_neworder.tbl"};
  Order o{"./task1/tpcc_order.tbl"};
  OrderLine ol{"./task1/tpcc_orderline.tbl"};
  Stock s{"./task1/tpcc_stock.tbl"};
  Warehouse w{"./task1/tpcc_warehouse.tbl"};
  
public: 
  TpccDatabase(){};
  
  //Transactions
  
  void newOrder(Integer w_id, Integer d_id, Integer c_id, int32_t items, int32_t* supware, int32_t* itemid, int32_t* qty, Timestamp now)
  {
    auto w_tax = this->w.w_id(w_id).w_tax();
    auto c_discount = this->c.getByPrimaryKey(std::make_tuple(w_id, d_id, c_id)).c_discount();
    Row_District& tmp_district = this->d.getByPrimaryKey(std::make_pair(w_id, d_id));
    auto o_id = tmp_district.d_next_o_id();
    auto d_tax = tmp_district.d_tax();
    //update
    tmp_district.d_next_o_id() = tmp_district.d_next_o_id() + 1;
    
    //check if all are local -> std::any_if or SSE?
    int32_t all_local = 1;
    for(int32_t index = 0; index < items; index++)
    {
      if(w_id != supware[index])
	all_local = 0;
    }
    //insert data into Order Table
    
    //insert data into NewOrder Table
    
    
    
  }


  const int32_t warehouses=5;

  int32_t urand(int32_t min,int32_t max) {
    return (random()%(max-min+1))+min;
  }

  int32_t urandexcept(int32_t min,int32_t max,int32_t v) {
    if (max<=min)
	return min;
    int32_t r=(random()%(max-min))+min;
    if (r>=v)
	return r+1; else
	return r;
  }

  int32_t nurand(int32_t A,int32_t x,int32_t y) {
    return ((((random()%A)|(random()%(y-x+1)+x))+42)%(y-x+1))+x;
  }

  void newOrderRandom(Timestamp now,int32_t w_id) {
    int32_t d_id=urand(1,1);
    int32_t c_id=nurand(1023,1,3000);
    int32_t ol_cnt=urand(5,15);

    int32_t supware[15];
    int32_t itemid[15];
    int32_t qty[15];
    for (int32_t i=0; i<ol_cnt; i++) {
	if (urand(1,100)>1)
	  supware[i]=w_id; else
	  supware[i]=urandexcept(1,warehouses,w_id);
	itemid[i]=nurand(8191,1,100000);
	qty[i]=urand(1,10);
    }

    newOrder(w_id,d_id,c_id,ol_cnt,supware,itemid,qty,now);
  }
  
};



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
