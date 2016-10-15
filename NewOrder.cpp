#include "NewOrder.hpp"

#include<fstream>
#include<experimental/filesystem>


NewOrder::NewOrder(std::string file)
{

  if(!std::experimental::filesystem::exists(file))
  {
    throw std::invalid_argument("Path to neworder data wrong!");
  }
  
  std::ifstream in { file };
  std::string line;
  
  while(std::getline(in, line))
  {
    this->rows.emplace_back<Row_NewOrder>(line); 
  }
  
}

void NewOrder::insert(Integer no_o_id, Integer no_d_id, Integer no_w_id)
{
  this->rows.emplace_back(no_o_id, no_d_id, no_w_id);
}
