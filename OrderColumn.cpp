#include "OrderColumn.hpp"

#include<fstream>
#include<experimental/filesystem>



Tid OrderColumn::getByPrimaryKey(const Integer& o_id, const Integer& o_d_id, const Integer& o_w_id) const
{
    return this->keys[std::make_tuple(o_id, o_d_id, o_w_id)];
}

std::size_t OrderColumn::size() const
{
    return std::get<0>(data).size();
}

void OrderColumn::insert(Integer o_id, Integer o_d_id, Integer o_w_id, Integer o_c_id, Date o_entry_d, Integer o_carrier_id, Numeric<2, 0> o_ol_cnt, Numeric<1, 0> o_all_local)
{
    auto tid = this->o_id().size();
    
    this->o_id().emplace_back(o_id);
    this->o_d_id().emplace_back(o_d_id);
    this->o_w_id().emplace_back(o_w_id);
    this->o_c_id().emplace_back(o_c_id);
    this->o_entry_d().emplace_back(o_entry_d);
    this->o_carrier_id().emplace_back(o_carrier_id);
    this->o_ol_cnt().emplace_back(o_ol_cnt);
    this->o_all_local().emplace_back(o_all_local);
    
    this->keys[std::make_tuple(o_id, o_d_id, o_w_id)] = tid;
    
}
