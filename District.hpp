#ifndef DISTRICT_HPP
#define DISTRICT_HPP

#include "Row.hpp"
#include <map>
#include <utility>

class Row_District : public Row<Integer, Integer, Varchar<10>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Numeric<4, 4>, Numeric<12, 2>, Integer>
{
public:
  Row_District(std::string line) : Row<Integer, Integer, Varchar<10>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Numeric<4, 4>, Numeric<12, 2>, Integer>(line)
  {}
  
  inline Integer& d_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline Integer& d_w_id()
  {
    return std::get<1>(this->data);     
  }
  
  
  inline Varchar<10>& d_name()
  {
    return std::get<2>(this->data);     
  }
  
  inline Varchar<20>& d_street_1()
  {
    return std::get<3>(this->data);     
  }
  
  Varchar<20>& d_street_2()
  {
    return std::get<4>(this->data);     
  }
  
  Varchar<20>& d_city()
  {
    return std::get<5>(this->data);     
  }
  
  Char<2>& d_state()
  {
    return std::get<6>(this->data);     
  }
  
  Char<9>& d_zip()
  {
    return std::get<7>(this->data);     
  }
  
  Numeric<4, 4>& d_tax()
  {
    return std::get<8>(this->data);     
  }
  
  Numeric<12, 2>& d_ytd()
  {
    return std::get<9>(this->data);     
  }  
  Integer& d_next_o_id()
  {
    return std::get<10>(this->data);     
  }  
};

class District 
{
  // (d_w_id, d_id) -> Tid
  std::map<std::pair<Integer, Integer>, Tid> primaryKey;
public:
  std::vector<Row_District> rows;
  
  District(std::string file);
  
  Row_District& getByPrimaryKey(std::pair<Integer, Integer> const& p_key);
};

#endif