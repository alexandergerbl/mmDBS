#ifndef ORDERCOLUMN_HPP
#define ORDERCOLUMN_HPP

#include <unordered_map>
#include <tuple>

#include "Types.hpp"

#include "ColumnStore.hpp"

#include "My_Hashes.hpp"

class OrderColumn : public ColumnStore<3, Integer, Integer, Integer, Integer, Date, Integer, Numeric<2, 0>, Numeric<1, 0>>
{
public:
  OrderColumn(std::string file) : ColumnStore(file) {}
  
  Tid getByPrimaryKey(Integer const& o_id, Integer const& o_d_id, Integer const& o_w_id) const;
  
  std::size_t size() const;
  
  void insert(Integer o_id, Integer o_d_id, Integer o_w_id, Integer o_c_id, Date o_entry_d, Integer o_carrier_id, Numeric<2, 0> o_ol_cnt, Numeric<1, 0> o_all_local);
  
  
  inline auto& o_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline auto& o_d_id()
  {
    return std::get<1>(this->data);     
  }
  
  inline auto& o_w_id()
  {
    return std::get<2>(this->data);     
  }
  
  inline auto& o_c_id()
  {
    return std::get<3>(this->data);     
  }
  
  inline auto& o_entry_d()
  {
    return std::get<4>(this->data);     
  }
  
  inline auto& o_carrier_id()
  {
    return std::get<5>(this->data);     
  }
  
  inline auto& o_ol_cnt()
  {
    return std::get<6>(this->data);     
  }
  
  inline auto& o_all_local()
  {
    return std::get<7>(this->data);     
  }
  
};
  
#endif
  
