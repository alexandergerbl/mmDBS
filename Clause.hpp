#ifndef CLAUSE_HPP
#define CLAUSE_HPP

#include <string>


struct Clause
{
   std::string table_name;
   std::string attribute;
   std::string table_name2;
   std::string value;
   //tells whether second value is a constant
   bool isConstant;
};
#endif
