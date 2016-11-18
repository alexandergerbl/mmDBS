#include "generatedSchema.cpp"
#include "My_Hashes.hpp"
#include <algorithm>

#include <tuple>
#include <utility>
#include <memory>


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

struct clause
{
    std::string table_name;
   std::string attribute;
   std::string table_name2;
   std::string value;
   //tells whether second value is a constant
   bool isConstant;
};

struct Attribute
{
    std::string table_name;
    std::string attribute_name;
    std::string type;
    
    bool operator<(Attribute const& rhs) const
    {
        if(this->table_name == rhs.table_name)
            return (this->attribute_name < rhs.attribute_name);
        return (this->table_name < rhs.table_name);
    }
    
    bool operator==(Attribute const& rhs) const
    {
        return (this->table_name == rhs.table_name) && (this->attribute_name == rhs.attribute_name);
    }
};

class Print;

class AlgebraOperator
{
public:
    std::shared_ptr<AlgebraOperator> input;
    std::weak_ptr<AlgebraOperator> parent;


    virtual void produce(std::shared_ptr<AlgebraOperator> parent) = 0;
    virtual void consume(std::shared_ptr<AlgebraOperator> curr) = 0;
    //look down the algebra tree and look whats pushed upwards
    virtual std::vector<Attribute> producesAttr() = 0;
    //look up the algebra tree and get what the operators above want you to produce
    virtual std::vector<Attribute> requires() = 0;
    
    //avoid empty weak_ptr 
    virtual void setParent(std::shared_ptr<AlgebraOperator> sp) = 0;
};


/*
 * 
 * HashJoin
 * 
 * 
 * 
 */
class HashJoin : public std::enable_shared_from_this<HashJoin>, public AlgebraOperator
{
public:
    std::shared_ptr<AlgebraOperator> left;
    std::shared_ptr<AlgebraOperator> right;
    
    std::weak_ptr<AlgebraOperator> parent;
    
    std::vector<std::pair<Attribute, Attribute>> join_attributes;
    
    HashJoin(std::shared_ptr<AlgebraOperator> left, std::shared_ptr<AlgebraOperator> right, std::vector<std::pair<Attribute, Attribute>> join_attributes) : left{left}, right{right}, join_attributes{join_attributes} {}

    
    void setParent(std::shared_ptr<AlgebraOperator> sp) override
    {
        this->parent = sp;
        this->left->setParent(shared_from_this());
        this->right->setParent(shared_from_this());
    }
    
    
    std::vector<Attribute> producesAttr() override
    {
        std::set<Attribute> produced;
     
        //get produced attributes
        auto left_tmp = left->producesAttr();
        auto right_tmp = right->producesAttr();
        
        //if left child is join, then only left side
        if(std::dynamic_pointer_cast<HashJoin>(left) == nullptr)
        {
            std::for_each(left_tmp.begin(), left_tmp.end(), [&](Attribute attr){ produced.insert(attr);});
        
        
            std::for_each(right_tmp.begin(), right_tmp.end(), [&](Attribute const& attr){ produced.insert(attr);});
        }
        else
        {
                   std::for_each(left_tmp.begin(), left_tmp.end(), [&](Attribute attr){ produced.insert(attr);});
        
        }
        
        std::set<Attribute> required;
        //get attributes that parents require
        if(auto sp_parent = this->parent.lock())
        {
            auto tmp = sp_parent->requires();
            std::for_each(tmp.begin(), tmp.end(), [&](Attribute const& attr){required.insert(attr);});   
        }
        else
        {
         std::cout << "\n\nERRORRRRRRRR print weakptr" << "\n" << std::endl;         
        }
        
        std::vector<Attribute> intersection;

        std::set_intersection(produced.begin(), produced.end(), required.begin(), required.end(), std::back_inserter(intersection));
        
    
        return intersection;
    }
    
    std::vector<Attribute> requires() override
    {
        if(auto sp_parent = this->parent.lock())
            return sp_parent->requires();
        throw std::bad_exception();
    }
    
