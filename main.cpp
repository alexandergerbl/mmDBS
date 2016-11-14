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
    

    Selection(std::shared_ptr<AlgebraOperator> input) : input{input} {}
    
    void produce(std::shared_ptr<AlgebraOperator> parent) override
    {
        this->parent = parent;
        input->produce(shared_from_this());
    }
    void consume(std::shared_ptr<AlgebraOperator> curr) override
    {
        std::cout << "if(p(t))\n--{" << std::endl;
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
    
    auto select = std::make_shared<Selection>(tablescanLeft);
    
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
	//DatabaseColumn db;
    
    //db.printInfoTask3();
    
    printTableScan();
    
    return 0;   
}
