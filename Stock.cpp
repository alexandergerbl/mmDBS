#include "Stock.hpp"

#include<fstream>
#include<experimental/filesystem>


Stock::Stock(std::string file)
{

  if(!std::experimental::filesystem::exists(file))
  {
    throw std::invalid_argument("Path to stock data wrong!");
  }
  
  std::ifstream in { file };
  std::string line;
  
  while(std::getline(in, line))
  {
    this->rows.emplace_back<Row_Stock>(line); 
    auto tid = this->rows.size() - 1;
    this->primaryKeys[std::make_pair(this->rows[tid].s_w_id(), this->rows[tid].s_i_id())] = tid;
  }
  
}

Row_Stock& Stock::getByPrimaryKey(std::pair<Integer, Integer> const& primaryKey)
{
  return this->rows[this->primaryKeys[primaryKey]];
}

void Stock::insert(Integer s_i_id, Integer s_w_id, Numeric<4,0> s_quantity, Char<24> s_dist_01, Char<24> s_dist_02, Char<24> s_dist_03, Char<24> s_dist_04, Char<24> s_dist_05, Char<24> s_dist_06, Char<24> s_dist_07, Char<24> s_dist_08, Char<24> s_dist_09, Char<24> s_dist_10, Numeric<8, 0> s_ytd,  Numeric<4, 0> s_order_cnt, Numeric<4, 0> s_remote_cnt, Varchar<50> s_data)
{
  this->rows.emplace_back(s_i_id, s_w_id, s_quantity, s_dist_01, s_dist_02, s_dist_03, s_dist_04, s_dist_05, s_dist_06, s_dist_07, s_dist_08, s_dist_09, s_dist_10, s_ytd, s_order_cnt, s_remote_cnt, s_data);
  auto tid = this->rows.size() -1;
  this->primaryKeys[std::make_pair(s_w_id, s_i_id)] = tid;  
}