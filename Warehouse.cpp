#include "Warehouse.hpp"

#include<fstream>
#include<experimental/filesystem>



Warehouse::Warehouse(std::string file)
{
  if(!std::experimental::filesystem::exists(file))
  {
    throw std::invalid_argument("Path to warehouse data wrong!");
  }
  
  std::ifstream in { file };
  std::string line;
  
  while(std::getline(in, line))
  {
    this->rows.push_back(Row_Warehouse(line)); 
  }
  
  
}
