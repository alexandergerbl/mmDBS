#ifndef COLUMNSTORE_HPP
#define COLUMNSTORE_HPP

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
#include <array>
#include <iostream>
#include <unordered_map>

#include "My_Hashes.hpp"





template <class Tuple, std::size_t... Is>
auto function_impl(Tuple t, std::index_sequence<Is...>)
{
    std::size_t seed = 0;
    hash_combine(seed, std::get<Is>(t)...);
    return seed;
}

template<int numKeys, class Tuple>
auto function(Tuple t)
{
   return function_impl(t, std::make_index_sequence<numKeys>{}); 
}


//enter param pack with numKeys * Integer NO TUPLE HERE

template<int numKeys, typename Type>
struct KeyCompare
{
  std::size_t operator()(Type arg) const
  {
/*    std::size_t seed;
    hash_combine<Type...>(seed, arg...);
    
    std::cout << seed << std::endl;
  */  
  //  seed ^= std::get<1>(t).hash() + 0x9e3779b9 + (seed << 6) + (seed>>2);
  //  seed ^= std::get<2>(t).hash() + 0x9e3779b9 + (seed << 6) + (seed>>2);
  //  seed ^= std::get<3>(t).hash() + 0x9e3779b9 + (seed << 6) + (seed>>2);
    
    return function<numKeys>(arg);
  }
};

template<int numKeys, typename Type>
struct PrimaryKeys
{
  std::unordered_map<Type, Tid, KeyCompare<numKeys, Type>> keys; 
  
  Tid operator[](Type const&  t) const
  {
    return keys.at(t);
  }
  Tid& operator[](Type const&  t)
  {
    return keys[t];
  }
};


template <typename Types, std::size_t... I>
auto createPrimaryKey_impl(std::index_sequence<I...>, Types&& keys)
{
    //PrimaryKeys<typename std::tuple_element<I, std::tuple<Types...>>::type...> type;
    return std::make_tuple(std::get<I>(std::forward<Types>(keys))...);
}

template<int numKeys, typename T, typename Indices = std::make_index_sequence<numKeys>>
auto createPrimaryKey(T&& keys)
{
   return createPrimaryKey_impl(Indices(), std::forward<T>(keys)); 
}

//template<int numKeys, typename...> class TestKey,
template <int NumKeys, typename... Types>
struct ColumnStore {
    std::tuple<std::vector<Types>...> data;
    PrimaryKeys<NumKeys, decltype(createPrimaryKey<NumKeys>(std::declval<std::tuple<Types...>>()))> keys;
    char* token;
    
    ColumnStore(std::string const& file) {
        
        
        if(!std::experimental::filesystem::exists(file))
        {
            throw std::invalid_argument("Path to data-file wrong!");
        }

        std::ifstream in { file };
        std::string line;

        //createPrimaryKey();
        
        
        
        while(std::getline(in, line))
        {
            char s [line.length()];
            strcpy(s, line.c_str());
            token = std::strtok(s, "|");        
            auto tmp = addRow<0, Types...>();
            
            //add primaryKey entry
            auto tid = std::get<0>(data).size() - 1;              
            keys[createPrimaryKey<NumKeys>(tmp)] = tid; 
            
        }
        
    }
    
    ColumnStore(Types&&... types)
    { 
      this->data = std::make_tuple(std::forward<Types>(types)...);
    }
 
    template <int I>
    auto addRow() {
        static_assert(I == sizeof...(Types), "No further attributes!\n");
        
        return std::make_tuple();
    }
 
    template <int I, typename Arg, typename... Args>
    auto addRow() {
        static_assert(I + 1 + sizeof...(Args) == sizeof...(Types), "Index Error!\n");
 
        auto columnVal = readAttribute<Arg>(token);
        std::get<I>(data).emplace_back(columnVal);
        
        token = std::strtok(NULL, "|");
 
        return std::tuple_cat(std::make_tuple(columnVal) , addRow<I + 1, Args...>());
    }
    
    //Row& operator[](Row_Attr attr)
    
};


#endif
