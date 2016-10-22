#include "OrderLineColumn.hpp"

#include<fstream>
#include<experimental/filesystem>


Tid OrderLineColumn::getByPrimaryKey(const Integer& ol_o_id, const Integer& ol_d_id, const Integer& ol_w_id, const Integer& ol_number) const
{
    return this->keys[std::make_tuple(ol_o_id, ol_d_id, ol_w_id, ol_number)];
}

std::size_t OrderLineColumn::size() const
{
    return std::get<0>(data).size();
}

void OrderLineColumn::insert(Integer ol_o_id, Integer ol_d_id, Integer ol_w_id, Integer ol_number, Integer ol_i_id, Integer ol_supply_w_id, Date ol_delivery_d, Numeric<2, 0> ol_quantity, Numeric<6, 2> ol_amount, Char<24> ol_dist_info)
{
    auto tid = this->size();
    
    this->ol_o_id().emplace_back(ol_o_id);
    this->ol_d_id().emplace_back(ol_d_id);
    this->ol_w_id().emplace_back(ol_w_id);
    this->ol_number().emplace_back(ol_number);
    this->ol_i_id().emplace_back(ol_i_id);
    this->ol_supply_w_id().emplace_back(ol_supply_w_id);
    this->ol_delivery_d().emplace_back(ol_delivery_d);
    this->ol_quantity().emplace_back(ol_quantity);
    this->ol_amount().emplace_back(ol_amount);
    this->ol_dist_info().emplace_back(ol_dist_info);
    
    this->keys[std::make_tuple(ol_o_id, ol_d_id, ol_w_id, ol_number)] = tid;
}