    void produce(std::shared_ptr<AlgebraOperator> parent) override
    {
        this->parent = parent;
        
        //TODO: Key Compare combine_hash() as templated Functor 
        std::cout << "std::unordered_multimap<std::tuple<";
        //types
        for(auto i = 0; i < join_attributes.size(); i++)
        {
            auto tmp_attr = join_attributes[i].first;
            std::cout << tmp_attr.type;
            if(i < join_attributes.size()-1)
                std::cout << ", ";
        }
        //std::cout << "Integer, Integer, Integer";
        
        //end types
        std::cout << ">, ";
        
        //change to tuple<....> <- specifiy which attributes are needed
        std::cout << "std::tuple<";
        
        std::vector<Attribute> value_attributes;
        if(auto sp_parent = this->parent.lock())
            value_attributes = sp_parent->requires();
        else
            throw std::bad_exception();
        /*
         * WATCH OUT PARENT MUST BE SET BEFORE
         * 
         */
        auto input_attr_l = this->left->producesAttr();
       // auto input_attr_r = this->right->producesAttr();
 
        //remove attributes that arent produced by input
        value_attributes.erase(std::remove_if(value_attributes.begin(), value_attributes.end(), [&](Attribute const& attr){ 
            auto left_search = std::find(input_attr_l.begin(), input_attr_l.end(), attr);            
         //   auto right_search = std::find(input_attr_r.begin(), input_attr_r.end(), attr);
 
            return (left_search == input_attr_l.end());// && (right_search == input_attr_r.end());
        }), value_attributes.end());
        
        for(auto i = 0; i < value_attributes.size(); i++)
        {
            std::cout << " " << value_attributes[i].type;
            if(i < value_attributes.size() - 1)
                std::cout << ", ";
        }
        
        // Insert RESULT Types 
        std::cout << ">";
        
        std::cout << ", IntIntIntHash> ";//TODO combine_hash from num Int
        
        std::cout << "join_";
        std::cout << this->join_attributes[0].first.table_name << "_";
        std::cout << this->join_attributes[0].second.table_name << "_HT;" << std::endl;
        left->produce(shared_from_this());
        right->produce(shared_from_this());
    
    }
    
    
    
    
    
