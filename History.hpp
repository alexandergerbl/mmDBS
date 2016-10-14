#ifndef HISTORY_HPP
#define HISTORY_HPP

#include "Row.hpp"

class Row_History : public Row<Integer, Integer, Integer, Integer, Integer, Date, Numeric<6, 2>, Varchar<24>>
{
public:
  Row_History(std::string line) : Row<Integer, Integer, Integer, Integer, Integer, Date, Numeric<6, 2>, Varchar<24>>(line)
  {}
  
  inline Integer& h_c_id()
  {
    return std::get<0>(this->data);     
  }
  
  inline Integer& h_c_d_id()
  {
    return std::get<1>(this->data);     
  }
  
  inline Integer& h_c_w_id()
  {
    return std::get<2>(this->data);     
  }
  
  inline Integer& h_d_id()
  {
    return std::get<3>(this->data);     
  }
  
  inline Integer& h_w_id()
  {
    return std::get<4>(this->data);     
  }
  
  inline Date& h_date()
  {
    return std::get<5>(this->data);     
  }
  
  inline Numeric<6, 2>& h_amount()
  {
    return std::get<6>(this->data);     
  }
  
  inline Varchar<24>& h_data()
  {
    return std::get<7>(this->data);     
  }
  
};

class History 
{
public:
  std::vector<Row_History> rows;
  
  History(std::string file);
};
  

#endif