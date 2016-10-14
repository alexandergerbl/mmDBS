#ifndef ORDERLINE_HPP
#define ORDERLINE_HPP

#include "Row.hpp"

class Row_OrderLine : public Row<Integer, Integer, Integer, Integer, Integer, Integer, Date, Numeric<2, 0>, Numeric<6, 2>, Char<24>>
{
public:
  Row_OrderLine(std::string line) : Row<Integer, Integer, Integer, Integer, Integer, Integer, Date, Numeric<2, 0>, Numeric<6, 2>, Char<24>>(line)
  {}
  
  inline Integer& ol_o_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline Integer& ol_d_id()
  {
    return std::get<1>(this->data);     
  }
  
  inline Integer& ol_w_id()
  {
    return std::get<2>(this->data);     
  }
  
  inline Integer& ol_number()
  {
    return std::get<3>(this->data);     
  }
  
  inline Integer& ol_i_id()
  {
    return std::get<4>(this->data);     
  }
  
  inline Integer& ol_supply_w_id()
  {
    return std::get<5>(this->data);     
  }
  
  
  inline Date& ol_delivery_d()
  {
    return std::get<6>(this->data);     
  }
  
  inline Numeric<2, 0>& ol_quantity()
  {
    return std::get<7>(this->data);     
  }
  
  inline Numeric<6, 2>& ol_amount()
  {
    return std::get<8>(this->data);     
  }
  
  inline Char<24>& ol_dist_info()
  {
    return std::get<9>(this->data);     
  }
};


class OrderLine 
{
public:
  std::vector<Row_OrderLine> rows;
  
  OrderLine(std::string file);
};
  


#endif