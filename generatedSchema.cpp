#include "AlgebraOperator.hpp"
#include "ColumnStore.hpp"
#include "Clause.hpp"
#include <cstdlib>
#include <tuple>
#include<chrono>
#include<cmath>
#include<map>
#include <utility>
#include <iterator>
#include <atomic>
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <exception>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <utility>
#include <memory>
#include <map>
#include <unordered_map>
#include <typeinfo>
#include <exception>
#include <set>

class warehouse : public ColumnStore<Integer, Varchar<10>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Numeric<4, 4>, Numeric<12, 2>>
{

std::unordered_map<std::tuple<Integer>, Tid, KeyCompare<1, std::tuple<Integer>>> keys;

public:
	warehouse(std::string file) : ColumnStore(file)
	{
		for(auto i = 0; i < this->size(); i++)
		{
			this->keys.emplace(std::make_tuple(std::get<0>(this->data)[i]), i);
		}

	}

	inline auto& w_id(){   return std::get<0>(this->data); }
	inline auto& w_name(){   return std::get<1>(this->data); }
	inline auto& w_street_1(){   return std::get<2>(this->data); }
	inline auto& w_street_2(){   return std::get<3>(this->data); }
	inline auto& w_city(){   return std::get<4>(this->data); }
	inline auto& w_state(){   return std::get<5>(this->data); }
	inline auto& w_zip(){   return std::get<6>(this->data); }
	inline auto& w_tax(){   return std::get<7>(this->data); }
	inline auto& w_ytd(){   return std::get<8>(this->data); }

	Tid find(const Integer& w_id) 
	{
		return this->keys[std::make_tuple(w_id)];
	}
	std::size_t size() const
	{
		return std::get<0>(data).size();
	}

	void insert(Integer w_id, Varchar<10> w_name, Varchar<20> w_street_1, Varchar<20> w_street_2, Varchar<20> w_city, Char<2> w_state, Char<9> w_zip, Numeric<4, 4> w_tax, Numeric<12, 2> w_ytd)
	{
		auto tid = this->size();

		//if(this->keys.count(std::make_tuple(w_id)) == 1)
			//throw std::invalid_argument("In relation warehouse: primary key already taken -> must be unique!");

		this->w_id().emplace_back(w_id);
		this->w_name().emplace_back(w_name);
		this->w_street_1().emplace_back(w_street_1);
		this->w_street_2().emplace_back(w_street_2);
		this->w_city().emplace_back(w_city);
		this->w_state().emplace_back(w_state);
		this->w_zip().emplace_back(w_zip);
		this->w_tax().emplace_back(w_tax);
		this->w_ytd().emplace_back(w_ytd);

		this->keys[std::make_tuple(w_id)] = tid;
	}

	void deleteEntry(const Integer& w_id)
	{
		auto tid = this->find(w_id);

		std::iter_swap(this->w_id().begin()+tid, this->w_id().end()-1);
		this->w_id().pop_back();
		std::iter_swap(this->w_name().begin()+tid, this->w_name().end()-1);
		this->w_name().pop_back();
		std::iter_swap(this->w_street_1().begin()+tid, this->w_street_1().end()-1);
		this->w_street_1().pop_back();
		std::iter_swap(this->w_street_2().begin()+tid, this->w_street_2().end()-1);
		this->w_street_2().pop_back();
		std::iter_swap(this->w_city().begin()+tid, this->w_city().end()-1);
		this->w_city().pop_back();
		std::iter_swap(this->w_state().begin()+tid, this->w_state().end()-1);
		this->w_state().pop_back();
		std::iter_swap(this->w_zip().begin()+tid, this->w_zip().end()-1);
		this->w_zip().pop_back();
		std::iter_swap(this->w_tax().begin()+tid, this->w_tax().end()-1);
		this->w_tax().pop_back();
		std::iter_swap(this->w_ytd().begin()+tid, this->w_ytd().end()-1);
		this->w_ytd().pop_back();

		this->keys.erase(std::make_tuple(w_id));

		this->keys[std::make_tuple(this->w_id()[tid])] = tid;

	}

};

class district : public ColumnStore<Integer, Integer, Varchar<10>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Numeric<4, 4>, Numeric<12, 2>, Integer>
{

std::unordered_map<std::tuple<Integer, Integer>, Tid, KeyCompare<2, std::tuple<Integer, Integer>>> keys;

public:
	district(std::string file) : ColumnStore(file)
	{
		for(auto i = 0; i < this->size(); i++)
		{
			this->keys.emplace(std::make_tuple(std::get<0>(this->data)[i], std::get<1>(this->data)[i]), i);
		}

	}

	inline auto& d_id(){   return std::get<0>(this->data); }
	inline auto& d_w_id(){   return std::get<1>(this->data); }
	inline auto& d_name(){   return std::get<2>(this->data); }
	inline auto& d_street_1(){   return std::get<3>(this->data); }
	inline auto& d_street_2(){   return std::get<4>(this->data); }
	inline auto& d_city(){   return std::get<5>(this->data); }
	inline auto& d_state(){   return std::get<6>(this->data); }
	inline auto& d_zip(){   return std::get<7>(this->data); }
	inline auto& d_tax(){   return std::get<8>(this->data); }
	inline auto& d_ytd(){   return std::get<9>(this->data); }
	inline auto& d_next_o_id(){   return std::get<10>(this->data); }

	Tid find(const Integer& d_id, const Integer& d_w_id) 
	{
		return this->keys[std::make_tuple(d_id, d_w_id)];
	}
	std::size_t size() const
	{
		return std::get<0>(data).size();
	}

	void insert(Integer d_id, Integer d_w_id, Varchar<10> d_name, Varchar<20> d_street_1, Varchar<20> d_street_2, Varchar<20> d_city, Char<2> d_state, Char<9> d_zip, Numeric<4, 4> d_tax, Numeric<12, 2> d_ytd, Integer d_next_o_id)
	{
		auto tid = this->size();

		//if(this->keys.count(std::make_tuple(d_id, d_w_id)) == 1)
			//throw std::invalid_argument("In relation district: primary key already taken -> must be unique!");

		this->d_id().emplace_back(d_id);
		this->d_w_id().emplace_back(d_w_id);
		this->d_name().emplace_back(d_name);
		this->d_street_1().emplace_back(d_street_1);
		this->d_street_2().emplace_back(d_street_2);
		this->d_city().emplace_back(d_city);
		this->d_state().emplace_back(d_state);
		this->d_zip().emplace_back(d_zip);
		this->d_tax().emplace_back(d_tax);
		this->d_ytd().emplace_back(d_ytd);
		this->d_next_o_id().emplace_back(d_next_o_id);

		this->keys[std::make_tuple(d_id, d_w_id)] = tid;
	}

	void deleteEntry(const Integer& d_id, const Integer& d_w_id)
	{
		auto tid = this->find(d_id, d_w_id);

		std::iter_swap(this->d_id().begin()+tid, this->d_id().end()-1);
		this->d_id().pop_back();
		std::iter_swap(this->d_w_id().begin()+tid, this->d_w_id().end()-1);
		this->d_w_id().pop_back();
		std::iter_swap(this->d_name().begin()+tid, this->d_name().end()-1);
		this->d_name().pop_back();
		std::iter_swap(this->d_street_1().begin()+tid, this->d_street_1().end()-1);
		this->d_street_1().pop_back();
		std::iter_swap(this->d_street_2().begin()+tid, this->d_street_2().end()-1);
		this->d_street_2().pop_back();
		std::iter_swap(this->d_city().begin()+tid, this->d_city().end()-1);
		this->d_city().pop_back();
		std::iter_swap(this->d_state().begin()+tid, this->d_state().end()-1);
		this->d_state().pop_back();
		std::iter_swap(this->d_zip().begin()+tid, this->d_zip().end()-1);
		this->d_zip().pop_back();
		std::iter_swap(this->d_tax().begin()+tid, this->d_tax().end()-1);
		this->d_tax().pop_back();
		std::iter_swap(this->d_ytd().begin()+tid, this->d_ytd().end()-1);
		this->d_ytd().pop_back();
		std::iter_swap(this->d_next_o_id().begin()+tid, this->d_next_o_id().end()-1);
		this->d_next_o_id().pop_back();

		this->keys.erase(std::make_tuple(d_id, d_w_id));

		this->keys[std::make_tuple(this->d_id()[tid], this->d_w_id()[tid])] = tid;

	}

};

