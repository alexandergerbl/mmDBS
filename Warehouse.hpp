#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "Types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <experimental/any>
#include <experimental/filesystem>
#include <exception>
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "Types.hpp"


struct Warehouse_Entry
{
  Integer w_id;
  Varchar<10> w_name;
  Varchar<20> w_street_1;
  Varchar<20> w_street_2;
  Varchar<20> w_city;
  Char<2> w_state;
  Char<9> w_zip;
  Numeric<4, 4> w_tax;
  Numeric<12,2> w_ytd;
};
//All types
enum Types_E
{
  INTEGER,
  DATE,
  VARCHAR_10,
  VARCHAR_16,
  VARCHAR_20,
  VARCHAR_500,
  CHAR_2,
  CHAR_9,
  CHAR_16,
  NUMERIC_4,
  NUMERIC_4_4,
  NUMERIC_12_2
};

//choose type at compile-time
template<Types_E value>
using my_test_Type = std::conditional<(value == Types_E::INTEGER), Integer, std::conditional<(value == Types_E::VARCHAR_10), Varchar<10>, std::conditional<(value == Types_E::VARCHAR_20), Varchar<20>, std::conditional<(value == Types_E::CHAR_2), Char<2>, std::conditional<(value == Types_E::CHAR_9), Char<9>, std::conditional<(value == Types_E::NUMERIC_4_4), Numeric<4, 4>, std::conditional<(value == Types_E::NUMERIC_12_2), Numeric<12, 2>, std::conditional<(value == Types_E::NUMERIC_4), Numeric<4, 0>, std::conditional<(value == Types_E::VARCHAR_16), Varchar<16>, std::conditional<(value == Types_E::CHAR_9), Char<9>, std::conditional<(value == Types_E::VARCHAR_500), Varchar<500>, Date>>>>>>>>>> >;
//std::conditional<(value == Types_E::VARCHAR_500), Varchar<500>, Date> 

//Base Class of all tables
template<int N>
class BaseType
{  
public:
  std::array<Types_E, N> attributes;
  std::vector<std::array<std::experimental::any, N>> tableEntries;
  
  template<typename ...Args>
  BaseType(std::string path, Args&&... args) : attributes{Types_E(args)...}
  {
    //if file exists
    namespace fs = std::experimental::filesystem;
    
    if(!fs::exists(path) )
    {
      throw std::invalid_argument("Path to Warehouse data wrong!\n");
    }
    
    std::ifstream in{path};
    std::string line;
    
    //read lines
    while(std::getline(in, line))
    {
      std::string att_value;
      std::stringstream ss{line};
      
      std::array<std::experimental::any, N> tmp_row;
      
      //get infos of each line
      for(auto i = 0; i < attributes.size(); i++)
      {
	switch(attributes[i])
	{
	  case (Types_E::INTEGER):
	    //firstAttr = how to create INTEGERS
	    std::getline(ss, att_value, '|');
	    tmp_row[i] = std::experimental::any_cast<Integer>(Integer::castString(att_value.c_str(), att_value.length()));
	    //std::cout << std::experimental::any_cast<Integer>(tmp_row[i]) << std::endl;
	    break;
	  case (Types_E::DATE):
	    std::getline(ss, att_value, '|');
	    std::cout << att_value << std::endl;
	    tmp_row[i] = Date::castString(att_value.c_str(), att_value.length());
	    break;
	  case (Types_E::VARCHAR_10):
	    //firstAttr = how to create VARCHAR<10>
	    std::getline(ss, att_value, '|');
	    tmp_row[i] = Varchar<10>::build(att_value.c_str());
	    break;
	  case (Types_E::VARCHAR_20):
	    //firstAttr = how to create VARCHAR<10>
	    std::getline(ss, att_value, '|');
	    tmp_row[i] = Varchar<20>::build(att_value.c_str());
	    break;
	  case (Types_E::VARCHAR_16):
	    //firstAttr = how to create VARCHAR<10>
	    std::getline(ss, att_value, '|');
	    tmp_row[i] = Varchar<16>::build(att_value.c_str());
	    break;
	  case (Types_E::VARCHAR_500):
	    //firstAttr = how to create VARCHAR<10>
	    std::getline(ss, att_value, '|');
	    tmp_row[i] = Varchar<500>::build(att_value.c_str());
	    break;
	  case (Types_E::CHAR_2):
	    //firstAttr = how to create VARCHAR<10>
	    std::getline(ss, att_value, '|');
	    tmp_row[i] = Char<2>::build(att_value.c_str());
	    break;
	  case (Types_E::CHAR_9):
	    std::getline(ss, att_value, '|');
	    tmp_row[i] = Char<9>::build(att_value.c_str());
	    break;
	  case (Types_E::CHAR_16):
	    //firstAttr = how to create VARCHAR<10>
	    std::getline(ss, att_value, '|');
	    tmp_row[i] = Char<16>::build(att_value.c_str());
	    break;
	  case (Types_E::NUMERIC_4_4):
	    std::getline(ss, att_value, '|');
	    tmp_row[i] = Numeric<4,4>::castString (att_value.c_str(), att_value.length());
	    break;
	  case (Types_E::NUMERIC_12_2):
	    std::getline(ss, att_value, '|');
	    tmp_row[i] = Numeric<12,2>::castString (att_value.c_str(), att_value.length());
	    break;
	  case (Types_E::NUMERIC_4):
	    std::getline(ss, att_value, '|');
	    tmp_row[i] = Numeric<4,0>::castString (att_value.c_str(), att_value.length());
	    break;
	default:
	  break;
	}
      }
      
      this->tableEntries.push_back(tmp_row);
    }
    
    if(in.is_open())
      in.close();
  }
};



class Warehouse
{
  std::vector<Warehouse_Entry> entries;
public:
  Warehouse(std::string path); 
};

#endif