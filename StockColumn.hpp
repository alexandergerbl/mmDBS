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
  
  //void insert(Integer s_i_id, Integer s_w_id, Numeric<4,0> s_quantity, Char<24> s_dist_01, Char<24> s_dist_02, Char<24> s_dist_03, Char<24> s_dist_04, Char<24> s_dist_05, Char<24> s_dist_06, Char<24> //s_dist_07, Char<24> s_dist_08, Char<24> s_dist_09, Char<24> s_dist_10, Numeric<8, 0> s_ytd,  Numeric<4, 0> s_order_cnt, Numeric<4, 0> s_remote_cnt, Varchar<50> s_data);
};


#endif
