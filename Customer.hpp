#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include "Types.hpp"
#include "Row.hpp"

class Row_Customer : public Row<Integer, Integer, Integer, Varchar<16>, Char<2>, Varchar<16>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Char<16>, Date, Char<2>, Numeric<12,2>, Numeric<4, 4>, Numeric<12,2>, Numeric<12,2>, Numeric<4, 0>, Numeric<4, 0>, Varchar<500>>
{
public:
  Row_Customer(const std::string& r) : Row<Integer, Integer, Integer, Varchar<16>, Char<2>, Varchar<16>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Char<16>, Date, Char<2>, Numeric<12,2>, Numeric<4, 4>, Numeric<12,2>, Numeric<12,2>, Numeric<4, 0>, Numeric<4, 0>, Varchar<500>>(r)
  {}
  
  inline Integer& c_id()
  {
    return std::get<0>(this->data); 
  }
  
  inline Integer& c_d_id()
  {
    return std::get<1>(this->data); 
  }
  
  inline Integer& c_w_id()
  {
    return std::get<2>(this->data); 
  }
  
  inline Varchar<16>& c_first()
  {
    return std::get<3>(this->data); 
  }
  
  inline Char<2>& c_middle()
  {
    return std::get<4>(this->data); 
  }
  
  inline Varchar<16>& c_last()
  {
    return std::get<5>(this->data); 
  }
  
  inline Varchar<20>& c_street_1()
  {
    return std::get<6>(this->data); 
  }
  
  
  inline Varchar<20>& c_street_2()
  {
    return std::get<7>(this->data); 
  }
  
  inline Varchar<20>& c_city()
  {
    return std::get<8>(this->data); 
  }
  
  inline Char<2>& c_state()
  {
    return std::get<9>(this->data); 
  }
  
  inline Char<9>& c_zip()
  {
    return std::get<10>(this->data); 
  }
  
  inline Char<16>& c_phone()
  {
    return std::get<11>(this->data); 
  }
  
  inline Date& c_since()
  {
    return std::get<12>(this->data); 
  }
  
  inline Char<2>& c_credit()
  {
    return std::get<13>(this->data); 
  }
  
  inline Numeric<12, 2>& c_credit_lim()
  {
    return std::get<14>(this->data); 
  }
  inline Numeric<4, 4>& c_discount()
  {
    return std::get<15>(this->data); 
  }
  
  inline Numeric<12, 2>& c_balance()
  {
    return std::get<16>(this->data); 
  }
  inline Numeric<12, 2>& c_ytd_payment()
  {
    return std::get<17>(this->data); 
  }
  inline Numeric<4, 0>& c_payment_cnt()
  {
    return std::get<18>(this->data); 
  }
  inline Numeric<4, 0>& c_delivery_cnt()
  {
    return std::get<19>(this->data); 
  }
  inline Varchar<500>& c_data()
  {
    return std::get<20>(this->data); 
  }
};

class Customer 
{
public:
  std::vector<Row_Customer> rows;
  
  Customer(std::string file);

};


#endif