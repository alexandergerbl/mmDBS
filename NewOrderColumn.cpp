#include "NewOrderColumn.hpp"

#include<fstream>
#include<experimental/filesystem>

Tid NewOrderColumn::getByPrimaryKey(const Integer& no_o_id, const Integer& no_d_id, const Integer& no_w_id) const
{
    return this->keys[std::make_tuple(no_o_id, no_d_id, no_w_id)];
}

std::size_t NewOrderColumn::size() const
{
    return std::get<0>(data).size();
}

void NewOrderColumn::insert(Integer no_o_id, Integer no_d_id, Integer no_w_id)
{
    auto tid = this->size();
    
    this->no_o_id().emplace_back(no_o_id);
    this->no_d_id().emplace_back(no_d_id);
    this->no_w_id().emplace_back(no_w_id);
    
    this->keys[std::make_tuple(no_o_id, no_d_id, no_w_id)] = tid;
}
