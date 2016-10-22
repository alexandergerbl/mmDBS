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

#include <iostream>

template <typename... Types>
struct Row {
    std::tuple<Types...> data;
 
    char* token;
    
    Row(std::string const& r) {
        char s [r.length()];
        
        strcpy(s, r.c_str());
        
        token = std::strtok(s, "|");        
        
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
 
        std::get<I>(data) = readAttribute<Arg>(token);
        
        token = std::strtok(NULL, "|");
 
        readRow<I + 1, Args...>();
    }
    
    //Row& operator[](Row_Attr attr)
    
};


#endif
