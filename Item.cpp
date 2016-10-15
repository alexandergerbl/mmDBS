#include "Item.hpp"

#include<fstream>
#include<experimental/filesystem>


Item::Item(std::string file)
{

  if(!std::experimental::filesystem::exists(file))
  {
    throw std::invalid_argument("Path to item data wrong!");
  }
  
  std::ifstream in { file };
  std::string line;
  
  while(std::getline(in, line))
  {
    this->rows.emplace_back<Row_Item>(line); 
    auto tid = this->rows.size() - 1;
    this->primaryKeys[this->rows[tid].i_id()] = tid;
  }
  
}

Row_Item& Item::getByPrimaryKey(Integer const& primaryKey)
{
  return this->rows[this->primaryKeys[primaryKey]];
}