#ifndef CUSTOMERCOLUMN_HPP
#define CUSTOMERCOLUMN_HPP

#include "Types.hpp"
#include "ColumnStore.hpp"

#include "My_Hashes.hpp"

#include <unordered_map>
#include <tuple>



class CustomerColumn : public ColumnStore<3, Integer, Integer, Integer, Varchar<16>, Char<2>, Varchar<16>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Char<16>, Date, Char<2>, Numeric<12,2>, Numeric<4, 4>, Numeric<12,2>, Numeric<12,2>, Numeric<4, 0>, Numeric<4, 0>, Varchar<500>> 
{
  
public:
  
  CustomerColumn(std::string file) : ColumnStore(file){}

  Tid getByPrimaryKey(Integer const& c_id, Integer const& c_d_id, Integer const& c_w_id) const;
  
  std::size_t size() const;
  
   
  inline auto& c_id()
  {
    return std::get<0>(this->data); 
  }
  
  inline auto& c_d_id()
  {
    return std::get<1>(this->data); 
  }
  
  inline auto& c_w_id()
  {
    return std::get<2>(this->data); 
  }
  
  inline auto& c_first()
  {
    return std::get<3>(this->data); 
  }
  
  inline auto& c_middle()
  {
    return std::get<4>(this->data); 
  }
  
  inline auto& c_last()
  {
    return std::get<5>(this->data); 
  }
  
  inline auto& c_street_1()
  {
    return std::get<6>(this->data); 
  }
  
  
  inline auto& c_street_2()
  {
    return std::get<7>(this->data); 
  }
  
  inline auto& c_city()
  {
    return std::get<8>(this->data); 
  }
  
  inline auto& c_state()
  {
    return std::get<9>(this->data); 
  }
  
  inline auto& c_zip()
  {
    return std::get<10>(this->data); 
  }
  
  inline auto& c_phone()
  {
    return std::get<11>(this->data); 
  }
  
  inline auto& c_since()
  {
    return std::get<12>(this->data); 
  }
  
  inline auto& c_credit()
  {
    return std::get<13>(this->data); 
  }
  
  inline auto& c_credit_lim()
  {
    return std::get<14>(this->data); 
  }
  inline auto& c_discount()
  {
    return std::get<15>(this->data); 
  }
  
  inline auto& c_balance()
  {
    return std::get<16>(this->data); 
  }
  inline auto& c_ytd_payment()
  {
    return std::get<17>(this->data); 
  }
  inline auto& c_payment_cnt()
  {
    return std::get<18>(this->data); 
  }
  inline auto& c_delivery_cnt()
  {
    return std::get<19>(this->data); 
  }
  inline auto& c_data()
  {
    return std::get<20>(this->data); 
  }
};


#endif
