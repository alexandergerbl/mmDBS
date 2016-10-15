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
    this->rows.emplace_back<Row_Warehouse>(line); 
    auto w_id = this->rows[this->rows.size()-1].w_id();
    this->primaryKey[w_id] = this->rows.size()-1;
  }
   
}


Row_Warehouse& Warehouse::w_id(Integer const& w_id)
{
  return this->rows[this->primaryKey[w_id]];
}