class customer : public ColumnStore<Integer, Integer, Integer, Varchar<16>, Char<2>, Varchar<16>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Char<16>, Timestamp, Char<2>, Numeric<12, 2>, Numeric<4, 4>, Numeric<12, 2>, Numeric<12, 2>, Numeric<4, 0>, Numeric<4, 0>, Varchar<500>>
{

std::unordered_map<std::tuple<Integer, Integer, Integer>, Tid, KeyCompare<3, std::tuple<Integer, Integer, Integer>>> keys;


std::multimap<std::tuple<Integer, Integer, Varchar<16>, Varchar<16>>, Tid> customer_wdl;

public:
	customer(std::string file) : ColumnStore(file)
	{
		for(auto i = 0; i < this->size(); i++)
		{
			this->keys.emplace(std::make_tuple(std::get<0>(this->data)[i], std::get<1>(this->data)[i], std::get<2>(this->data)[i]), i);
		}
		for(auto i = 0; i < this->size(); i++)
		{
			this->customer_wdl.emplace(std::make_tuple(std::get<2>(this->data)[i], std::get<1>(this->data)[i], std::get<5>(this->data)[i], std::get<3>(this->data)[i]), i);
		}
	}

	inline auto& c_id(){   return std::get<0>(this->data); }
	inline auto& c_d_id(){   return std::get<1>(this->data); }
	inline auto& c_w_id(){   return std::get<2>(this->data); }
	inline auto& c_first(){   return std::get<3>(this->data); }
	inline auto& c_middle(){   return std::get<4>(this->data); }
	inline auto& c_last(){   return std::get<5>(this->data); }
	inline auto& c_street_1(){   return std::get<6>(this->data); }
	inline auto& c_street_2(){   return std::get<7>(this->data); }
	inline auto& c_city(){   return std::get<8>(this->data); }
	inline auto& c_state(){   return std::get<9>(this->data); }
	inline auto& c_zip(){   return std::get<10>(this->data); }
	inline auto& c_phone(){   return std::get<11>(this->data); }
	inline auto& c_since(){   return std::get<12>(this->data); }
	inline auto& c_credit(){   return std::get<13>(this->data); }
	inline auto& c_credit_lim(){   return std::get<14>(this->data); }
	inline auto& c_discount(){   return std::get<15>(this->data); }
	inline auto& c_balance(){   return std::get<16>(this->data); }
	inline auto& c_ytd_paymenr(){   return std::get<17>(this->data); }
	inline auto& c_payment_cnt(){   return std::get<18>(this->data); }
	inline auto& c_delivery_cnt(){   return std::get<19>(this->data); }
	inline auto& c_data(){   return std::get<20>(this->data); }

	Tid find(const Integer& c_id, const Integer& c_d_id, const Integer& c_w_id) 
	{
		return this->keys[std::make_tuple(c_id, c_d_id, c_w_id)];
	}
	std::size_t size() const
	{
		return std::get<0>(data).size();
	}

	void insert(Integer c_id, Integer c_d_id, Integer c_w_id, Varchar<16> c_first, Char<2> c_middle, Varchar<16> c_last, Varchar<20> c_street_1, Varchar<20> c_street_2, Varchar<20> c_city, Char<2> c_state, Char<9> c_zip, Char<16> c_phone, Timestamp c_since, Char<2> c_credit, Numeric<12, 2> c_credit_lim, Numeric<4, 4> c_discount, Numeric<12, 2> c_balance, Numeric<12, 2> c_ytd_paymenr, Numeric<4, 0> c_payment_cnt, Numeric<4, 0> c_delivery_cnt, Varchar<500> c_data)
	{
		auto tid = this->size();

		//if(this->keys.count(std::make_tuple(c_id, c_d_id, c_w_id)) == 1)
			//throw std::invalid_argument("In relation customer: primary key already taken -> must be unique!");

		this->c_id().emplace_back(c_id);
		this->c_d_id().emplace_back(c_d_id);
		this->c_w_id().emplace_back(c_w_id);
		this->c_first().emplace_back(c_first);
		this->c_middle().emplace_back(c_middle);
		this->c_last().emplace_back(c_last);
		this->c_street_1().emplace_back(c_street_1);
		this->c_street_2().emplace_back(c_street_2);
		this->c_city().emplace_back(c_city);
		this->c_state().emplace_back(c_state);
		this->c_zip().emplace_back(c_zip);
		this->c_phone().emplace_back(c_phone);
		this->c_since().emplace_back(c_since);
		this->c_credit().emplace_back(c_credit);
		this->c_credit_lim().emplace_back(c_credit_lim);
		this->c_discount().emplace_back(c_discount);
		this->c_balance().emplace_back(c_balance);
		this->c_ytd_paymenr().emplace_back(c_ytd_paymenr);
		this->c_payment_cnt().emplace_back(c_payment_cnt);
		this->c_delivery_cnt().emplace_back(c_delivery_cnt);
		this->c_data().emplace_back(c_data);

		this->keys[std::make_tuple(c_id, c_d_id, c_w_id)] = tid;

		this->customer_wdl.emplace(std::make_tuple(c_w_id, c_d_id, c_last, c_first), tid);
	}

	void deleteEntry(const Integer& c_id, const Integer& c_d_id, const Integer& c_w_id)
	{
		auto tid = this->find(c_id, c_d_id, c_w_id);


		//remove index from nonprimarykey before deletion
		auto iterpair = this->customer_wdl.equal_range(std::make_tuple(std::get<2>(this->data)[tid], std::get<1>(this->data)[tid], std::get<5>(this->data)[tid], std::get<3>(this->data)[tid]));
		auto it = iterpair.first;
        for (; it != iterpair.second; ++it) {
            if (it->second == tid) { 
                this->customer_wdl.erase(it);
                break;
            }
        }
		std::iter_swap(this->c_id().begin()+tid, this->c_id().end()-1);
		this->c_id().pop_back();
		std::iter_swap(this->c_d_id().begin()+tid, this->c_d_id().end()-1);
		this->c_d_id().pop_back();
		std::iter_swap(this->c_w_id().begin()+tid, this->c_w_id().end()-1);
		this->c_w_id().pop_back();
		std::iter_swap(this->c_first().begin()+tid, this->c_first().end()-1);
		this->c_first().pop_back();
		std::iter_swap(this->c_middle().begin()+tid, this->c_middle().end()-1);
		this->c_middle().pop_back();
		std::iter_swap(this->c_last().begin()+tid, this->c_last().end()-1);
		this->c_last().pop_back();
		std::iter_swap(this->c_street_1().begin()+tid, this->c_street_1().end()-1);
		this->c_street_1().pop_back();
		std::iter_swap(this->c_street_2().begin()+tid, this->c_street_2().end()-1);
		this->c_street_2().pop_back();
		std::iter_swap(this->c_city().begin()+tid, this->c_city().end()-1);
		this->c_city().pop_back();
		std::iter_swap(this->c_state().begin()+tid, this->c_state().end()-1);
		this->c_state().pop_back();
		std::iter_swap(this->c_zip().begin()+tid, this->c_zip().end()-1);
		this->c_zip().pop_back();
		std::iter_swap(this->c_phone().begin()+tid, this->c_phone().end()-1);
		this->c_phone().pop_back();
		std::iter_swap(this->c_since().begin()+tid, this->c_since().end()-1);
		this->c_since().pop_back();
		std::iter_swap(this->c_credit().begin()+tid, this->c_credit().end()-1);
		this->c_credit().pop_back();
		std::iter_swap(this->c_credit_lim().begin()+tid, this->c_credit_lim().end()-1);
		this->c_credit_lim().pop_back();
		std::iter_swap(this->c_discount().begin()+tid, this->c_discount().end()-1);
		this->c_discount().pop_back();
		std::iter_swap(this->c_balance().begin()+tid, this->c_balance().end()-1);
		this->c_balance().pop_back();
		std::iter_swap(this->c_ytd_paymenr().begin()+tid, this->c_ytd_paymenr().end()-1);
		this->c_ytd_paymenr().pop_back();
		std::iter_swap(this->c_payment_cnt().begin()+tid, this->c_payment_cnt().end()-1);
		this->c_payment_cnt().pop_back();
		std::iter_swap(this->c_delivery_cnt().begin()+tid, this->c_delivery_cnt().end()-1);
		this->c_delivery_cnt().pop_back();
		std::iter_swap(this->c_data().begin()+tid, this->c_data().end()-1);
		this->c_data().pop_back();

		this->keys.erase(std::make_tuple(c_id, c_d_id, c_w_id));

		this->keys[std::make_tuple(this->c_id()[tid], this->c_d_id()[tid], this->c_w_id()[tid])] = tid;


		auto iterpair2 = this->customer_wdl.equal_range(std::make_tuple(std::get<2>(this->data)[tid], std::get<1>(this->data)[tid], std::get<5>(this->data)[tid], std::get<3>(this->data)[tid]));
		auto it2 = iterpair2.first;
        for (; it2 != iterpair2.second; ++it2) {
            if (it2->second == this->size()) { 
                it2->second = tid;
                break;
            }
        }
	}

};