    std::vector<Attribute> addToValueOfHT(std::shared_ptr<HashJoin> curr)
    {
          std::vector<Attribute> value_attributes;
        if(auto sp_parent = curr->parent.lock())
            value_attributes = sp_parent->requires();
        else
            throw std::bad_exception();
        /*
         * WATCH OUT PARENT MUST BE SET BEFORE
         * 
         */
        auto input_attr_l = curr->left->producesAttr();
        //auto input_attr_r = this->right->producesAttr();
      
        //remove attributes that arent produced by input
        value_attributes.erase(std::remove_if(value_attributes.begin(), value_attributes.end(), [&](Attribute const& attr){ 
            auto left_search = std::find(input_attr_l.begin(), input_attr_l.end(), attr);            
            //auto right_search = std::find(input_attr_r.begin(), input_attr_r.end(), attr);
    
            return (left_search == input_attr_l.end());// && (right_search == input_attr_r.end());
        }), value_attributes.end());
  
        return value_attributes;
 
    }
    
    
    
    
    void consume(std::shared_ptr<AlgebraOperator> curr) override
    {
        //only for left
        if(curr == left)
        {
            //if(std::dynamic_pointer_cast<HashJoin>(left) == nullptr)
            {
                std::cout << "join_";
                std::cout << this->join_attributes[0].first.table_name << "_";
                std::cout << this->join_attributes[0].second.table_name << "_HT";
                std::cout << ".emplace(std::make_tuple(";
                for(auto i = 0; i < this->join_attributes.size(); i++)
                {
                    std::cout << "db." << this->join_attributes[i].first.table_name << ".";
                    std::cout << this->join_attributes[i].first.attribute_name << "()[tid]";
                    if(i < this->join_attributes.size()-1)
                        std::cout << ", ";
                }
                std::cout << "), ";
            if(std::dynamic_pointer_cast<HashJoin>(left) != nullptr)
            {
                std::cout << "std::tuple_cat(it->second, ";
            }
                std::cout << "std::make_tuple(";
                //TODO
    //----------------------------------------------------------
    // get attributes that will be generated by this operator
    //----------------------------------------------------------
      
        std::vector<Attribute> new_value_attributes = addToValueOfHT(shared_from_this());
                
        auto childHash = std::dynamic_pointer_cast<HashJoin>(left);
          if(childHash != nullptr)
          {
              //remove attributes that have been added previously to only add new attrs.
              std::vector<Attribute> new_value_attributes_old = addToValueOfHT(childHash);
              
              //get two sets
              std::set<Attribute> prev_produced;
                
                std::for_each(new_value_attributes_old.begin(), new_value_attributes_old.end(), [&](Attribute attr){ prev_produced.insert(attr);});
            
                std::set<Attribute> now_produced;
                //get attributes that parents require
                if(auto sp_parent = this->parent.lock())
                {
                    std::for_each(new_value_attributes.begin(), new_value_attributes.end(), [&](Attribute const& attr){now_produced.insert(attr);});   
                }
              //difference 
              std::vector<Attribute> diff;
              
              std::set_difference(now_produced.begin(), now_produced.end(), prev_produced.begin(), prev_produced.end(), std::back_inserter(diff));
              //remove all that are not element of difference               
              
              new_value_attributes.erase(std::remove_if(new_value_attributes.begin(), new_value_attributes.end(), [&](Attribute const& attr){ 
                auto left_search = std::find(diff.begin(), diff.end(), attr);            
           
    
                return (left_search == diff.end());// && (right_search == input_attr_r.end());
            }), new_value_attributes.end());
            
              
          }
                
        for(auto i = 0; i < new_value_attributes.size(); i++)
        {
            std::cout << "db." << new_value_attributes[i].table_name << "." << new_value_attributes[i].attribute_name << "()[tid]";
            if(i < new_value_attributes.size() - 1)
                std::cout << ", ";
        }
        //----------------------------------------------------------
                std::cout << "))";
                if(std::dynamic_pointer_cast<HashJoin>(left) != nullptr)
                    std::cout << ")" << std::endl;
                std::cout << ";" << std::endl;
            }
        }
        else
        {
            std::cout <<"auto range = ";
            std::cout << "join_";
            std::cout << this->join_attributes[0].first.table_name << "_";
            std::cout << this->join_attributes[0].second.table_name << "_HT";
            
            std::cout << ".equal_range(std::make_tuple(";
            
            for(auto i = 0; i < this->join_attributes.size(); i++)
            {
                std::cout << "db." << this->join_attributes[i].second.table_name << ".";
                std::cout << this->join_attributes[i].second.attribute_name << "()[tid]";
                if(i < this->join_attributes.size()-1)
                    std::cout << ", ";
            }
             
            
            std::cout << "));" << std::endl;
            std::cout << "for(auto it = range.first; it != range.second; ++it)\n{\n" << std::endl;
            
            
            if(auto sp_parent = this->parent.lock())
            {
                //if(std::dynamic_pointer_cast<Print>(sp_parent) == nullptr)
                 //       std::cout << "store t_tmp and t_b in HT_b" << std::endl;
                
                sp_parent->consume(shared_from_this());                
            }
            std::cout << "}\n" << std::endl; 
        }
    }
};




typedef uint16_t UI;
UI counter = 0;
//key: <table_name, attribute_name>
//value: UI
std::map<UI, std::tuple<std::string, std::string>> uis;

/*
 * 
 * TableScan
 * 
 * 
 * 
 */
class TableScan : public std::enable_shared_from_this<TableScan>, public AlgebraOperator
{
public:
    std::weak_ptr<AlgebraOperator> parent;
    std::string table_name;

    //all attributes of the table
    std::vector<Attribute> attributes;
    
    TableScan(std::string table_name, std::vector<Attribute> attributes) : table_name{table_name}, attributes{attributes} {}
    
    void setParent(std::shared_ptr<AlgebraOperator> sp) override
    {
        this->parent = sp;
    }
    
    std::vector<Attribute> producesAttr() override
    {
        //all attributes of this table
        return this->attributes;
    }
    
    std::vector<Attribute> requires() override
    {
        //always leaf -> should never be called
        throw std::bad_exception();
    }
    
    void produce(std::shared_ptr<AlgebraOperator> parent) override
    {
        this->parent = parent;
        std::cout << "for(auto tid = 0; tid < db.";
        std::cout << this->table_name << ".size(); tid++)\n{" << std::endl;
        if(auto sp_parent = this->parent.lock())
            sp_parent->consume(shared_from_this());
        std::cout << "}\n" << std::endl;
    }
    void consume(std::shared_ptr<AlgebraOperator> curr) override
    {
        //Nothing to do here
    }
};

/*
 * 
 * Selection
 * 
 * 
 * 
 */
