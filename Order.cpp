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
    this->rows.push_back(Row_Order(line)); 
  }
};