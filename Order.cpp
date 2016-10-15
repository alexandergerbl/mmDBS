#include "Order.hpp"

#include<fstream>
#include<experimental/filesystem>


Order::Order(std::string file)
{

  if(!std::experimental::filesystem::exists(file))
  {
    throw std::invalid_argument("Path to order data wrong!");
  }
  
  std::ifstream in { file };
  std::string line;
  
  while(std::getline(in, line))
  {
    this->rows.emplace_back<Row_Order>(line); 
    auto tid = this->rows.size() - 1;
    this->primaryKeys[std::make_tuple(this->rows[tid].o_w_id(), this->rows[tid].o_d_id(), this->rows[tid].o_c_id())] = tid;
  }
};

Row_Order& Order::getByPrimaryKey(std::tuple<Integer, Integer, Integer> const& primaryKey)
{
  return this->rows[this->primaryKeys[primaryKey]];
}