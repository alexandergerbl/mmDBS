#ifndef HISTORYCOLUMN_HPP
#define HISTORYCOLUMN_HPP

#include "ColumnStore.hpp"



class HistoryColumn : ColumnStore<0, Integer, Integer, Integer, Integer, Integer, Date, Numeric<6, 2>, Varchar<24>> 
{
public:
  HistoryColumn(std::string file) : ColumnStore(file){}
  
  std::size_t size() const;
};
  

#endif
