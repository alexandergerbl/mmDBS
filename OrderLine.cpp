#include "OrderLine.hpp"

#include<fstream>
#include<experimental/filesystem>


OrderLine::OrderLine(std::string file)
{

  if(!std::experimental::filesystem::exists(file))
  {
    throw std::invalid_argument("Path to orderline data wrong!");
  }
  
  std::ifstream in { file };
  std::string line;
  
  while(std::getline(in, line))
  {
    this->rows.emplace_back<Row_OrderLine>(line); 
    auto tid = this->rows.size() - 1;
    this->primaryKeys[std::make_tuple(this->rows[tid].ol_w_id(), this->rows[tid].ol_d_id(), this->rows[tid].ol_o_id(), this->rows[tid].ol_number())] = tid;
  }
  
}

Row_OrderLine& OrderLine::getByPrimaryKey(std::tuple<Integer, Integer, Integer, Integer> const& primaryKey)
{
  return this->rows[this->primaryKeys[primaryKey]]; 
}

void OrderLine::insert(Integer ol_o_id, Integer ol_d_id, Integer ol_w_id, Integer ol_number, Integer ol_i_id, Integer ol_supply_w_id, Date ol_delivery_d, Numeric<2, 0> ol_quantity, Numeric<6, 2> ol_amount, Char<24> ol_dist_info)
{
  this->rows.emplace_back(ol_o_id, ol_d_id, ol_w_id, ol_number, ol_i_id, ol_supply_w_id, ol_delivery_d, ol_quantity, ol_amount, ol_dist_info);
  auto tid = this->rows.size();
  this->primaryKeys[std::make_tuple(ol_w_id, ol_d_id, ol_o_id, ol_number)] = tid;  
}
