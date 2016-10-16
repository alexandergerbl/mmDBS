#if !defined(__cplusplus) || !defined(__GNUC__) 
#error "Wrong Compiler!" 
#endif

#include <iostream>
#include <chrono>

#include "Database.hpp"







int main()
{
  std::cout << "Main Memory Database System\n" << std::endl;

  Database db;
  
  db.printTask1Info(1000000);
  
  
  return 0;
}
