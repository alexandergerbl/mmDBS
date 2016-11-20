#ifndef H_QueryParser_hpp
#define H_QueryParser_hpp

#include <exception>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>

#include "./parser/Schema.hpp"
#include "./parser/Parser.hpp"
#include "Clause.hpp"

namespace QueryParser{

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
   enum class State : unsigned { Init, Select, From, Where, And, Semicolon, Equal, AttributeName, FurtherAttribute, TableName, FurtherTableName, WhereAttributeName, WhereAttributeValue, WhereFurtherAttribute};
   State state;
   
   std::unique_ptr<SQL::Schema> schema;
   
   std::vector<std::string> selectedAttributes;
   std::vector<Clause> whereClauses;
   
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
   std::unique_ptr<SQL::Schema> parse(std::string const& query);

   private:
       //returns true if query ended ";"
   void nextToken(unsigned line, const std::string& token, SQL::Schema& s);
};

};

#endif