class Selection : public std::enable_shared_from_this<Selection>, public AlgebraOperator
{
public:
    std::shared_ptr<AlgebraOperator> input;
    std::weak_ptr<AlgebraOperator> parent;
    
    std::vector<clause> clauses;
    
    Selection(std::shared_ptr<AlgebraOperator> input, std::vector<clause> clauses) : input{input}, clauses{clauses} {}
    
    void setParent(std::shared_ptr<AlgebraOperator> sp) override
    {
        this->parent = sp;
        input->setParent(shared_from_this());
    }
    
    
    std::vector<Attribute> producesAttr() override
    {
        return input->producesAttr();
    }
    
    std::vector<Attribute> requires() override
    {
        if(auto sp_parent = this->parent.lock())
            return sp_parent->requires();
        else
            throw std::bad_exception();
    }
    
    void produce(std::shared_ptr<AlgebraOperator> parent) override
    {
        this->parent = parent;
        input->produce(shared_from_this());
    }
    void consume(std::shared_ptr<AlgebraOperator> curr) override
    {
        std::cout << "\tif(";
        
        for(int i = 0; i < this->clauses.size(); i++)
        {
                //first always attrobute-name
                std::cout << "db." << this->clauses[i].table_name << "." << this->clauses[i].attribute << "()[tid]";
                std::cout << " == ";
                if(this->clauses[i].isConstant)
                    std::cout << this->clauses[i].value;
                else
                    std::cout << "db." << this->clauses[i].table_name2 << "." << this->clauses[i].value << "()[tid]";
                
                if(i < this->clauses.size()-1)
                    std::cout << " && ";
        }
        
        std::cout << ")\n\t{" << std::endl;
        if(auto sp_parent = parent.lock())
            sp_parent->consume(shared_from_this());
        std::cout << "\n\t}" << std::endl;
    };
};

/*
 * Root Operator 
 * 
 * 
 */
class Print : public std::enable_shared_from_this<Print>, public AlgebraOperator
{
    std::shared_ptr<AlgebraOperator> input;
    std::weak_ptr<AlgebraOperator> parent;
    //attributes that should be printed
    std::vector<Attribute> attributes;
public:
    Print(std::shared_ptr<AlgebraOperator> input, std::vector<Attribute> attributes) : input{input}, attributes{attributes} 
    {
        
    }
    
    void setParent(std::shared_ptr<AlgebraOperator> sp) override
    {
        this->parent = sp;
        input->setParent(shared_from_this());
    }
    
    //dummy wont ever get called, root always topmost
    std::vector<Attribute> producesAttr() override
    {
        return std::vector<Attribute>();
    }
    
    std::vector<Attribute> requires() override
    {
        return this->attributes;
    }
    
    
    void produce(std::shared_ptr<AlgebraOperator> parent) override
    {
        parent = parent;
        input->produce(shared_from_this());
    };
    void consume(std::shared_ptr<AlgebraOperator> curr) override
    {
        std::cout << "\t\tstd::cout";
        
        auto produced_by_child = input->producesAttr();
        
        for(auto i = 0; i < produced_by_child.size(); i++)
        {
            std::cout << " << std::get<" << i << ">(it->second) << \" \" ";
            
        }
        
        for(auto i = produced_by_child.size(); i < this->attributes.size(); i++)
        {
            std::cout << " << db." << this->attributes[i].table_name << "." << this->attributes[i].attribute_name << "()[" << "tid" << "] << \" \"";            
        }
        std::cout << " << std::endl;" << std::endl;
    };
};

/*
 * 
 * printTableScan
 * 
 * 
 * 
 */
