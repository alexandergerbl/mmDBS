#ifndef STOCK_HPP
#define STOCK_HPP

#include <map>
#include <utility>

#include "Row.hpp"

class Row_Stock : public Row<Integer, Integer, Numeric<4, 0>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Numeric<8, 0>, Numeric<4, 0>, Numeric<4, 0>, Varchar<50>>
{
public:
  Row_Stock(std::string line) : Row<Integer, Integer, Numeric<4, 0>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Numeric<8, 0>, Numeric<4, 0>, Numeric<4, 0>, Varchar<50>>(line)
  {}
  
  inline Integer& s_i_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline Integer& s_w_id()
  {
    return std::get<1>(this->data);     
  }
  
  inline Numeric<4,0>& s_quantity()
  {
    return std::get<2>(this->data);     
  }
  
  inline Char<24>& s_dist_01()
  {
    return std::get<3>(this->data);     
  }
  
  inline Char<24>& s_dist_02()
  {
    return std::get<4>(this->data);     
  }
  inline Char<24>& s_dist_03()
  {
    return std::get<5>(this->data);     
  }
  inline Char<24>& s_dist_04()
  {
    return std::get<6>(this->data);     
  }
  inline Char<24>& s_dist_05()
  {
    return std::get<7>(this->data);     
  }
  inline Char<24>& s_dist_06()
  {
    return std::get<8>(this->data);     
  }
  inline Char<24>& s_dist_07()
  {
    return std::get<9>(this->data);     
  }
  inline Char<24>& s_dist_08()
  {
    return std::get<10>(this->data);     
  }
  inline Char<24>& s_dist_09()
  {
    return std::get<11>(this->data);     
  }
  inline Char<24>& s_dist_10()
  {
    return std::get<12>(this->data);     
  }
  inline Numeric<8, 0>& s_ytd()
  {
    return std::get<13>(this->data);     
  }
  
  inline Numeric<4, 0>& s_order_cnt()
  {
    return std::get<14>(this->data);     
  }
  
  inline Numeric<4, 0>& s_remote_cnt()
  {
    return std::get<15>(this->data);     
  }
  
  inline Varchar<50>& s_data()
  {
    return std::get<16>(this->data);     
  }
  
  
};

class Stock
{
  std::map<std::pair<Integer, Integer>, Tid> primaryKeys;
public:
  std::vector<Row_Stock> rows;
  
  Stock(std::string file);
  
  Row_Stock& getByPrimaryKey(std::pair<Integer, Integer> const& primaryKey);
};


#endif