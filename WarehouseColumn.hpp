#ifndef WAREHOUSECOLUMN_H
#define WAREHOUSECOLUMN_H

#include <vector>
#include <unordered_map>

#include "ColumnStore.hpp"
#include "Types.hpp"

#include "My_Hashes.hpp"


class WarehouseColumn : ColumnStore<1, Integer, Varchar<10>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Numeric<4, 4>, Numeric<12, 2>>
{
public:
       
    WarehouseColumn(std::string file);
  
  inline auto& w_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline auto& w_name()
  {
    return std::get<1>(this->data);     
  }
  
  inline auto& w_street_1()
  {
    return std::get<2>(this->data);     
  }
  
  inline auto& w_street_2()
  {
    return std::get<3>(this->data);     
  }
  
  inline auto& w_city()
  {
    return std::get<4>(this->data);     
  }
  
  inline auto& w_state()
  {
    return std::get<5>(this->data);     
  }
  
  inline auto& w_zip()
  {
    return std::get<6>(this->data);     
  }
  
  inline auto& w_tax()
  {
    return std::get<7>(this->data);     
  }
  
  inline auto& w_ytd()
  {
    return std::get<8>(this->data);     
  }  

  
};

#endif
