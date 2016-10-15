#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <vector>
#include <map>

#include "Row.hpp"
#include "Types.hpp"



class Row_Warehouse : public Row<Integer, Varchar<10>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Numeric<4, 4>, Numeric<12, 2>>
{
public:
  Row_Warehouse(std::string const& line) : Row<Integer, Varchar<10>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Numeric<4, 4>, Numeric<12, 2>>(line) {}
  
  inline Integer& w_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline Varchar<10>& w_name()
  {
    return std::get<1>(this->data);     
  }
  
  inline Varchar<20>& w_street_1()
  {
    return std::get<2>(this->data);     
  }
  
  Varchar<20>& w_street_2()
  {
    return std::get<3>(this->data);     
  }
  
  Varchar<20>& w_city()
  {
    return std::get<4>(this->data);     
  }
  
  Char<2>& w_state()
  {
    return std::get<5>(this->data);     
  }
  
  Char<9>& w_zip()
  {
    return std::get<6>(this->data);     
  }
  
  Numeric<4, 4>& w_tax()
  {
    return std::get<7>(this->data);     
  }
  
  Numeric<12, 2>& w_ytd()
  {
    return std::get<8>(this->data);     
  }  
  
};


class Warehouse
{
  std::map<Integer, Tid> primaryKey;
  
public:
  std::vector<Row_Warehouse> rows;
  
  Warehouse(std::string file);
  
  Row_Warehouse& w_id(Integer const& w_id);
};

#endif