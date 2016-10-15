#ifndef ORDER_HPP
#define ORDER_HPP

#include <map>
#include <tuple>

#include "Row.hpp"

class Row_Order : public Row<Integer, Integer, Integer, Integer, Date, Integer, Numeric<2, 0>, Numeric<1, 0>>
{
public:
  Row_Order(std::string line) : Row<Integer, Integer, Integer, Integer, Date, Integer, Numeric<2, 0>, Numeric<1, 0>>(line)
  {}
  
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
  std::map<std::tuple<Integer, Integer, Integer>, Tid> primaryKeys;
public:
  std::vector<Row_Order> rows;
  
  Order(std::string file);
  
  Row_Order& getByPrimaryKey(std::tuple<Integer, Integer, Integer> const& primaryKey);
};
  
#endif
  