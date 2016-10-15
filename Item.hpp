#ifndef ITEM_HPP
#define ITEM_HPP

#include "Row.hpp"

#include <map>

class Row_Item : public Row<Integer, Integer, Varchar<24>, Numeric<5, 2>, Varchar<50>>
{
public:
  Row_Item(std::string line) : Row<Integer, Integer, Varchar<24>, Numeric<5, 2>, Varchar<50>>(line)
  {}
  
  inline Integer& i_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline Integer& i_im_id()
  {
    return std::get<1>(this->data);     
  }
  
  inline Varchar<24>& i_name()
  {
    return std::get<2>(this->data);     
  }
  
  inline Numeric<5, 2>& i_price()
  {
    return std::get<3>(this->data);     
  }
  
  inline Varchar<50>& i_data()
  {
    return std::get<4>(this->data);     
  }
};
  
class Item 
{
  std::map<Integer, Tid> primaryKeys;
public:
  std::vector<Row_Item> rows;
  
  Item(std::string file);
  
  Row_Item& getByPrimaryKey(Integer const& primaryKey);
};

#endif