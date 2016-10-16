#if !defined(__cplusplus) || !defined(__GNUC__) 
#error "Wrong Compiler!" 
#endif

#include <iostream>
#include <chrono>

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
  
  void newOrder(Integer w_id, Integer d_id, Integer c_id, int32_t items, int32_t* supware, int32_t* itemid, int32_t* qty, Timestamp datetime)
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
    o.insert(o_id, d_id, w_id, c_id, Date{(int32_t) datetime.value}, 0, Integer{items}, Integer{all_local});
    
    //insert data into NewOrder Table
    no.insert(o_id, d_id, w_id);
    
    for(int32_t index = 0; index < items; index++)
    {
      auto i_price = i.getByPrimaryKey(itemid[index]).i_price();
      
      Row_Stock& tmp_s = s.getByPrimaryKey(std::make_pair(supware[index], itemid[index]));
      Char<24> s_dist;
      
      switch(d_id.value)
      {
        case(1):
              s_dist = tmp_s.s_dist_01();
              break;
        case(2):
              s_dist = tmp_s.s_dist_02();
              break;
        case(3):
              s_dist = tmp_s.s_dist_03();
              break;
        case(4):
              s_dist = tmp_s.s_dist_04();
              break;
        case(5):
              s_dist = tmp_s.s_dist_05();
              break;
        case(6):
              s_dist = tmp_s.s_dist_06();
              break;
        case(7):
              s_dist = tmp_s.s_dist_07();
              break;
        case(8):
              s_dist = tmp_s.s_dist_08();
              break;
        case(9):
              s_dist = tmp_s.s_dist_09();
              break;
        case(10):
              s_dist = tmp_s.s_dist_10();
              break;
          default:
              break;
      }
      
      if(tmp_s.s_quantity() > qty[index])
      {
          tmp_s.s_quantity() = tmp_s.s_quantity() - qty[index];
      }
      else
      {
          tmp_s.s_quantity() = tmp_s.s_quantity() + 91 - qty[index];
          
          if(supware[index] != w_id.value)
          {
              tmp_s.s_remote_cnt() = tmp_s.s_remote_cnt() + 1;
          }else
          {
              tmp_s.s_remote_cnt() = tmp_s.s_order_cnt() + 1;
          }
          
          //insert into orderline
          Numeric<6, 2> a{qty[index]*100};
          Numeric<6, 2> b{i_price.value};
          Numeric<6, 2> c{(10000+w_tax.value+d_tax.value)/100};
          Numeric<6, 2> d{(10000-c_discount.value)/100};
          auto ol_amount = (((a.value*b.value)/100)*((c.value*d.value)/100)) / 100;

          //
          ol.insert(o_id, d_id, w_id, Integer{index+1}, Integer{itemid[index]}, Integer{supware[index]}, Date{0}, Numeric<2, 0>(qty[index]), ol_amount, s_dist);
      }
      
    }    
    
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

  TpccDatabase db;
  
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  
  for(auto i = 0; i < 1000000; i++)
  {
      db.newOrderRandom(Timestamp{static_cast<uint64_t>( 40+i)}, i%5);
  }
  end = std::chrono::system_clock::now();

  std::cout << 1000000/ std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " newOrderRandom/s" << std::endl;
  

  
  
  return 0;
}