class history : public ColumnStore<Integer, Integer, Integer, Integer, Integer, Timestamp, Numeric<6, 2>, Varchar<24>>
{
public:
	history(std::string file) : ColumnStore(file)
	{

	}

	inline auto& h_c_id(){   return std::get<0>(this->data); }
	inline auto& h_c_d_id(){   return std::get<1>(this->data); }
	inline auto& h_c_w_id(){   return std::get<2>(this->data); }
	inline auto& h_d_id(){   return std::get<3>(this->data); }
	inline auto& h_w_id(){   return std::get<4>(this->data); }
	inline auto& h_date(){   return std::get<5>(this->data); }
	inline auto& h_amount(){   return std::get<6>(this->data); }
	inline auto& h_data(){   return std::get<7>(this->data); }

	std::size_t size() const
	{
		return std::get<0>(data).size();
	}

	void insert(Integer h_c_id, Integer h_c_d_id, Integer h_c_w_id, Integer h_d_id, Integer h_w_id, Timestamp h_date, Numeric<6, 2> h_amount, Varchar<24> h_data)
	{
		auto tid = this->size();

		//if(this->keys.count(std::make_tuple()) == 1)
			//throw std::invalid_argument("In relation history: primary key already taken -> must be unique!");

		this->h_c_id().emplace_back(h_c_id);
		this->h_c_d_id().emplace_back(h_c_d_id);
		this->h_c_w_id().emplace_back(h_c_w_id);
		this->h_d_id().emplace_back(h_d_id);
		this->h_w_id().emplace_back(h_w_id);
		this->h_date().emplace_back(h_date);
		this->h_amount().emplace_back(h_amount);
		this->h_data().emplace_back(h_data);
	}

};

class neworder : public ColumnStore<Integer, Integer, Integer>
{
public:

std::map<std::tuple<Integer, Integer, Integer>, Tid> keys;

public:
	neworder(std::string file) : ColumnStore(file)
	{
		for(auto i = 0; i < this->size(); i++)
		{
			this->keys.emplace(std::make_tuple(std::get<0>(this->data)[i], std::get<1>(this->data)[i], std::get<2>(this->data)[i]), i);
		}

	}

	inline auto& no_o_id(){   return std::get<0>(this->data); }
	inline auto& no_d_id(){   return std::get<1>(this->data); }
	inline auto& no_w_id(){   return std::get<2>(this->data); }

	Tid find(const Integer& no_o_id, const Integer& no_d_id, const Integer& no_w_id) 
	{
		return this->keys[std::make_tuple(no_o_id, no_d_id, no_w_id)];
	}
	std::size_t size() const
	{
		return std::get<0>(data).size();
	}

	void insert(Integer no_o_id, Integer no_d_id, Integer no_w_id)
	{
		auto tid = this->size();

		//if(this->keys.count(std::make_tuple(no_o_id, no_d_id, no_w_id)) == 1)
			//throw std::invalid_argument("In relation neworder: primary key already taken -> must be unique!");

		this->no_o_id().emplace_back(no_o_id);
		this->no_d_id().emplace_back(no_d_id);
		this->no_w_id().emplace_back(no_w_id);

		this->keys[std::make_tuple(no_o_id, no_d_id, no_w_id)] = tid;
	}

	void deleteEntry(const Integer& no_o_id, const Integer& no_d_id, const Integer& no_w_id)
	{
		auto tid = this->find(no_o_id, no_d_id, no_w_id);

		std::iter_swap(this->no_o_id().begin()+tid, this->no_o_id().end()-1);
		this->no_o_id().pop_back();
		std::iter_swap(this->no_d_id().begin()+tid, this->no_d_id().end()-1);
		this->no_d_id().pop_back();
		std::iter_swap(this->no_w_id().begin()+tid, this->no_w_id().end()-1);
		this->no_w_id().pop_back();

		this->keys.erase(std::make_tuple(no_o_id, no_d_id, no_w_id));

		this->keys[std::make_tuple(this->no_o_id()[tid], this->no_d_id()[tid], this->no_w_id()[tid])] = tid;

	}

};

class order : public ColumnStore<Integer, Integer, Integer, Integer, Timestamp, Integer, Numeric<2, 0>, Numeric<1, 0>>
{

std::unordered_map<std::tuple<Integer, Integer, Integer>, Tid, KeyCompare<3, std::tuple<Integer, Integer, Integer>>> keys;


std::multimap<std::tuple<Integer, Integer, Integer, Integer>, Tid> order_wdc;

public:
	order(std::string file) : ColumnStore(file)
	{
		for(auto i = 0; i < this->size(); i++)
		{
			this->keys.emplace(std::make_tuple(std::get<0>(this->data)[i], std::get<1>(this->data)[i], std::get<2>(this->data)[i]), i);
		}
		for(auto i = 0; i < this->size(); i++)
		{
			this->order_wdc.emplace(std::make_tuple(std::get<2>(this->data)[i], std::get<1>(this->data)[i], std::get<3>(this->data)[i], std::get<0>(this->data)[i]), i);
		}
	}

	inline auto& o_id(){   return std::get<0>(this->data); }
	inline auto& o_d_id(){   return std::get<1>(this->data); }
	inline auto& o_w_id(){   return std::get<2>(this->data); }
	inline auto& o_c_id(){   return std::get<3>(this->data); }
	inline auto& o_entry_d(){   return std::get<4>(this->data); }
	inline auto& o_carrier_id(){   return std::get<5>(this->data); }
	inline auto& o_ol_cnt(){   return std::get<6>(this->data); }
	inline auto& o_all_local(){   return std::get<7>(this->data); }

	Tid find(const Integer& o_id, const Integer& o_d_id, const Integer& o_w_id) 
	{
		return this->keys[std::make_tuple(o_id, o_d_id, o_w_id)];
	}
	std::size_t size() const
	{
		return std::get<0>(data).size();
	}

	void insert(Integer o_id, Integer o_d_id, Integer o_w_id, Integer o_c_id, Timestamp o_entry_d, Integer o_carrier_id, Numeric<2, 0> o_ol_cnt, Numeric<1, 0> o_all_local)
	{
		auto tid = this->size();

		//if(this->keys.count(std::make_tuple(o_id, o_d_id, o_w_id)) == 1)
			//throw std::invalid_argument("In relation order: primary key already taken -> must be unique!");

		this->o_id().emplace_back(o_id);
		this->o_d_id().emplace_back(o_d_id);
		this->o_w_id().emplace_back(o_w_id);
		this->o_c_id().emplace_back(o_c_id);
		this->o_entry_d().emplace_back(o_entry_d);
		this->o_carrier_id().emplace_back(o_carrier_id);
		this->o_ol_cnt().emplace_back(o_ol_cnt);
		this->o_all_local().emplace_back(o_all_local);

		this->keys[std::make_tuple(o_id, o_d_id, o_w_id)] = tid;

		this->order_wdc.emplace(std::make_tuple(o_w_id, o_d_id, o_c_id, o_id), tid);
	}

