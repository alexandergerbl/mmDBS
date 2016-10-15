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