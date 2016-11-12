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
    virtual void consume() = 0;
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
        std::cout << "for(auto tid = 0; tid < R.size(); tid++)\n{" << std::endl;
        if(auto sp_parent = this->parent.lock())
            sp_parent->consume();
        std::cout << "}\n" << std::endl;
    }
    void consume() override
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
    void consume() override
    {
        std::cout << "if(p(t))\n{" << std::endl;
        if(auto sp_parent = parent.lock())
            sp_parent->consume();
        std::cout << "}" << std::endl;
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
    void consume() override
    {
        std::cout << "std::cout << t << std::endl;" << std::endl;
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
    auto tablescan = std::make_shared<TableScan>();
    auto select = std::make_shared<Selection>(tablescan);
    auto root = std::make_shared<Print>(select);
    
    root->produce(std::shared_ptr<AlgebraOperator>(nullptr));
}

int main()
{
	//DatabaseColumn db;
    
    //db.printInfoTask3();
    
    printTableScan();
    
    return 0;   
}
