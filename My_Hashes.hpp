#ifndef MY_HASHES_HPP
#define MY_HASHES_HPP

#include "Types.hpp"
#include <cstdlib>
#include <utility>
#include <tuple>
#include <functional>

//use better hash functions than XOR ! ! ! 
//looked up at http://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x


inline void hash_combine(std::size_t& seed) {}

template <typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, T const& v, Rest... rest)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed>>2);
    hash_combine(seed, rest...);    
}



struct IntHash
{
   std::size_t operator()(Integer const& i) const
   {
       return i.hash();
   }
};

struct IntIntHash
{
   std::size_t operator()(std::pair<Integer, Integer> const& p) const
   {
       std::size_t seed = p.first.hash();
       seed ^= p.second.hash() + 0x9e3779b9 + (seed << 6) + (seed>>2);
       return seed;
   }
};

struct IntIntIntHash
{
  std::size_t operator()(std::tuple<Integer, Integer, Integer> const& p) const 
  {
      std::size_t seed = std::get<0>(p).hash();
      seed ^= std::get<1>(p).hash() + 0x9e3779b9 + (seed << 6) + (seed>>2);
      seed ^= std::get<2>(p).hash() + 0x9e3779b9 + (seed << 6) + (seed>>2);
      return seed;
  }
};

struct IntIntIntIntHash
{
    //TODO XOR of only 2 parameters should also be fine - but check this 
  std::size_t operator()(std::tuple<Integer, Integer, Integer, Integer> const& p) const
  {
      std::size_t seed = std::get<0>(p).hash();
      seed ^= std::get<1>(p).hash() + 0x9e3779b9 + (seed << 6) + (seed>>2);
      seed ^= std::get<2>(p).hash() + 0x9e3779b9 + (seed << 6) + (seed>>2);
      seed ^= std::get<3>(p).hash() + 0x9e3779b9 + (seed << 6) + (seed>>2);
      return seed;
  }
};

#endif
