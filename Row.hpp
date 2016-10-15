#ifndef ROW_HPP
#define ROW_HPP

#include "Types.hpp"

#include <string>
#include <vector>
#include <memory>
#include <experimental/any>
#include <experimental/filesystem>
#include <exception>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdint>


template<typename T>
inline auto readAttribute(std::string att_value)
{
  static_assert(sizeof(T) == 0, "Read used for unhandled type!\n");
};

template<>
inline auto readAttribute<Integer>(std::string att_value)
{
  return Integer::castString(att_value.c_str(), att_value.length());
}
/*
template<>
inline auto readAttribute<Date>(std::string att_value)
{
  return Date::castString(att_value.c_str(), att_value.length());
}
*/

template<>
inline auto readAttribute<Date>(std::string att_value)
{
  uint32_t date = atoi(att_value.c_str());
  return Date(date);
}

template<>
inline auto readAttribute<Varchar<10>>(std::string att_value)
{
  return Varchar<10>::build(att_value.c_str());
}

template<>
inline auto readAttribute<Varchar<16>>(std::string att_value)
{
  return Varchar<16>::build(att_value.c_str());
}

template<>
inline auto readAttribute<Varchar<20>>(std::string att_value)
{
  return Varchar<20>::build(att_value.c_str());
}
template<>
inline auto readAttribute<Varchar<24>>(std::string att_value)
{
  return Varchar<24>::build(att_value.c_str());
}

template<>
inline auto readAttribute<Varchar<50>>(std::string att_value)
{
  return Varchar<50>::build(att_value.c_str());
}

template<>
inline auto readAttribute<Varchar<500>>(std::string att_value)
{
  return Varchar<500>::build(att_value.c_str());
}

template<>
inline auto readAttribute<Char<2>>(std::string att_value)
{
  return Char<2>::build(att_value.c_str());
}

template<>
inline auto readAttribute<Char<9>>(std::string att_value)
{
  return Char<9>::build(att_value.c_str());
}

template<>
inline auto readAttribute<Char<16>>(std::string att_value)
{
  return Char<16>::build(att_value.c_str());
}

template<>
inline auto readAttribute<Char<24>>(std::string att_value)
{
  return Char<24>::build(att_value.c_str());
}

template<>
inline auto readAttribute<Numeric<1, 0>>(std::string att_value)
{
  return Numeric<1,0>::castString (att_value.c_str(), att_value.length());
}

template<>
inline auto readAttribute<Numeric<2, 0>>(std::string att_value)
{
  return Numeric<2,0>::castString (att_value.c_str(), att_value.length());
}

template<>
inline auto readAttribute<Numeric<4, 0>>(std::string att_value)
{
  return Numeric<4,0>::castString (att_value.c_str(), att_value.length());
}

template<>
inline auto readAttribute<Numeric<4, 4>>(std::string att_value)
{
  return Numeric<4,4>::castString (att_value.c_str(), att_value.length());
}

template<>
inline auto readAttribute<Numeric<5, 2>>(std::string att_value)
{
  return Numeric<5,2>::castString (att_value.c_str(), att_value.length());
}

template<>
inline auto readAttribute<Numeric<6, 2>>(std::string att_value)
{
  return Numeric<6,2>::castString (att_value.c_str(), att_value.length());
}

template<>
inline auto readAttribute<Numeric<8, 0>>(std::string att_value)
{
  return Numeric<8, 0>::castString (att_value.c_str(), att_value.length());
}

template<>
inline auto readAttribute<Numeric<12, 2>>(std::string att_value)
{
  return Numeric<12,2>::castString (att_value.c_str(), att_value.length());
}

template <typename... Types>
struct Row {
    std::tuple<Types...> data;
    std::stringstream ss;
 
    Row(std::string const& r) : ss{r} {
        readRow<0, Types...>();
    }
    
    Row(Types&&... types)
    { 
      this->data = std::make_tuple(std::forward<Types>(types)...);
    }
 
    template <int I>
    auto readRow() {
        static_assert(I == sizeof...(Types), "No further attributes!\n");
    }
 
    template <int I, typename Arg, typename... Args>
    auto readRow() {
        static_assert(I + 1 + sizeof...(Args) == sizeof...(Types), "Index Error!\n");
 
        std::string att_value;
        std::getline(ss, att_value, '|');
 
        std::get<I>(data) = readAttribute<Arg>(att_value);
 
        readRow<I + 1, Args...>();
    }
    
    //Row& operator[](Row_Attr attr)
    
};


#endif