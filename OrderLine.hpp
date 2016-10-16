#ifndef ORDERLINE_HPP
#define ORDERLINE_HPP

#include <unordered_map>
#include <tuple>

#include "Row.hpp"

class Row_OrderLine : public Row<Integer, Integer, Integer, Integer, Integer, Integer, Date, Numeric<2, 0>, Numeric<6, 2>, Char<24>>
{
public:
  Row_OrderLine(std::string line) : Row<Integer, Integer, Integer, Integer, Integer, Integer, Date, Numeric<2, 0>, Numeric<6, 2>, Char<24>>(line)
  {}
  
  Row_OrderLine(Integer ol_o_id, Integer ol_d_id, Integer ol_w_id, Integer ol_number, Integer ol_i_id, Integer ol_supply_w_id, Date g, Numeric<2, 0> h, Numeric<6, 2> i, Char<24> j) : Row(std::move(ol_o_id), std::move(ol_d_id), std::move(ol_w_id), std::move(ol_number), std::move(ol_i_id), std::move(ol_supply_w_id), std::move(g), std::move(h), std::move(i), std::move(j))
  {}
  
  inline Integer& ol_o_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline Integer& ol_d_id()
  {
    return std::get<1>(this->data);     
  }
  
  inline Integer& ol_w_id()
  {
    return std::get<2>(this->data);     
  }
  
  inline Integer& ol_number()
  {
    return std::get<3>(this->data);     
  }
  
  inline Integer& ol_i_id()
  {
    return std::get<4>(this->data);     
  }
  
  inline Integer& ol_supply_w_id()
  {
    return std::get<5>(this->data);     
  }
  
  
  inline Date& ol_delivery_d()
  {
    return std::get<6>(this->data);     
  }
  
  inline Numeric<2, 0>& ol_quantity()
  {
    return std::get<7>(this->data);     
  }
  
  inline Numeric<6, 2>& ol_amount()
  {
    return std::get<8>(this->data);     
  }
  
  inline Char<24>& ol_dist_info()
  {
    return std::get<9>(this->data);     
  }
};

struct IntIntIntIntHash
{
    //TODO XOR of only 2 parameters should also be fine - but check this 
  std::size_t operator()(std::tuple<Integer, Integer, Integer, Integer> const& p) const
  {
     return (std::get<0>(p).hash() ^ std::get<1>(p).hash()) ^ (std::get<2>(p).hash() ^ std::get<3>(p).hash());
  }
};

class OrderLine 
{
  std::unordered_map<std::tuple<Integer, Integer, Integer, Integer>, Tid, IntIntIntIntHash> primaryKeys;
public:
  std::vector<Row_OrderLine> rows;
  
  OrderLine(std::string file);
  
  Row_OrderLine& getByPrimaryKey(std::tuple<Integer, Integer, Integer, Integer> const& primaryKey);
  
  void insert(Integer ol_o_id, Integer ol_d_id, Integer ol_w_id, Integer ol_number, Integer ol_i_id, Integer ol_supply_w_id, Date ol_delivery_d, Numeric<2, 0> ol_quantity, Numeric<6, 2> ol_amount, Char<24> ol_dist_info);
};
  


#endif
