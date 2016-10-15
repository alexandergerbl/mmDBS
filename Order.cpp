#include "Order.hpp"

#include<fstream>
#include<experimental/filesystem>


Order::Order(std::string file)
{

  if(!std::experimental::filesystem::exists(file))
  {
    throw std::invalid_argument("Path to order data wrong!");
  }
  
  std::ifstream in { file };
  std::string line;
  
  while(std::getline(in, line))
  {
    this->rows.emplace_back<Row_Order>(line); 
    auto tid = this->rows.size() - 1;
    this->primaryKeys[std::make_tuple(this->rows[tid].o_w_id(), this->rows[tid].o_d_id(), this->rows[tid].o_c_id())] = tid;
  }
};

Row_Order& Order::getByPrimaryKey(std::tuple<Integer, Integer, Integer> const& primaryKey)
{
  return this->rows[this->primaryKeys[primaryKey]];
}

void Order::addRow(Integer o_id, Integer o_d_id, Integer o_w_id, Integer o_c_id, Date o_entry_d, Integer o_carrier_id, Numeric<2, 0> o_ol_cnt, Numeric<1, 0> o_all_local)
{
  this->rows.emplace_back<Row_Order>(o_id, o_d_id, o_w_id, o_c_id, o_entry_d, o_carrier_id, o_ol_cnt, o_all_local); 
  auto tid = this->rows.size() - 1;
  this->primaryKeys[std::make_tuple(o_w_id, o_d_id, o_c_id)] = tid;
}