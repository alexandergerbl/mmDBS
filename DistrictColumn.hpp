#ifndef DISTRICTCOLUMN_HPP
#define DISTRICTCOLUMN_HPP

#include "My_Hashes.hpp"

#include "ColumnStore.hpp"

#include <unordered_map>
#include <utility>


class DistrictColumn : ColumnStore<2, Integer, Integer, Varchar<10>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Numeric<4, 4>, Numeric<12, 2>, Integer>
 
{
public:
  
  DistrictColumn(std::string file) : ColumnStore(file){}
  
  Tid getByPrimaryKey(Integer const& d_id, Integer const& d_w_id) const;
  
  std::size_t size() const;
  
  inline auto& d_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline auto& d_w_id()
  {
    return std::get<1>(this->data);     
  }
  
  
  inline auto& d_name()
  {
    return std::get<2>(this->data);     
  }
  
  inline auto& d_street_1()
  {
    return std::get<3>(this->data);     
  }
  
  auto& d_street_2()
  {
    return std::get<4>(this->data);     
  }
  
  auto& d_city()
  {
    return std::get<5>(this->data);     
  }
  
  auto& d_state()
  {
    return std::get<6>(this->data);     
  }
  
  auto& d_zip()
  {
    return std::get<7>(this->data);     
  }
  
  auto& d_tax()
  {
    return std::get<8>(this->data);     
  }
  
  auto& d_ytd()
  {
    return std::get<9>(this->data);     
  }  
  auto& d_next_o_id()
  {
    return std::get<10>(this->data);     
  }
};

#endif
