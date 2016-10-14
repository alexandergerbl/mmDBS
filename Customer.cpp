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
    this->rows.push_back(Row_Customer(line)); 
  }
  
}
