#include "Warehouse.hpp"

#include <experimental/filesystem>
#include <exception>
#include <fstream>
#include <string>
#include <memory>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "Types.hpp"

Warehouse::Warehouse(std::string path)
{
  namespace fs = std::experimental::filesystem;
  
  if(!fs::exists(path) )
  {
    throw std::invalid_argument("Path to Warehouse data wrong!\n");
  }
  
  std::ifstream in{path};
  std::string line;
 
  while(std::getline(in, line))
  {
    std::string att_value;
    std::stringstream ss{line};
    
    Integer w_id;
    std::getline(ss, att_value, '|');
    w_id = Integer::castString(att_value.c_str(), att_value.length());
    
    Varchar<10> w_name;
    std::getline(ss, att_value, '|');
    w_name = Varchar<10>::build(att_value.c_str());
    
    Varchar<20> w_street_1;
    std::getline(ss, att_value, '|');
    w_street_1 = Varchar<20>::build(att_value.c_str());
    
    Varchar<20> w_street_2;
    std::getline(ss, att_value, '|');
    w_street_2 = Varchar<20>::build(att_value.c_str());
    
    Varchar<20> w_city;
    std::getline(ss, att_value, '|');
    w_city = Varchar<20>::build(att_value.c_str());
    
    Char<2> w_state;
    std::getline(ss, att_value, '|');
    w_state = Char<2>::build(att_value.c_str());
    
    Char<9> w_zip;
    std::getline(ss, att_value, '|');
    w_zip = Char<9>::build(att_value.c_str());
    
    Numeric<4, 4> w_tax;
    std::getline(ss, att_value, '|');
    w_tax = Numeric<4,4>::castString (att_value.c_str(), att_value.length());
    
    Numeric<12,2> w_ytd;
    std::getline(ss, att_value, '|');
    w_ytd = Numeric<12,2>::castString (att_value.c_str(), att_value.length());
    
    
    entries.push_back(Warehouse_Entry{w_id, w_name, w_street_1, w_street_2, w_city, w_state, w_zip, w_tax, w_ytd});
    //for(int i = 0; i < 9; i++)
    {
      
      std::cout << w_id << "|" << w_name << "|" << w_street_1 << "|" << w_street_2 << "|" << w_city << "|" << w_state << "|" << w_zip << "|" << w_tax << "|" << w_ytd << std::endl;
    }
  }
  
      
    
    
    //
    
    //this->entries.push_back(Warehouse_Entry{1, 2, 3, 4, 5, 6, 7, 8, 9});
    
  
    
};
