#ifndef H_QueryParser_hpp
#define H_QueryParser_hpp

#include <exception>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
//#include <set>
#include <iostream>
//#include <unordered_map>
#include <map>
#include <utility>

#include "./parser/Schema.hpp"
#include "./parser/Parser.hpp"
#include "Clause.hpp"
#include "AlgebraOperator.hpp"

namespace QueryParser{

class MissingJoinAttributeException : std::exception
{
public:
    const char* what() const throw()
    {
        return "Missing Join Attributes -> Would require Cross Product!";
    }
};
    
class QueryParserError : std::exception {
   std::string msg;
   unsigned line;
   public:
   QueryParserError(unsigned line, const std::string& m) : msg(m), line(line) {}
   ~QueryParserError() throw() {}
   const char* what() const throw() {
      return msg.c_str();
   }
};

struct QueryParser {
   
   std::ifstream in;
   enum class State : unsigned { Init, End, Select, From, Where, And, Semicolon, Equal, AttributeName, FurtherAttribute, TableName, FurtherTableName, WhereAttributeName, WhereAttributeValue, WhereFurtherAttribute};
   State state;
   
   std::unique_ptr<SQL::Schema> schema;
   
   std::vector<AlgebraOperator::Attribute> selectedAttributes;
   std::vector<Clause> whereClauses;
   //get from FROM part
   std::vector<std::string> tables;
   //to access hashjoins to add attributes later
   //key <table-name1, table-name2> value <ptr to HashJoin>
   std::map<std::pair<std::string, std::string>, std::shared_ptr<AlgebraOperator::HashJoin>> getHashJoin;
   
   
   std::vector<std::shared_ptr<AlgebraOperator::AlgebraOperator>> stack;
   
   QueryParser() : state(State::Init)
   {
    SchemaParser::Parser p("./task1/schema.sql");
    
    try
    {
      schema = p.parse();
    } 
    catch (SchemaParser::ParserError& e) 
    {
      std::cerr << e.what() << std::endl;
    }
    
   }
   ~QueryParser() {};
   std::string parse(std::string const& query);

   void generateOperatorTree() const;
   
   private:
       //returns true if query ended ";"
   void nextToken(unsigned line, const std::string& token, SQL::Schema& s);
   
   //doesnt really belong to parser, but only one function
   std::vector<Clause> getJoinAttributes(std::string table_name1, std::string table_name2) const;
   std::vector<Clause> getSelectionClauses() const;
   
   bool requiresCrossProduct() const;
};

};

#endif
