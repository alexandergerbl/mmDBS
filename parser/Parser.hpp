#ifndef H_Parser_hpp
#define H_Parser_hpp

#include <exception>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include "Schema.hpp"



namespace SchemaParser{

class ParserError : std::exception {
   std::string msg;
   unsigned line;
   public:
   ParserError(unsigned line, const std::string& m) : msg(m), line(line) {}
   ~ParserError() throw() {}
   const char* what() const throw() {
      return msg.c_str();
   }
};

struct Parser {
   std::string fileName;
   std::ifstream in;
   enum class State : unsigned { Init, Create, Table, Index, IndexName, IndexTableName, NonPrimaryKeyStart, NonPrimaryKeyListBegin, NonPrimaryKeyName, NonPrimaryKeyListEnd, NonPrimaryKeyEnd, On, CreateTableBegin, CreateTableEnd, TableName, Primary, Key, KeyListBegin, KeyName, KeyListEnd, AttributeName, AttributeTypeInt, AttributeTypeTimestamp,  AttributeTypeVarchar, VarcharBegin, VarcharValue, VarcharEnd, AttributeTypeChar, CharBegin, CharValue, CharEnd, AttributeTypeNumeric, NumericBegin, NumericValue1, NumericSeparator, NumericValue2, NumericEnd, Not, Null, Separator, Semicolon};
   State state;
   Parser(const std::string& fileName) : fileName(fileName), state(State::Init) {}
   ~Parser() {};
   std::unique_ptr<SQL::Schema> parse();

   private:
   void nextToken(unsigned line, const std::string& token, SQL::Schema& s);
};

};
#endif
