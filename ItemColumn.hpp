#ifndef ITEMCOLUMN_HPP
#define ITEMCOLUMN_HPP


#include "My_Hashes.hpp"
#include "ColumnStore.hpp"

#include <unordered_map>


class ItemColumn : ColumnStore<1, Integer, Integer, Varchar<24>, Numeric<5, 2>, Varchar<50>>
{
public:
  ItemColumn(std::string file) :ColumnStore(file){}
 
  Tid getByPrimaryKey(Integer const& i_id) const; 
  
  std::size_t size() const;
  
    
  inline auto& i_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline auto& i_im_id()
  {
    return std::get<1>(this->data);     
  }
  
  inline auto& i_name()
  {
    return std::get<2>(this->data);     
  }
  
  inline auto& i_price()
  {
    return std::get<3>(this->data);     
  }
  
  inline auto& i_data()
  {
    return std::get<4>(this->data);     
  }

};

#endif
