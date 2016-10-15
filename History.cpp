#include "History.hpp"

#include<fstream>
#include<experimental/filesystem>


History::History(std::string file)
{

  if(!std::experimental::filesystem::exists(file))
  {
    throw std::invalid_argument("Path to history data wrong!");
  }
  
  std::ifstream in { file };
  std::string line;
  
  while(std::getline(in, line))
  {
    this->rows.emplace_back<Row_History>(line); 
  }
  
}