void printTableScan()
{
    //TODO
    //all string parameter must be read from SQL statements
    //and checked whether they exist (using schema.sql file)
    std::vector<Attribute> attributes_customer = {
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
    
    std::vector<Attribute> attributes_order ={
        {"m_order", "o_id", "Integer"},
        {"m_order", "o_d_id", "Integer"},
        {"m_order", "o_w_id", "Integer"},
        {"m_order", "o_c_id", "Integer"},
        {"m_order", "o_entry_d", "Timestamp"},
        {"m_order", "o_carrier_id", "Integer"},
        {"m_order", "o_ol_cnt", "Numeric<2, 0>"},
        {"m_order", "o_all_local", "Numeric<1, 0>"}
    };
    
    std::vector<Attribute> attributes_orderline = {
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
    
    
    auto customer = std::make_shared<TableScan>("m_customer", attributes_customer);
    auto order = std::make_shared<TableScan>("m_order", attributes_order);
    auto orderline = std::make_shared<TableScan>("m_orderline", attributes_orderline);
    
    std::vector<clause> clauses = {{"m_customer", "c_d_id", "", "1", true}, {"m_customer", "c_w_id", "", "1", true}, {"m_customer", "c_id", "", "322", true}};
    auto select = std::make_shared<Selection>(customer, clauses);

    //1. Join customer and order
    std::vector<std::pair<Attribute, Attribute>> join_attributes = {{{"m_customer", "c_w_id", "Integer"}, {"m_order", "o_w_id", "Integer"}}, {{"m_customer", "c_d_id", "Integer"}, {"m_order", "o_d_id", "Integer"}}, {{"m_customer", "c_id", "Integer"}, {"m_order", "o_c_id", "Integer"}}};
    auto join_customer_order = std::make_shared<HashJoin>(select, order, join_attributes);
 
    //2. Join customer_order and orderline
    std::vector<std::pair<Attribute, Attribute>> join_attributes2 = {{{"m_order", "o_w_id", "Integer"}, {"m_orderline", "ol_w_id", "Integer"}}, {{"m_order", "o_d_id", "Integer"}, {"m_orderline", "ol_d_id", "Integer"}}, {{"m_order", "o_id", "Integer"}, {"m_orderline", "ol_o_id", "Integer"}}};
    auto join_customer_order_orderline = std::make_shared<HashJoin>(join_customer_order, orderline, join_attributes2);
 
    std::vector<Attribute> attributes = {{"m_customer", "c_first", "Varchar<16>"}, {"m_customer", "c_last", "Varchar<16>"}, {"m_order", "o_all_local", "Numeric<1, 0>"}, {"m_orderline", "ol_amount", "Numeric<6, 2>"}};
    auto root = std::make_shared<Print>(join_customer_order_orderline, attributes);
   
    root->setParent(nullptr);
    root->produce(std::shared_ptr<AlgebraOperator>(nullptr));
}


int main()
{
	DatabaseColumn db;
  
    
    std::unordered_multimap<std::tuple<Integer, Integer, Integer>, std::tuple< Varchar<16>,  Varchar<16>,  Numeric<1, 0>>, IntIntIntHash> join_m_order_m_orderline_HT;
std::unordered_multimap<std::tuple<Integer, Integer, Integer>, std::tuple< Varchar<16>,  Varchar<16>>, IntIntIntHash> join_m_customer_m_order_HT;
for(auto tid = 0; tid < db.m_customer.size(); tid++)
{
	if(db.m_customer.c_d_id()[tid] == 1 && db.m_customer.c_w_id()[tid] == 1 && db.m_customer.c_id()[tid] == 322)
	{
join_m_customer_m_order_HT.emplace(std::make_tuple(db.m_customer.c_w_id()[tid], db.m_customer.c_d_id()[tid], db.m_customer.c_id()[tid]), std::make_tuple(db.m_customer.c_first()[tid], db.m_customer.c_last()[tid]));

	}
}

for(auto tid = 0; tid < db.m_order.size(); tid++)
{
auto range = join_m_customer_m_order_HT.equal_range(std::make_tuple(db.m_order.o_w_id()[tid], db.m_order.o_d_id()[tid], db.m_order.o_c_id()[tid]));
for(auto it = range.first; it != range.second; ++it)
{

join_m_order_m_orderline_HT.emplace(std::make_tuple(db.m_order.o_w_id()[tid], db.m_order.o_d_id()[tid], db.m_order.o_id()[tid]), std::tuple_cat(it->second, std::make_tuple(db.m_order.o_all_local()[tid])))
;
}

}

for(auto tid = 0; tid < db.m_orderline.size(); tid++)
{
auto range = join_m_order_m_orderline_HT.equal_range(std::make_tuple(db.m_orderline.ol_w_id()[tid], db.m_orderline.ol_d_id()[tid], db.m_orderline.ol_o_id()[tid]));
for(auto it = range.first; it != range.second; ++it)
{

		std::cout << std::get<0>(it->second) << " "  << std::get<1>(it->second) << " "  << std::get<2>(it->second) << " "  << db.m_orderline.ol_amount()[tid] << " " << std::endl;
}

}

    
    //printTableScan();
    
    return 0;   
}
