#ifndef STOCKCOLUMN_HPP
#define STOCKCOLUMN_HPP

#include <unordered_map>
#include <utility>

#include "ColumnStore.hpp"

#include "My_Hashes.hpp"


class StockColumn : public ColumnStore<2, Integer, Integer, Numeric<4, 0>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Numeric<8, 0>, Numeric<4, 0>, Numeric<4, 0>, Varchar<50>>
{
public:
  StockColumn(std::string file) : ColumnStore(file){}
  
  Tid getByPrimaryKey(Integer const& s_i_id, Integer const& s_w_id) const;
  
  std::size_t size() const;
  
  //void insert(Integer s_i_id, Integer s_w_id, Numeric<4,0> s_quantity, Char<24> s_dist_01, Char<24> s_dist_02, Char<24> s_dist_03, Char<24> s_dist_04, Char<24> s_dist_05, Char<24> s_dist_06, Char<24> //s_dist_07, Char<24> s_dist_08, Char<24> s_dist_09, Char<24> s_dist_10, Numeric<8, 0> s_ytd,  Numeric<4, 0> s_order_cnt, Numeric<4, 0> s_remote_cnt, Varchar<50> s_data);
  
  
  inline auto& s_i_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline auto& s_w_id()
  {
    return std::get<1>(this->data);     
  }
  
  inline auto& s_quantity()
  {
    return std::get<2>(this->data);     
  }
  
  inline auto& s_dist_01()
  {
    return std::get<3>(this->data);     
  }
  
  inline auto& s_dist_02()
  {
    return std::get<4>(this->data);     
  }
  inline auto& s_dist_03()
  {
    return std::get<5>(this->data);     
  }
  inline auto& s_dist_04()
  {
    return std::get<6>(this->data);     
  }
  inline auto& s_dist_05()
  {
    return std::get<7>(this->data);     
  }
  inline auto& s_dist_06()
  {
    return std::get<8>(this->data);     
  }
  inline auto& s_dist_07()
  {
    return std::get<9>(this->data);     
  }
  inline auto& s_dist_08()
  {
    return std::get<10>(this->data);     
  }
  inline auto& s_dist_09()
  {
    return std::get<11>(this->data);     
  }
  inline auto& s_dist_10()
  {
    return std::get<12>(this->data);     
  }
  inline auto& s_ytd()
  {
    return std::get<13>(this->data);     
  }
  
  inline auto& s_order_cnt()
  {
    return std::get<14>(this->data);     
  }
  
  inline auto& s_remote_cnt()
  {
    return std::get<15>(this->data);     
  }
  
  inline auto& s_data()
  {
    return std::get<16>(this->data);     
  }
  
};


#endif
