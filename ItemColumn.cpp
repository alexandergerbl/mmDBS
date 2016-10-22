#include "ItemColumn.hpp"

#include<fstream>
#include<experimental/filesystem>

Tid ItemColumn::getByPrimaryKey(const Integer& i_id) const
{
    return keys[std::make_tuple(i_id)];
}

std::size_t ItemColumn::size() const
{
  return std::get<0>(data).size();
}
