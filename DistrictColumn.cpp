#include "DistrictColumn.hpp"

#include<fstream>
#include<experimental/filesystem>

Tid DistrictColumn::getByPrimaryKey(const Integer& d_id, const Integer& d_w_id) const
{
    return keys[std::make_tuple(d_id, d_w_id)];
}

std::size_t DistrictColumn::size() const
{
    return std::get<0>(data).size();
}
