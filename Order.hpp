#ifndef ORDER_HPP
#define ORDER_HPP

#include <unordered_map>
#include <tuple>

#include "Row.hpp"

#include "My_Hashes.hpp"

class Row_Order : public Row<Integer, Integer, Integer, Integer, Date, Integer, Numeric<2, 0>, Numeric<1, 0>>
{
public:
  Row_Order(std::string line) : Row<Integer, Integer, Integer, Integer, Date, Integer, Numeric<2, 0>, Numeric<1, 0>>(line)
  {}
  
  Row_Order(Integer o_id, Integer o_d_id, Integer o_w_id, Integer o_c_id, Date o_entry_d, Integer o_carrier_id, Numeric<2, 0> o_ol_cnt, Numeric<1, 0> o_all_local) : Row (std::move(o_id), std::move(o_d_id), std::move(o_w_id), std::move(o_c_id), std::move(o_entry_d), std::move(o_carrier_id), std::move(o_ol_cnt), std::move(o_all_local))
  {
  }
  
  
  inline Integer& o_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline Integer& o_d_id()
  {
    return std::get<1>(this->data);     
  }
  
  inline Integer& o_w_id()
  {
    return std::get<2>(this->data);     
  }
  
  inline Integer& o_c_id()
  {
    return std::get<3>(this->data);     
  }
  
  inline Date& o_entry_d()
  {
    return std::get<4>(this->data);     
  }
  
  inline Integer& o_carrier_id()
  {
    return std::get<5>(this->data);     
  }
  
  inline Numeric<2,0>& o_ol_cnt()
  {
    return std::get<6>(this->data);     
  }
  
  inline Numeric<1, 0>& o_all_local()
  {
    return std::get<7>(this->data);     
  }
    
};



class Order 
{
  std::unordered_map<std::tuple<Integer, Integer, Integer>, Tid, IntIntIntHash> primaryKeys;
public:
  std::vector<Row_Order> rows;
  
  Order(std::string file);
  
  Row_Order& getByPrimaryKey(std::tuple<Integer, Integer, Integer> const& primaryKey);
  
  void insert(Integer o_id, Integer o_d_id, Integer o_w_id, Integer o_c_id, Date o_entry_d, Integer o_carrier_id, Numeric<2, 0> o_ol_cnt, Numeric<1, 0> o_all_local);
};
  
#endif
  
