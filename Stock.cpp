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