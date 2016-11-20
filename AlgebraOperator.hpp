#ifndef ALGEBRAOPERATOR_HPP
#define ALGEBRAOPERATOR_HPP

#include "Clause.hpp"

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <map>

namespace AlgebraOperator
{

    struct Attribute
    {
        std::string table_name;
        std::string attribute_name;
        std::string type;
        
        bool operator<(Attribute const& rhs) const;
        
        bool operator==(Attribute const& rhs) const;
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

        
        void setParent(std::shared_ptr<AlgebraOperator> sp) override;
        
        std::vector<Attribute> producesAttr() override;
        
        std::vector<Attribute> requires() override;
        
        void produce(std::shared_ptr<AlgebraOperator> parent) override;
        
        
        
        std::vector<Attribute> addToValueOfHT(std::shared_ptr<HashJoin> curr);
        
        
        
        void consume(std::shared_ptr<AlgebraOperator> curr) override;
    };

    
/*
    typedef uint16_t UI;
    UI counter = 0;
    //key: <table_name, attribute_name>
    //value: UI
    std::map<UI, std::tuple<std::string, std::string>> uis;
*/
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
        
        void setParent(std::shared_ptr<AlgebraOperator> sp) override;
        
        std::vector<Attribute> producesAttr() override;
        
        std::vector<Attribute> requires() override;
        
        void produce(std::shared_ptr<AlgebraOperator> parent) override;
        
        void consume(std::shared_ptr<AlgebraOperator> curr) override;
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
        
        std::vector<Clause> clauses;
        
        Selection(std::shared_ptr<AlgebraOperator> input, std::vector<Clause> clauses) : input{input}, clauses{clauses} {}
        
        void setParent(std::shared_ptr<AlgebraOperator> sp) override;
        
        
        std::vector<Attribute> producesAttr() override;
        
        std::vector<Attribute> requires() override;
        
        void produce(std::shared_ptr<AlgebraOperator> parent) override;
        
        void consume(std::shared_ptr<AlgebraOperator> curr) override;
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
        
        void setParent(std::shared_ptr<AlgebraOperator> sp) override;
        
        //dummy wont ever get called, root always topmost
        std::vector<Attribute> producesAttr() override;
        
        std::vector<Attribute> requires() override;
        
        
        void produce(std::shared_ptr<AlgebraOperator> parent) override;
        
        void consume(std::shared_ptr<AlgebraOperator> curr) override;
    };


}
#endif 