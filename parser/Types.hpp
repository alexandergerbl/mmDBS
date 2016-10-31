#ifndef H_Types_hpp
#define H_Types_hpp

#include <string>
#include <cmath>
#include <cstdint>
#include <string.h>
#include <algorithm>
#include <sstream>

/**
 * Types
 */
namespace Types {
   enum class Tag : unsigned {Integer, Char, Varchar, Timestamp, Numeric};
}


/**
 * Integer
 */
typedef int Integer;

/**
 * Timestamp
 */
struct Timestamp{
  uint64_t value;  
  void loadString(const std::string& str) {
    value = strtoll(str.c_str(), NULL, 10);
  }
  std::string toString(){
    std::ostringstream oss;
    oss << value;
   return oss.str();
  }
};


/**
 * Varchar
 */
template <unsigned len>
struct Varchar {
   char data[len];
   void loadString(const std::string& str);
   std::string toString();
};

template <unsigned len>
void Varchar<len>::loadString(const std::string& str) {
   if (str.size() >= len) {
      memcpy(data, str.c_str(), len);
   } else {
      memset(data, ' ', len);
      memcpy(data, str.c_str(), str.size());
   }
}

template <unsigned len>
std::string Varchar<len>::toString() {
   return std::string(data, data+len);
}


/**
 * Char
 */
template <unsigned len>
struct Char {
   char data[len];
   void loadString(const std::string& str);
   std::string toString();
};

template <unsigned len>
void Char<len>::loadString(const std::string& str) {
   if (str.size() >= len) {
      memcpy(data, str.c_str(), len);
   } else {
      memset(data, ' ', len);
      memcpy(data, str.c_str(), str.size());
   }
}

template <unsigned len>
std::string Char<len>::toString() {
   return std::string(data, data+len);
}

/**
 * Numeric
 */
template <unsigned len1, unsigned len2>
struct Numeric {
   uint64_t data;
   Numeric(uint64_t pre, uint64_t decimal);
   std::string toString()
   {
       return std::string(data);
   }
};

template <unsigned len1, unsigned len2>
Numeric<len1,len2>::Numeric(uint64_t pre, uint64_t decimal) {
   data = decimal + std::pow(10, len2)*pre;
}

#endif
