#ifndef NEWORDER_HPP
#define NEWORDER_HPP

#include "Row.hpp"

class Row_NewOrder : public Row<Integer, Integer, Integer>
{
public:
  Row_NewOrder(std::string line) : Row<Integer, Integer, Integer>(line)
  {}
  
  inline Integer& no_o_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline Integer& no_d_id()
  {
    return std::get<1>(this->data);     
  }
  
  inline Integer& no_w_id()
  {
    return std::get<2>(this->data);     
  }
};


class NewOrder
{
public:
  std::vector<Row_NewOrder> rows;
  
  NewOrder(std::string file);
};
  

#endif