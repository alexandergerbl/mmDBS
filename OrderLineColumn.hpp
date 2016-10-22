#ifndef ORDERLINECOLUMN_HPP
#define ORDERLINECOLUMN_HPP

#include <unordered_map>
#include <tuple>

#include "ColumnStore.hpp"

#include "My_Hashes.hpp"


class OrderLineColumn : public ColumnStore<4, Integer, Integer, Integer, Integer, Integer, Integer, Date, Numeric<2, 0>, Numeric<6, 2>, Char<24>> 
{
public:
  OrderLineColumn(std::string file) : ColumnStore(file){}
  
  Tid getByPrimaryKey(const Integer& ol_o_id, const Integer& ol_d_id, const Integer& ol_w_id, const Integer& ol_number) const;
  
  std::size_t size() const;
  
  void insert(Integer ol_o_id, Integer ol_d_id, Integer ol_w_id, Integer ol_number, Integer ol_i_id, Integer ol_supply_w_id, Date ol_delivery_d, Numeric<2, 0> ol_quantity, Numeric<6, 2> ol_amount, Char<24> ol_dist_info);
  
  
  inline auto& ol_o_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline auto& ol_d_id()
  {
    return std::get<1>(this->data);     
  }
  
  inline auto& ol_w_id()
  {
    return std::get<2>(this->data);     
  }
  
  inline auto& ol_number()
  {
    return std::get<3>(this->data);     
  }
  
  inline auto& ol_i_id()
  {
    return std::get<4>(this->data);     
  }
  
  inline auto& ol_supply_w_id()
  {
    return std::get<5>(this->data);     
  }
  
  
  inline auto& ol_delivery_d()
  {
    return std::get<6>(this->data);     
  }
  
  inline auto& ol_quantity()
  {
    return std::get<7>(this->data);     
  }
  
  inline auto& ol_amount()
  {
    return std::get<8>(this->data);     
  }
  
  inline auto& ol_dist_info()
  {
    return std::get<9>(this->data);     
  }

};
  


#endif