	void deleteEntry(const Integer& o_id, const Integer& o_d_id, const Integer& o_w_id)
	{
		auto tid = this->find(o_id, o_d_id, o_w_id);


		//remove index from nonprimarykey before deletion
		auto iterpair = this->order_wdc.equal_range(std::make_tuple(std::get<2>(this->data)[tid], std::get<1>(this->data)[tid], std::get<3>(this->data)[tid], std::get<0>(this->data)[tid]));
		auto it = iterpair.first;
        for (; it != iterpair.second; ++it) {
            if (it->second == tid) { 
                this->order_wdc.erase(it);
                break;
            }
        }
		std::iter_swap(this->o_id().begin()+tid, this->o_id().end()-1);
		this->o_id().pop_back();
		std::iter_swap(this->o_d_id().begin()+tid, this->o_d_id().end()-1);
		this->o_d_id().pop_back();
		std::iter_swap(this->o_w_id().begin()+tid, this->o_w_id().end()-1);
		this->o_w_id().pop_back();
		std::iter_swap(this->o_c_id().begin()+tid, this->o_c_id().end()-1);
		this->o_c_id().pop_back();
		std::iter_swap(this->o_entry_d().begin()+tid, this->o_entry_d().end()-1);
		this->o_entry_d().pop_back();
		std::iter_swap(this->o_carrier_id().begin()+tid, this->o_carrier_id().end()-1);
		this->o_carrier_id().pop_back();
		std::iter_swap(this->o_ol_cnt().begin()+tid, this->o_ol_cnt().end()-1);
		this->o_ol_cnt().pop_back();
		std::iter_swap(this->o_all_local().begin()+tid, this->o_all_local().end()-1);
		this->o_all_local().pop_back();

		this->keys.erase(std::make_tuple(o_id, o_d_id, o_w_id));

		this->keys[std::make_tuple(this->o_id()[tid], this->o_d_id()[tid], this->o_w_id()[tid])] = tid;


		auto iterpair2 = this->order_wdc.equal_range(std::make_tuple(std::get<2>(this->data)[tid], std::get<1>(this->data)[tid], std::get<3>(this->data)[tid], std::get<0>(this->data)[tid]));
		auto it2 = iterpair2.first;
        for (; it2 != iterpair2.second; ++it2) {
            if (it2->second == this->size()) { 
                it2->second = tid;
                break;
            }
        }
	}

};

class orderline : public ColumnStore<Integer, Integer, Integer, Integer, Integer, Integer, Timestamp, Numeric<2, 0>, Numeric<6, 2>, Char<24>>
{

std::unordered_map<std::tuple<Integer, Integer, Integer, Integer>, Tid, KeyCompare<4, std::tuple<Integer, Integer, Integer, Integer>>> keys;

public:
	orderline(std::string file) : ColumnStore(file)
	{
		for(auto i = 0; i < this->size(); i++)
		{
			this->keys.emplace(std::make_tuple(std::get<3>(this->data)[i], std::get<0>(this->data)[i], std::get<1>(this->data)[i], std::get<2>(this->data)[i]), i);
		}

	}

	inline auto& ol_o_id(){   return std::get<0>(this->data); }
	inline auto& ol_d_id(){   return std::get<1>(this->data); }
	inline auto& ol_w_id(){   return std::get<2>(this->data); }
	inline auto& ol_number(){   return std::get<3>(this->data); }
	inline auto& ol_i_id(){   return std::get<4>(this->data); }
	inline auto& ol_supply_w_id(){   return std::get<5>(this->data); }
	inline auto& ol_delivery_d(){   return std::get<6>(this->data); }
	inline auto& ol_quantity(){   return std::get<7>(this->data); }
	inline auto& ol_amount(){   return std::get<8>(this->data); }
	inline auto& ol_dist_info(){   return std::get<9>(this->data); }

	Tid find(const Integer& ol_number, const Integer& ol_o_id, const Integer& ol_d_id, const Integer& ol_w_id) 
	{
		return this->keys[std::make_tuple(ol_number, ol_o_id, ol_d_id, ol_w_id)];
	}
	std::size_t size() const
	{
		return std::get<0>(data).size();
	}

	void insert(Integer ol_o_id, Integer ol_d_id, Integer ol_w_id, Integer ol_number, Integer ol_i_id, Integer ol_supply_w_id, Timestamp ol_delivery_d, Numeric<2, 0> ol_quantity, Numeric<6, 2> ol_amount, Char<24> ol_dist_info)
	{
		auto tid = this->size();

		//if(this->keys.count(std::make_tuple(ol_number, ol_o_id, ol_d_id, ol_w_id)) == 1)
			//throw std::invalid_argument("In relation orderline: primary key already taken -> must be unique!");

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

		this->keys[std::make_tuple(ol_number, ol_o_id, ol_d_id, ol_w_id)] = tid;
	}

	void deleteEntry(const Integer& ol_number, const Integer& ol_o_id, const Integer& ol_d_id, const Integer& ol_w_id)
	{
		auto tid = this->find(ol_number, ol_o_id, ol_d_id, ol_w_id);

		std::iter_swap(this->ol_o_id().begin()+tid, this->ol_o_id().end()-1);
		this->ol_o_id().pop_back();
		std::iter_swap(this->ol_d_id().begin()+tid, this->ol_d_id().end()-1);
		this->ol_d_id().pop_back();
		std::iter_swap(this->ol_w_id().begin()+tid, this->ol_w_id().end()-1);
		this->ol_w_id().pop_back();
		std::iter_swap(this->ol_number().begin()+tid, this->ol_number().end()-1);
		this->ol_number().pop_back();
		std::iter_swap(this->ol_i_id().begin()+tid, this->ol_i_id().end()-1);
		this->ol_i_id().pop_back();
		std::iter_swap(this->ol_supply_w_id().begin()+tid, this->ol_supply_w_id().end()-1);
		this->ol_supply_w_id().pop_back();
		std::iter_swap(this->ol_delivery_d().begin()+tid, this->ol_delivery_d().end()-1);
		this->ol_delivery_d().pop_back();
		std::iter_swap(this->ol_quantity().begin()+tid, this->ol_quantity().end()-1);
		this->ol_quantity().pop_back();
		std::iter_swap(this->ol_amount().begin()+tid, this->ol_amount().end()-1);
		this->ol_amount().pop_back();
		std::iter_swap(this->ol_dist_info().begin()+tid, this->ol_dist_info().end()-1);
		this->ol_dist_info().pop_back();

		this->keys.erase(std::make_tuple(ol_number, ol_o_id, ol_d_id, ol_w_id));

		this->keys[std::make_tuple(this->ol_number()[tid], this->ol_o_id()[tid], this->ol_d_id()[tid], this->ol_w_id()[tid])] = tid;

	}

};

class item : public ColumnStore<Integer, Integer, Varchar<24>, Numeric<5, 2>, Varchar<50>>
{

std::unordered_map<std::tuple<Integer>, Tid, KeyCompare<1, std::tuple<Integer>>> keys;

public:
	item(std::string file) : ColumnStore(file)
	{
		for(auto i = 0; i < this->size(); i++)
		{
			this->keys.emplace(std::make_tuple(std::get<0>(this->data)[i]), i);
		}

	}

	inline auto& i_id(){   return std::get<0>(this->data); }
	inline auto& i_im_id(){   return std::get<1>(this->data); }
	inline auto& i_name(){   return std::get<2>(this->data); }
	inline auto& i_price(){   return std::get<3>(this->data); }
	inline auto& i_data(){   return std::get<4>(this->data); }

	Tid find(const Integer& i_id) 
	{
		return this->keys[std::make_tuple(i_id)];
	}
	std::size_t size() const
	{
		return std::get<0>(data).size();
	}

