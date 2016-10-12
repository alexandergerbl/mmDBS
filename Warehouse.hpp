#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "Types.hpp"
#include <string>
#include <vector>

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

class Warehouse
{
  std::vector<Warehouse_Entry> entries;
  
  Warehouse(std::string path); 
};

#endif