#include "Customer.hpp"

#include<fstream>
#include<experimental/filesystem>


Customer::Customer(std::string file)
{
  if(!std::experimental::filesystem::exists(file))
  {
    throw std::invalid_argument("Path to customer data wrong!");
  }
  
  std::ifstream in { file };
  std::string line;
  
  while(std::getline(in, line))
  {
    this->rows.emplace_back<Row_Customer>(line); 
    auto tid = this->rows.size() -1;
    this->primaryKeys[std::make_tuple(this->rows[tid].c_w_id(), this->rows[tid].c_d_id(),this->rows[tid].c_id())] = tid;
  }
  
}

Row_Customer& Customer::getByPrimaryKey(std::tuple<Integer, Integer, Integer> const& primaryKey)
{
  return this->rows[this->primaryKeys[primaryKey]];
}