	void insert(Integer i_id, Integer i_im_id, Varchar<24> i_name, Numeric<5, 2> i_price, Varchar<50> i_data)
	{
		auto tid = this->size();

		//if(this->keys.count(std::make_tuple(i_id)) == 1)
			//throw std::invalid_argument("In relation item: primary key already taken -> must be unique!");

		this->i_id().emplace_back(i_id);
		this->i_im_id().emplace_back(i_im_id);
		this->i_name().emplace_back(i_name);
		this->i_price().emplace_back(i_price);
		this->i_data().emplace_back(i_data);

		this->keys[std::make_tuple(i_id)] = tid;
	}

	void deleteEntry(const Integer& i_id)
	{
		auto tid = this->find(i_id);

		std::iter_swap(this->i_id().begin()+tid, this->i_id().end()-1);
		this->i_id().pop_back();
		std::iter_swap(this->i_im_id().begin()+tid, this->i_im_id().end()-1);
		this->i_im_id().pop_back();
		std::iter_swap(this->i_name().begin()+tid, this->i_name().end()-1);
		this->i_name().pop_back();
		std::iter_swap(this->i_price().begin()+tid, this->i_price().end()-1);
		this->i_price().pop_back();
		std::iter_swap(this->i_data().begin()+tid, this->i_data().end()-1);
		this->i_data().pop_back();

		this->keys.erase(std::make_tuple(i_id));

		this->keys[std::make_tuple(this->i_id()[tid])] = tid;

	}

};

class stock : public ColumnStore<Integer, Integer, Numeric<4, 0>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Numeric<8, 0>, Numeric<4, 0>, Numeric<4, 0>, Varchar<50>>
{

std::unordered_map<std::tuple<Integer, Integer>, Tid, KeyCompare<2, std::tuple<Integer, Integer>>> keys;

public:
	stock(std::string file) : ColumnStore(file)
	{
		for(auto i = 0; i < this->size(); i++)
		{
			this->keys.emplace(std::make_tuple(std::get<0>(this->data)[i], std::get<1>(this->data)[i]), i);
		}

	}

	inline auto& s_i_id(){   return std::get<0>(this->data); }
	inline auto& s_w_id(){   return std::get<1>(this->data); }
	inline auto& s_quantity(){   return std::get<2>(this->data); }
	inline auto& s_dist_01(){   return std::get<3>(this->data); }
	inline auto& s_dist_02(){   return std::get<4>(this->data); }
	inline auto& s_dist_03(){   return std::get<5>(this->data); }
	inline auto& s_dist_04(){   return std::get<6>(this->data); }
	inline auto& s_dist_05(){   return std::get<7>(this->data); }
	inline auto& s_dist_06(){   return std::get<8>(this->data); }
	inline auto& s_dist_07(){   return std::get<9>(this->data); }
	inline auto& s_dist_08(){   return std::get<10>(this->data); }
	inline auto& s_dist_09(){   return std::get<11>(this->data); }
	inline auto& s_dist_10(){   return std::get<12>(this->data); }
	inline auto& s_ytd(){   return std::get<13>(this->data); }
	inline auto& s_order_cnt(){   return std::get<14>(this->data); }
	inline auto& s_remote_cnt(){   return std::get<15>(this->data); }
	inline auto& s_data(){   return std::get<16>(this->data); }

	Tid find(const Integer& s_i_id, const Integer& s_w_id) 
	{
		return this->keys[std::make_tuple(s_i_id, s_w_id)];
	}
	std::size_t size() const
	{
		return std::get<0>(data).size();
	}

	void insert(Integer s_i_id, Integer s_w_id, Numeric<4, 0> s_quantity, Char<24> s_dist_01, Char<24> s_dist_02, Char<24> s_dist_03, Char<24> s_dist_04, Char<24> s_dist_05, Char<24> s_dist_06, Char<24> s_dist_07, Char<24> s_dist_08, Char<24> s_dist_09, Char<24> s_dist_10, Numeric<8, 0> s_ytd, Numeric<4, 0> s_order_cnt, Numeric<4, 0> s_remote_cnt, Varchar<50> s_data)
	{
		auto tid = this->size();

		//if(this->keys.count(std::make_tuple(s_i_id, s_w_id)) == 1)
			//throw std::invalid_argument("In relation stock: primary key already taken -> must be unique!");

		this->s_i_id().emplace_back(s_i_id);
		this->s_w_id().emplace_back(s_w_id);
		this->s_quantity().emplace_back(s_quantity);
		this->s_dist_01().emplace_back(s_dist_01);
		this->s_dist_02().emplace_back(s_dist_02);
		this->s_dist_03().emplace_back(s_dist_03);
		this->s_dist_04().emplace_back(s_dist_04);
		this->s_dist_05().emplace_back(s_dist_05);
		this->s_dist_06().emplace_back(s_dist_06);
		this->s_dist_07().emplace_back(s_dist_07);
		this->s_dist_08().emplace_back(s_dist_08);
		this->s_dist_09().emplace_back(s_dist_09);
		this->s_dist_10().emplace_back(s_dist_10);
		this->s_ytd().emplace_back(s_ytd);
		this->s_order_cnt().emplace_back(s_order_cnt);
		this->s_remote_cnt().emplace_back(s_remote_cnt);
		this->s_data().emplace_back(s_data);

		this->keys[std::make_tuple(s_i_id, s_w_id)] = tid;
	}

	void deleteEntry(const Integer& s_i_id, const Integer& s_w_id)
	{
		auto tid = this->find(s_i_id, s_w_id);

		std::iter_swap(this->s_i_id().begin()+tid, this->s_i_id().end()-1);
		this->s_i_id().pop_back();
		std::iter_swap(this->s_w_id().begin()+tid, this->s_w_id().end()-1);
		this->s_w_id().pop_back();
		std::iter_swap(this->s_quantity().begin()+tid, this->s_quantity().end()-1);
		this->s_quantity().pop_back();
		std::iter_swap(this->s_dist_01().begin()+tid, this->s_dist_01().end()-1);
		this->s_dist_01().pop_back();
		std::iter_swap(this->s_dist_02().begin()+tid, this->s_dist_02().end()-1);
		this->s_dist_02().pop_back();
		std::iter_swap(this->s_dist_03().begin()+tid, this->s_dist_03().end()-1);
		this->s_dist_03().pop_back();
		std::iter_swap(this->s_dist_04().begin()+tid, this->s_dist_04().end()-1);
		this->s_dist_04().pop_back();
		std::iter_swap(this->s_dist_05().begin()+tid, this->s_dist_05().end()-1);
		this->s_dist_05().pop_back();
		std::iter_swap(this->s_dist_06().begin()+tid, this->s_dist_06().end()-1);
		this->s_dist_06().pop_back();
		std::iter_swap(this->s_dist_07().begin()+tid, this->s_dist_07().end()-1);
		this->s_dist_07().pop_back();
		std::iter_swap(this->s_dist_08().begin()+tid, this->s_dist_08().end()-1);
		this->s_dist_08().pop_back();
		std::iter_swap(this->s_dist_09().begin()+tid, this->s_dist_09().end()-1);
		this->s_dist_09().pop_back();
		std::iter_swap(this->s_dist_10().begin()+tid, this->s_dist_10().end()-1);
		this->s_dist_10().pop_back();
		std::iter_swap(this->s_ytd().begin()+tid, this->s_ytd().end()-1);
		this->s_ytd().pop_back();
		std::iter_swap(this->s_order_cnt().begin()+tid, this->s_order_cnt().end()-1);
		this->s_order_cnt().pop_back();
		std::iter_swap(this->s_remote_cnt().begin()+tid, this->s_remote_cnt().end()-1);
		this->s_remote_cnt().pop_back();
		std::iter_swap(this->s_data().begin()+tid, this->s_data().end()-1);
		this->s_data().pop_back();

		this->keys.erase(std::make_tuple(s_i_id, s_w_id));

		this->keys[std::make_tuple(this->s_i_id()[tid], this->s_w_id()[tid])] = tid;

	}

};

	static std::atomic<bool> childRunning_local;		
			
	static void SIGCHLD_handler(int /*sig*/) {		
	   int status;		
	   pid_t childPid=wait(&status);		
	   // now the child with process id childPid is dead		
	   childRunning_local=false;		
	}



