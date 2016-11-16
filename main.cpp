#include "generatedSchema.cpp"
#include "My_Hashes.hpp"
#include <algorithm>

#include <tuple>
#include <utility>
#include <memory>

class AlgebraOperator
{
    std::shared_ptr<AlgebraOperator> input;
    std::weak_ptr<AlgebraOperator> parent;

public:
    virtual void produce(std::shared_ptr<AlgebraOperator> parent) = 0;
    virtual void consume(std::shared_ptr<AlgebraOperator> curr) = 0;
};

//global next free id
auto iu = 0;

/*
 * stores which attributes are important
 */
struct Context
{
    
};

struct clause
{
   std::string attribute;
   std::string value;
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
    
    
    
    HashJoin(std::shared_ptr<AlgebraOperator> left, std::shared_ptr<AlgebraOperator> right) : left{left}, right{right} {}
    
    
    void produce(std::shared_ptr<AlgebraOperator> parent) override
    {
        this->parent = parent;
        std::cout << "HT_A = INIT HT;" << std::endl;
        left->produce(shared_from_this());
        
        std::cout << "HT_B = INIT HT;" << std::endl;
        
        right->produce(shared_from_this());
    
    }
    
    void consume(std::shared_ptr<AlgebraOperator> curr) override
    {
        //only for left
        if(curr == left)
        {
            if(std::dynamic_pointer_cast<HashJoin>(left) == nullptr)
                std::cout << "store t in HT_A" << std::endl;
            
        }
        else
        {
            std::cout << "for tb in HT_A.lookup(tb)\n{\n" << std::endl;
            std::cout << "store t_tmp and t_b in HT_b" << std::endl;
            
            if(auto sp_parent = this->parent.lock())
                sp_parent->consume(shared_from_this());
            std::cout << "}\n" << std::endl; 
        }
    }
};


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
    

    TableScan() {}
    
    void produce(std::shared_ptr<AlgebraOperator> parent) override
    {
        this->parent = parent;
        std::cout << "for t in R\n{" << std::endl;
        if(auto sp_parent = this->parent.lock())
            sp_parent->consume(shared_from_this());
        std::cout << "}\n" << std::endl;
    }
    void consume(std::shared_ptr<AlgebraOperator> curr) override
    {
        //TODO
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
    
    void produce(std::shared_ptr<AlgebraOperator> parent) override
    {
        this->parent = parent;
        input->produce(shared_from_this());
    }
    void consume(std::shared_ptr<AlgebraOperator> curr) override
    {
        std::cout << "if(";
        
        for(int i = 0; i < this->clauses.size(); i++)
        {
                std::cout << this->clauses[i].attribute << " = " << this->clauses[i].value;
                if(i < this->clauses.size()-1)
                    std::cout << " && ";
        }
        
        std::cout << ")\n--{" << std::endl;
        if(auto sp_parent = parent.lock())
            sp_parent->consume(shared_from_this());
        std::cout << "\n--}" << std::endl;
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
public:
    Print(std::shared_ptr<AlgebraOperator> input) : input{input} {}
    
    void produce(std::shared_ptr<AlgebraOperator> parent) override
    {
        parent = parent;
        input->produce(shared_from_this());
    };
    void consume(std::shared_ptr<AlgebraOperator> curr) override
    {
        std::cout << "\t\tstd::cout << t << std::endl;" << std::endl;
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
    auto tablescanLeft = std::make_shared<TableScan>();
    
    std::vector<clause> clauses = {{"w_id", "5"}};
    auto select = std::make_shared<Selection>(tablescanLeft, clauses);
    
    auto tablescanRight = std::make_shared<TableScan>();
    //auto select2 = std::make_shared<Selection>(tablescanRight);
    
    auto hashJoin = std::make_shared<HashJoin>(select, tablescanRight);
    
    auto tableRightRight = std::make_shared<TableScan>();
    auto hashJoin2 = std::make_shared<HashJoin>(hashJoin, tableRightRight);
    
    auto root = std::make_shared<Print>(hashJoin2);
    
    root->produce(std::shared_ptr<AlgebraOperator>(nullptr));
}

int main()
{
	DatabaseColumn db;
  
    std::unordered_multimap<std::tuple<Integer, Integer, Integer>, std::tuple<Varchar<16>, Varchar<16>, Numeric<1, 0>>, IntIntIntHash> join_m_order_m_orderline_HT;
    std::unordered_multimap<std::tuple<Integer, Integer, Integer>, Tid, IntIntIntHash> join_m_customer_m_order_HT;
    for(auto tid = 0; tid < db.m_customer.size(); tid++)
    {
        if(db.m_customer.c_d_id()[tid] == 1 && db.m_customer.c_w_id()[tid] == 1 && db.m_customer.c_id()[tid] == 322)
        {
            join_m_customer_m_order_HT.emplace(std::make_tuple(db.m_customer.c_w_id()[tid], db.m_customer.c_d_id()[tid], db.m_customer.c_id()[tid]), tid);
        }
    }

    for(auto tid = 0; tid < db.m_order.size(); tid++)
    {
        auto range = join_m_customer_m_order_HT.equal_range(std::make_tuple(db.m_order.o_w_id()[tid], db.m_order.o_d_id()[tid], db.m_order.o_c_id()[tid]));
        for(auto it = range.first; it != range.second; ++it)
        {
            join_m_order_m_orderline_HT.emplace(std::make_tuple(db.m_order.o_w_id()[tid], db.m_order.o_d_id()[tid], db.m_order.o_id()[tid]), std::make_tuple(db.m_customer.c_first()[it->second], db.m_customer.c_last()[it->second], db.m_order.o_all_local()[tid]));
        }

    }

    for(auto tid = 0; tid < db.m_orderline.size(); tid++)
    {
        auto range = join_m_order_m_orderline_HT.equal_range(std::make_tuple(db.m_orderline.ol_w_id()[tid], db.m_orderline.ol_d_id()[tid], db.m_orderline.ol_o_id()[tid]));
        for(auto it = range.first; it != range.second; ++it)
        {
                std::cout << std::get<0>(it->second) << " " << std::get<1>(it->second) << " " << std::get<2>(it->second) << " " << db.m_orderline.ol_amount()[tid] << " " << std::endl;
        }

    }

    //db.printInfoTask3();
    
    //printTableScan();
    
    return 0;   
}
