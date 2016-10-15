#include "District.hpp"

#include<fstream>
#include<experimental/filesystem>


District::District(std::string file)
{

  if(!std::experimental::filesystem::exists(file))
  {
    throw std::invalid_argument("Path to district data wrong!");
  }
  
  std::ifstream in { file };
  std::string line;
  
  while(std::getline(in, line))
  {
    this->rows.emplace_back<Row_District>(line);
    this->primaryKey[std::make_pair(this->rows[this->rows.size()-1].d_w_id(), this->rows[this->rows.size()-1].d_id())] = this->rows.size() - 1;
  }
  
}

Row_District& District::getByPrimaryKey(std::pair<Integer, Integer> const& p_key)
{
  return this->rows[this->primaryKey[p_key]];
}