class DatabaseColumn
{
public:
	warehouse m_warehouse{"./task1/tpcc_warehouse.tbl"};
	district m_district{"./task1/tpcc_district.tbl"};
	customer m_customer{"./task1/tpcc_customer.tbl"};
	history m_history{"./task1/tpcc_history.tbl"};
	neworder m_neworder{"./task1/tpcc_neworder.tbl"};
	order m_order{"./task1/tpcc_order.tbl"};
	orderline m_orderline{"./task1/tpcc_orderline.tbl"};
	item m_item{"./task1/tpcc_item.tbl"};
	stock m_stock{"./task1/tpcc_stock.tbl"};

const int32_t warehouses=5;

	DatabaseColumn(){
std::map<std::tuple<std::string, std::string>, int> getTableIndex;
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("warehouse", "w_id"), 0 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("warehouse", "w_name"), 1 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("warehouse", "w_street_1"), 2 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("warehouse", "w_street_2"), 3 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("warehouse", "w_city"), 4 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("warehouse", "w_state"), 5 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("warehouse", "w_zip"), 6 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("warehouse", "w_tax"), 7 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("warehouse", "w_ytd"), 8 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("district", "d_id"), 0 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("district", "d_w_id"), 1 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("district", "d_name"), 2 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("district", "d_street_1"), 3 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("district", "d_street_2"), 4 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("district", "d_city"), 5 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("district", "d_state"), 6 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("district", "d_zip"), 7 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("district", "d_tax"), 8 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("district", "d_ytd"), 9 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("district", "d_next_o_id"), 10 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_id"), 0 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_d_id"), 1 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_w_id"), 2 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_first"), 3 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_middle"), 4 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_last"), 5 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_street_1"), 6 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_street_2"), 7 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_city"), 8 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_state"), 9 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_zip"), 10 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_phone"), 11 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_since"), 12 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_credit"), 13 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_credit_lim"), 14 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_discount"), 15 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_balance"), 16 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_ytd_paymenr"), 17 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_payment_cnt"), 18 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_delivery_cnt"), 19 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("customer", "c_data"), 20 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("history", "h_c_id"), 0 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("history", "h_c_d_id"), 1 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("history", "h_c_w_id"), 2 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("history", "h_d_id"), 3 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("history", "h_w_id"), 4 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("history", "h_date"), 5 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("history", "h_amount"), 6 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("history", "h_data"), 7 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("neworder", "no_o_id"), 0 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("neworder", "no_d_id"), 1 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("neworder", "no_w_id"), 2 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("order", "o_id"), 0 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("order", "o_d_id"), 1 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("order", "o_w_id"), 2 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("order", "o_c_id"), 3 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("order", "o_entry_d"), 4 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("order", "o_carrier_id"), 5 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("order", "o_ol_cnt"), 6 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("order", "o_all_local"), 7 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("orderline", "ol_o_id"), 0 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("orderline", "ol_d_id"), 1 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("orderline", "ol_w_id"), 2 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("orderline", "ol_number"), 3 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("orderline", "ol_i_id"), 4 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("orderline", "ol_supply_w_id"), 5 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("orderline", "ol_delivery_d"), 6 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("orderline", "ol_quantity"), 7 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("orderline", "ol_amount"), 8 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("orderline", "ol_dist_info"), 9 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("item", "i_id"), 0 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("item", "i_im_id"), 1 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("item", "i_name"), 2 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("item", "i_price"), 3 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("item", "i_data"), 4 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_i_id"), 0 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_w_id"), 1 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_quantity"), 2 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_dist_01"), 3 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_dist_02"), 4 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_dist_03"), 5 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_dist_04"), 6 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_dist_05"), 7 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_dist_06"), 8 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_dist_07"), 9 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_dist_08"), 10 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_dist_09"), 11 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_dist_10"), 12 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_ytd"), 13 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_order_cnt"), 14 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_remote_cnt"), 15 );
	getTableIndex.emplace(std::make_tuple<std::string, std::string>("stock", "s_data"), 16 );
	};

	void printInfo() const
	{
		std::cout << "warehouse #entires: " << m_warehouse.size() << std::endl;
		std::cout << "district #entires: " << m_district.size() << std::endl;
		std::cout << "customer #entires: " << m_customer.size() << std::endl;
		std::cout << "history #entires: " << m_history.size() << std::endl;
		std::cout << "neworder #entires: " << m_neworder.size() << std::endl;
		std::cout << "order #entires: " << m_order.size() << std::endl;
		std::cout << "orderline #entires: " << m_orderline.size() << std::endl;
		std::cout << "item #entires: " << m_item.size() << std::endl;
		std::cout << "stock #entires: " << m_stock.size() << std::endl;
	}

	void delivery(Integer w_id, Integer o_carrier_id, Timestamp datetime)
    {
        Integer d_id{1};
        for(;d_id <= Integer{10};d_id = d_id + 1)
        {
            auto lower_bound = this->m_neworder.keys.lower_bound(std::make_tuple(w_id, d_id, Integer{INT32_MIN}));
            auto upper_bound = this->m_neworder.keys.lower_bound(std::make_tuple(w_id, d_id, Integer{INT32_MAX}));
            
            Integer o_id;
            
            if(lower_bound != upper_bound)
            {
                //tid
                auto result_pair = *lower_bound;
                o_id = m_neworder.no_d_id()[result_pair.second];
            }else
            {
                continue;
            }
                
            m_neworder.deleteEntry(o_id, d_id, w_id); 
        
            auto o_tid = m_order.find(o_id, d_id, w_id);
            auto o_ol_cnt = m_order.o_ol_cnt()[o_tid];
            auto o_c_id = m_order.o_c_id()[o_tid];
            
            m_order.o_carrier_id()[o_tid] = o_carrier_id;
        
            Numeric<6, 2> ol_total = 0;
            for(Integer ol_number = 1; ol_number.value <= o_ol_cnt.value; ol_number = ol_number +1)
            {
                auto ol_tid = m_orderline.find(o_id, d_id, w_id, ol_number);
                auto ol_amount = m_orderline.ol_amount()[ol_tid];
                ol_total = ol_total + ol_amount;
                m_orderline.ol_delivery_d()[ol_tid] = datetime;
            }
                 
        //TODO achtung primary keys ordnen in schema.sql orderline ! ! !
            auto tid = m_customer.find(o_c_id, d_id, w_id);
            m_customer.c_balance()[tid] = m_customer.c_balance()[tid] + ol_total.castS<12>();
        }
    }

