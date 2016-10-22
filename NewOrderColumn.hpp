#ifndef NEWORDERCOLUMN_HPP
#define NEWORDERCOLUMN_HPP

#include "Row.hpp"

#include "ColumnStore.hpp"

class NewOrderColumn : ColumnStore<3, Integer, Integer, Integer>
{
public:
  NewOrderColumn(std::string file) : ColumnStore(file){}
  
  Tid getByPrimaryKey(Integer const& no_o_id, Integer const& no_d_id, Integer const& no_w_id) const;
  
  std::size_t size() const;
  
  void insert(Integer no_o_id, Integer no_d_id, Integer no_w_id);
  
  inline auto& no_o_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline auto& no_d_id()
  {
    return std::get<1>(this->data);     
  }
  
  inline auto& no_w_id()
  {
    return std::get<2>(this->data);     
  }
};
  

#endif
