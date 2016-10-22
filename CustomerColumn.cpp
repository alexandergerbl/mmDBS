#include "CustomerColumn.hpp"

#include<fstream>
#include<experimental/filesystem>

Tid CustomerColumn::getByPrimaryKey(const Integer& c_id, const Integer& c_d_id, const Integer& c_w_id) const
{
    return keys[std::make_tuple(c_id, c_d_id, c_w_id)];
}

std::size_t CustomerColumn::size() const
{
    return std::get<0>(data).size();
}