void newOrder(Integer w_id, Integer d_id, Integer c_id, int32_t items, int32_t* supware, int32_t* itemid, int32_t* qty, Timestamp datetime)
{    
    auto w_tax = m_warehouse.w_tax()[m_warehouse.find(w_id)];  

    auto c_discount = m_customer.c_discount()[m_customer.find( c_id, d_id, w_id)];

    auto district_tid = m_district.find(d_id, w_id);

    auto o_id = m_district.d_next_o_id()[district_tid];
    auto d_tax = m_district.d_tax()[district_tid];

    //update
    m_district.d_tax()[district_tid] = m_district.d_tax()[district_tid] + 1;
    
    //check if all are local -> std::any_if or SSE?
    int32_t all_local = 1;
    for(int32_t index = 0; index < items; index++)
    {
        if(w_id != supware[index])
        all_local = 0;
    }
    //insert data into Order Table
    m_order.insert(o_id, d_id, w_id, c_id, datetime, 0, Integer{items}, Integer{all_local});
    //insert data into NewOrder Table
    m_neworder.insert(o_id, d_id, w_id);
    
    for(int32_t index = 0; index < items; index++)
    {
            
        auto i_price = m_item.i_price()[m_item.find(itemid[index])];
            
        auto stock_tid = m_stock.find(itemid[index], supware[index]);
            
        Char<24> s_dist;
        
        switch(d_id.value)
        {
        case(1):
                s_dist = m_stock.s_dist_01()[stock_tid];
                break;
        case(2):
                s_dist = m_stock.s_dist_02()[stock_tid];
                break;
        case(3):
                s_dist = m_stock.s_dist_03()[stock_tid];
                break;
        case(4):
                s_dist = m_stock.s_dist_04()[stock_tid];
                break;
        case(5):
                s_dist = m_stock.s_dist_05()[stock_tid];
                break;
        case(6):
                s_dist = m_stock.s_dist_06()[stock_tid];
                break;
        case(7):
                s_dist = m_stock.s_dist_07()[stock_tid];
                break;
        case(8):
                s_dist = m_stock.s_dist_08()[stock_tid];
                break;
        case(9):
                s_dist = m_stock.s_dist_09()[stock_tid];
                break;
        case(10):
                s_dist = m_stock.s_dist_10()[stock_tid];
                break;
            default:
                break;
        }
            
        if(m_stock.s_quantity()[stock_tid] > qty[index])
        {
            m_stock.s_quantity()[stock_tid] = m_stock.s_quantity()[stock_tid] - qty[index];
        }
        else
        {
            m_stock.s_quantity()[stock_tid] = m_stock.s_quantity()[stock_tid] + 91 - qty[index];
            
            if(supware[index] != w_id.value)
            {
                m_stock.s_remote_cnt()[stock_tid] = m_stock.s_remote_cnt()[stock_tid] + 1;
            }else
            {
                m_stock.s_remote_cnt()[stock_tid] = m_stock.s_order_cnt()[stock_tid] + 1;
            }
             
            Numeric<4, 0> one{1};
            Numeric<4, 0> one2{1};
            Numeric<4, 0> one3{1};


            auto a = (Numeric<6, 0>{qty[index]}.castP2() * i_price.castS<6>()).castS<8>();//3457,8
            auto b = (one2.castP2().castP2() + w_tax + d_tax).castS<8>();//1,2141
            auto c = ((one.castP2().castP2()  - c_discount)).castS<8>();

            auto tmp_result = (b * c).castM2<8>().castM2<8>() * a;
            
            
            Numeric<6, 2> ol_amount = tmp_result.castM2<8>().castM2<8>().castM2<6>();
            //due to precision instead of 2001,71 its 2430.14
            //            std::cout << "a * b * c =  2001,72 = " << ol_amount << std::endl;          
            //
            m_orderline.insert(o_id, d_id, w_id, Integer{index+1}, Integer{itemid[index]}, Integer{supware[index]}, Timestamp{0}, Numeric<2, 0>(qty[index]), ol_amount, s_dist);
        }
        
    }    

}
int32_t urand(int32_t min,int32_t max) {
	return (random()%(max-min+1))+min;
}
int32_t urandexcept(int32_t min,int32_t max,int32_t v) {
    if (max<=min)
	return min;
    int32_t r=(random()%(max-min))+min;
    if (r>=v)
	return r+1; else
	return r;
}

int32_t nurand(int32_t A,int32_t x,int32_t y) {
    return ((((random()%A)|(random()%(y-x+1)+x))+42)%(y-x+1))+x;
}

	void newOrderRandom(Timestamp now,int32_t w_id) {
    int32_t d_id=urand(1,10);
    int32_t c_id=nurand(1023,1,3000);
    int32_t ol_cnt=urand(5,15);

    int32_t supware[15];
    int32_t itemid[15];
    int32_t qty[15];
    for (int32_t i=0; i<ol_cnt; i++) {
	if (urand(1,100)>1)
	  supware[i]=w_id; else
	  supware[i]=urandexcept(1,warehouses,w_id);
	itemid[i]=nurand(8191,1,100000);
	qty[i]=urand(1,10);
    }

    newOrder(w_id,d_id,c_id,ol_cnt,supware,itemid,qty,now);
}

	void printTask1Info(int numRepeat)
	{    std::cout << "Database - ColumnStore\n" << std::endl;
    std::cout << "\tNewOrder has " << m_neworder.size() << " rows!" << std::endl;
    std::cout << "\tOrder has " << m_order.size() << " rows!" << std::endl;
    std::cout << "\tOrderLine has " << m_orderline.size() << " rows!\n" << std::endl;

	std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  
	for(auto i = 0; i < numRepeat; i++)
	{
		      newOrderRandom(Timestamp{static_cast<uint64_t>( 40+i)}, 1+(i%5));
  }
	end = std::chrono::system_clock::now();

	std::cout << "\t" << ((double) numRepeat*1000000) / std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << " newOrderRandom/s\n" << std::endl;
  
    std::cout << "\tNewOrder has " << m_neworder.size() << " rows!" << std::endl;
    std::cout << "\tOrder has " << m_order.size() << " rows!" << std::endl;
    std::cout << "\tOrderLine has " << m_orderline.size() << " rows!\n" << std::endl;
  	}


    void deliveryRandom(Timestamp now) {
        delivery(urand(1,warehouses),urand(1,10),now);
    }
   
    void oltp(Timestamp now, int i) {
        int rnd=urand(1,100);
        if (rnd<=10) {
            deliveryRandom(now);
        } else {
            newOrderRandom(now, i);
        }
    }
    
    void printTask2Info(int numRepeat)
    {
        std::cout << "Database - ColumnStore - task 2\n" << std::endl;
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        
        for(auto i = 0; i < numRepeat; i++)
        {
            oltp(Timestamp{static_cast<uint64_t>( 40+i)}, 1+(i%5));
        }
        end = std::chrono::system_clock::now();

        std::cout << "\t" << ((double) numRepeat*1000000) / std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << " Ops/s\n" << std::endl;
    }

    Numeric<12, 2> hashJoinTask3()
    {       
        
        std::unordered_map<std::tuple<Integer, Integer, Integer>, std::pair<Tid, Tid>, IntIntIntHash> customer_HT;
        
        for(auto tid = 0; tid < m_customer.size(); tid++)
        {
            auto c_last_tmp = m_customer.c_last()[tid];
            
            if(c_last_tmp.value[0] == 'B')
            {
                auto c_id = m_customer.c_id()[tid];
                auto c_d_id = m_customer.c_d_id()[tid];
                auto c_w_id = m_customer.c_w_id()[tid];
                
                customer_HT.emplace(std::make_tuple(c_d_id, c_w_id, c_id), std::make_pair(tid, 0));
            }
        }
        
        
            //std::pair<tid of customer, tid of order>
        std::unordered_map<std::tuple<Integer, Integer, Integer>, std::pair<Tid, Tid>, IntIntIntHash> resultJoin1;    
        
        for(auto tid = 0; tid < m_order.size(); tid++)
        {
            auto o_c_id = m_order.o_c_id()[tid];
            auto o_d_id = m_order.o_d_id()[tid];
            auto o_w_id = m_order.o_w_id()[tid];
            auto o_id = m_order.o_id()[tid];
            
            auto tid_customer_it = customer_HT.find(std::make_tuple(o_d_id, o_w_id, o_c_id));
            
            if(tid_customer_it != customer_HT.end())
            {
                resultJoin1.emplace(std::make_tuple(o_id, o_d_id, o_w_id), std::make_pair(tid_customer_it->second.first, tid)); 
            }
        }
        
        //second join
        Numeric<12, 2> sum{0};
        
        for(auto ol_tid = 0; ol_tid < m_orderline.size(); ol_tid++)
        {
            //get range of possible fitting keys
            auto o_id = m_orderline.ol_o_id()[ol_tid];
            auto o_d_id = m_orderline.ol_d_id()[ol_tid];
            auto o_w_id = m_orderline.ol_w_id()[ol_tid];
            
            auto search = resultJoin1.find(std::make_tuple(o_id, o_d_id, o_w_id));
            
            if(resultJoin1.end() != search)
            {
                    // sum += ol_quality * ol_amount - c_balance*o_ol_cnt
                    auto a = m_orderline.ol_quantity()[ol_tid].castP2().castS<12>() * m_orderline.ol_amount()[ol_tid].castS<12>();
                    auto b = m_customer.c_balance()[search->second.first] * m_order.o_ol_cnt()[search->second.second].castS<12>().castP2();
                
                    sum = sum + a.castM2<12>() - b.castM2<12>();
            }
        
        }
        return sum;
    }

    int fork_task3_hashjoin()
    {
            struct sigaction sa;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags=0;
            sa.sa_handler=SIGCHLD_handler;
            sigaction(SIGCHLD,&sa,NULL);

            int numTA = 1000000;
            
            std::chrono::time_point<std::chrono::system_clock> start, end;
            
            int64_t fork_avg = 0;
            int64_t numFork = 0;
            
            
            int fd[2];
            int status = pipe(fd);
            
            if(status != 0)
                throw std::runtime_error("Pipe couldn't be created!");
            
            int query_avg = 0;
            int numQueries = 0;
            
            int TA_time = 0;
            
            bool finishedTA = false;
    	    thread_local int ka = 0;        
            while(!finishedTA) 
            {
                childRunning_local=true;
                
                start = std::chrono::system_clock::now();
                pid_t pid=fork();
                end = std::chrono::system_clock::now();
                
                fork_avg += std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
                numFork++;
                
                //numQueries++;
                
                start = std::chrono::system_clock::now();
                if (pid) { // parent
                    //exec 1 mio transaction
                    while(ka < numTA)
                    {
                        if(!childRunning_local)
                        {
                            //reading only
                            close(fd[1]);
                            decltype(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()) val; 
                            status = read(fd[0], &val, sizeof(val));
                            
                            query_avg += val;
                            numQueries++;
                            break;
                        }
                        oltp(Timestamp{static_cast<uint64_t>( 40+ka)}, 1+(ka%5));
                        
                        ka++;
                        
                            
                    }
                    
                    if(ka >= numTA)
                        finishedTA = true;
                } else { // forked child
                    //only writing -> close read descriptor
                    close(fd[0]);
                    
                    
                    //start hashJoinTask3 - Query
                    start = std::chrono::system_clock::now();
                    this->hashJoinTask3();
                    end = std::chrono::system_clock::now();
                    auto val = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
                    
                    status = write(fd[1], &val, sizeof(val));
                    
                    //close write descriptor
                    close(fd[1]);
                    
                    return 0;
                    // child is finished
                }
                end = std::chrono::system_clock::now();
                TA_time += std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
            }
            
            //PRINT RESULT
            std::cout << "\t" << ((double)numTA*1000000)/TA_time << " TA/sec" << std::endl;
            std::cout << "\t" << (int)((double)query_avg)/numQueries << "µs per query" << std::endl;
            std::cout << "\t" << fork_avg/numFork << "µs per fork" << std::endl;

    }

	void printInfoTask3()
    {
        int numRepeat = 10;
        
        //auto result = hashJoinTask3(db);
        
        std::cout << "Database - ColumnStore - task 3\n" << std::endl;
        std::chrono::time_point<std::chrono::system_clock> start, end;
        
        for(auto i = 0; i < numRepeat; i++)
        {
            start = std::chrono::system_clock::now();
        
            auto result = hashJoinTask3();
            
            end = std::chrono::system_clock::now();
            std::cout << (i+1) << "\t" << result << "\t" << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << " µs/hashjoin" << std::endl;
        }
        std::cout << "\nSnapshotting - Measurements" << std::endl;
        
        fork_task3_hashjoin();
        
    }

    void printInfoTask4()
    {
        /*
        std::vector<AlgebraOperator::Attribute> attributes_customer = {
        {"m_customer", "c_id", "Integer"},
        {"m_customer", "c_d_id", "Integer"},
        {"m_customer", "c_w_id", "Integer"},
        {"m_customer", "c_first", "Varchar<16>"},
        {"m_customer", "c_middle", "Char<2>"},
        {"m_customer", "c_last", "Varchar<16>"},
        {"m_customer", "c_street_1", "Varchar<20>"},
        {"m_customer", "c_street_2", "Varchar<20>"}, 
        {"m_customer", "c_city", "Varchar<20>"},
        {"m_customer", "c_state", "Char<2>"},
        {"m_customer", "c_zip", "Char<9>"},
        {"m_customer", "c_phone", "Char<16>"},
        {"m_customer", "c_since", "Timestamp"}, 
        {"m_customer", "c_credit", "Char<2>"}, 
        {"m_customer", "c_credit_lim", "Numeric<12, 2>"},
        {"m_customer", "c_discount", "Numeric<4, 4>"},
        {"m_customer", "c_balance", "Numeric<12, 2>"},
        {"m_customer", "c_ytd_paymenr", "Numeric<12, 2>"},
        {"m_customer", "c_payment_cnt", "Numeric<4, 0>"}, 
        {"m_customer", "c_delivery_cnt", "Numeric<4, 0>"},
        {"m_customer", "c_data", "Varchar<500>"}
        };
        
        std::vector<AlgebraOperator::Attribute> attributes_order ={
            {"m_order", "o_id", "Integer"},
            {"m_order", "o_d_id", "Integer"},
            {"m_order", "o_w_id", "Integer"},
            {"m_order", "o_c_id", "Integer"},
            {"m_order", "o_entry_d", "Timestamp"},
            {"m_order", "o_carrier_id", "Integer"},
            {"m_order", "o_ol_cnt", "Numeric<2, 0>"},
            {"m_order", "o_all_local", "Numeric<1, 0>"}
        };
        
        std::vector<AlgebraOperator::Attribute> attributes_orderline = {
            {"m_orderline", "ol_o_id", "Integer"},
            {"m_orderline", "ol_d_id", "Integer"},
            {"m_orderline", "ol_w_id", "Integer"},
            {"m_orderline", "ol_number", "Integer"},
            {"m_orderline", "ol_i_id", "Integer"},
            {"m_orderline", "ol_supply_w_id", "Integer"},
            {"m_orderline", "ol_delivery_d", "Timestamp"},
            {"m_orderline", "ol_quantity", "Numeric<2, 0>"},
            {"m_orderline", "ol_amount", "Numeric<6, 2>"},
            {"m_orderline", "ol_dist_info", "Char<24>"}
        };
        
        
        auto customer = std::make_shared<AlgebraOperator::TableScan>("m_customer", attributes_customer);
        auto order = std::make_shared<AlgebraOperator::TableScan>("m_order", attributes_order);
        auto orderline = std::make_shared<AlgebraOperator::TableScan>("m_orderline", attributes_orderline);
        
        std::vector<Clause> clauses = {{"m_customer", "c_d_id", "", "1", true}, {"m_customer", "c_w_id", "", "1", true}, {"m_customer", "c_id", "", "322", true}};
        auto select = std::make_shared<AlgebraOperator::Selection>(customer, clauses);

        //1. Join customer and order
        std::vector<std::pair<AlgebraOperator::Attribute, AlgebraOperator::Attribute>> join_attributes = {{{"m_customer", "c_w_id", "Integer"}, {"m_order", "o_w_id", "Integer"}}, {{"m_customer", "c_d_id", "Integer"}, {"m_order", "o_d_id", "Integer"}}, {{"m_customer", "c_id", "Integer"}, {"m_order", "o_c_id", "Integer"}}};
        auto join_customer_order = std::make_shared<AlgebraOperator::HashJoin>(select, order, join_attributes);
    
        //2. Join customer_order and orderline
        std::vector<std::pair<AlgebraOperator::Attribute, AlgebraOperator::Attribute>> join_attributes2 = {{{"m_order", "o_w_id", "Integer"}, {"m_orderline", "ol_w_id", "Integer"}}, {{"m_order", "o_d_id", "Integer"}, {"m_orderline", "ol_d_id", "Integer"}}, {{"m_order", "o_id", "Integer"}, {"m_orderline", "ol_o_id", "Integer"}}};
        auto join_customer_order_orderline = std::make_shared<AlgebraOperator::HashJoin>(join_customer_order, orderline, join_attributes2);
    
        std::vector<AlgebraOperator::Attribute> attributes = {{"m_customer", "c_first", "Varchar<16>"}, {"m_customer", "c_last", "Varchar<16>"}, {"m_order", "o_all_local", "Numeric<1, 0>"}, {"m_orderline", "ol_amount", "Numeric<6, 2>"}};
        auto root = std::make_shared<AlgebraOperator::Print>(join_customer_order_orderline, attributes);
    
        root->setParent(nullptr);
        root->produce(std::shared_ptr<AlgebraOperator::AlgebraOperator>(nullptr));
        */
    }
    };


