#include "Parser.hpp"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <cstdlib>

namespace SchemaParser{

namespace keyword {
   const std::string Primary = "primary";
   const std::string Key = "key";
   const std::string Create = "create";
   const std::string Table = "table";
   const std::string Index = "index";
   const std::string Integer = "integer";
   const std::string Numeric = "numeric";
   const std::string Not = "not";
   const std::string On = "on";
   const std::string Null = "null";
   const std::string Char = "char";
   const std::string Varchar = "varchar";
   const std::string Timestamp = "timestamp";
}

namespace literal {
   const char ParenthesisLeft = '(';
   const char ParenthesisRight = ')';
   const char Comma = ',';
   const char Semicolon = ';';
   const char QuotationMark = '\"';
}

std::unique_ptr<SQL::Schema> Parser::parse() {
   std::string token;
   unsigned line=1;
   std::unique_ptr<SQL::Schema> s(new SQL::Schema());
   in.open(fileName.c_str());
   if (!in.is_open())
      throw ParserError(line, "cannot open file '"+fileName+"'");
   while (in >> token) {
      std::string::size_type pos;
      std::string::size_type prevPos = 0;

      while ((pos = token.find_first_of(",)(;", prevPos)) != std::string::npos) {
         nextToken(line, token.substr(prevPos, pos-prevPos), *s);
         nextToken(line, token.substr(pos,1), *s);
         prevPos=pos+1;
      }
      nextToken(line, token.substr(prevPos), *s);
      if (token.find("\n")!=std::string::npos)
         ++line;
   }
   
   in.close();
   return std::move(s);
}

static bool isIdentifier(const std::string& str) {
   if (
      str==keyword::Primary ||
      str==keyword::Key ||
      str==keyword::Table ||
      str==keyword::Index ||
      str==keyword::Create ||
      str==keyword::Integer ||
      str==keyword::Numeric ||
      str==keyword::Not ||
      str==keyword::On ||
      str==keyword::Null ||
      str==keyword::Char ||
      str==keyword::Varchar ||
      str==keyword::Timestamp
   )
      return false;
   return str.find_first_not_of("abcdefghijklmnopqrstuvwxyz_1234567890") == std::string::npos;
}

static bool isInt(const std::string& str) {
   return str.find_first_not_of("0123456789") == std::string::npos;
}

void Parser::nextToken(unsigned line, const std::string& token, SQL::Schema& schema) {
	if (getenv("DEBUG"))
		std::cerr << line << ": " << token << std::endl;
   if (token.empty())
      return;
   std::string tok;
   std::transform(token.begin(), token.end(), std::back_inserter(tok), tolower);
   switch(state) {
      case State::Semicolon: /* fallthrough */
      case State::Init:
         if (tok==keyword::Create)
            state=State::Create;
         else
            throw ParserError(line, "Expected 'CREATE', found '"+token+"'");
         break;
      case State::Create:
         if (tok==keyword::Table)
            state=State::Table;
         else if(tok==keyword::Index)
             state=State::Index;
         else
            throw ParserError(line, "Expected 'TABLE' or 'INDEX', found '"+token+"'");
         break;
      case State::Table:
          //std::string tmp;
          if(tok[0] == literal::QuotationMark)
          {
                //tmp = token.substr(1, std::distance(token.begin(), token.end()) - 2);
                state=State::TableName;
                schema.relations.push_back(SQL::Schema::Relation(token.substr(1, std::distance(token.begin(), token.end()) - 2)));
                break;
          }
         if (isIdentifier(tok)) {
            state=State::TableName;
            schema.relations.push_back(SQL::Schema::Relation(token));
         } else {
            throw ParserError(line, "Expected TableName, found '"+token+"'");
         }
         break;
//Added for index
      case State::Index:
          if (isIdentifier(tok)) {
            state=State::IndexName;
            //dont remember nonPrimaryKey name e.g. customer_wdl
            schema.relations.back().nonPrimaryKeyName = token;
          } else {
            throw ParserError(line, "Expected IndexName, found '"+token+"'");
          }
          break;
      case State::IndexName:
          if (tok==keyword::On)
            state=State::On;
          break;
      case State::On:
          if(tok[0] == literal::QuotationMark)
          {
                //tmp = token.substr(1, std::distance(token.begin(), token.end()) - 2);
                state=State::IndexTableName;
                
                schema.relationName = tok.substr(1, std::distance(token.begin(), token.end()) - 2);
            
                //check whether table/relation exists
                auto it_relation = std::find_if(schema.relations.begin(), schema.relations.end(), [&](auto const& r){ return (tok.substr(1, std::distance(token.begin(), token.end()) - 2) == r.name);});
        
                if(it_relation == schema.relations.end())
                    throw ParserError(line, "'"+token+"' is not a TABLE ");
                
                break;
          }
          if (isIdentifier(tok)) {
            state=State::IndexTableName;
            
            schema.relationName = tok;
           
            //check whether table/relation exists
            auto it_relation = std::find_if(schema.relations.begin(), schema.relations.end(), [&](auto const& r){ return (tok == r.name);});
    
            if(it_relation == schema.relations.end())
                throw ParserError(line, "'"+token+"' is not a TABLE ");
            //remember table on which to set up a non primary key  
          } else {
            throw ParserError(line, "Expected IndexTableName, found '"+token+"'");
          }
          break;
      case State::IndexTableName:
          if (tok.size()==1 && tok[0]==literal::ParenthesisLeft)
            state=State::NonPrimaryKeyStart; //TODO 
          else
            throw ParserError(line, "Expected '(', found '"+token+"'");
          break;
      case State::NonPrimaryKeyStart:
          //KeyListBegin adjust
          //same as key but for non-primaryKey
    
          
          
          if (isIdentifier(tok)) {
            struct AttributeNamePredicate {
               const std::string& name;
               AttributeNamePredicate(const std::string& name) : name(name) {}
               bool operator()(const SQL::Schema::Relation::Attribute& attr) const {
                  return attr.name == name;
               }
            };
            //check whether 
            auto it_relation = std::find_if(schema.relations.begin(), schema.relations.end(), [&](auto const& r){ return (schema.relationName == r.name);});
            const auto& attributes = it_relation->attributes;
      //            const auto& attributes = schema.relations.back().attributes;
            AttributeNamePredicate p(token);
            auto it = std::find_if(attributes.begin(), attributes.end(), p);
            if (it == attributes.end())
               throw ParserError(line, "'"+token+"' is not an attribute of '"+schema.relations.back().name+"'");
            schema.nonprimaryKey.push_back(std::distance(attributes.begin(), it));
            it_relation->nonPrimaryKey.push_back(std::distance(attributes.begin(), it));
            state=State::NonPrimaryKeyName;
         } else {
            throw ParserError(line, "Expected key attribute, found '"+token+"'");
         }
         break;
      case State::NonPrimaryKeyName:
         if (tok.size()==1 && tok[0] == literal::Comma)
            state=State::NonPrimaryKeyStart;
         else if (tok.size()==1 && tok[0] == literal::ParenthesisRight)
            state=State::NonPrimaryKeyListEnd;
         else
            throw ParserError(line, "Expected ',' or ')', found '"+token+"'");
         break;
      case State::NonPrimaryKeyListEnd:
         if (tok.size()==1 && tok[0] == literal::Semicolon)
         {
            //copy nonprimarykey to relation and resize() vector to 0 and relation name of nonprimarykey
            //token read to much data until here
            auto it_relation = std::find_if(schema.relations.begin(), schema.relations.end(), [&](auto const& r){ return (schema.relationName == r.name);});
        
            it_relation->nonPrimaryKey = schema.nonprimaryKey;
            schema.nonprimaryKey.clear();
            
            state=State::Semicolon;
         }
         else
            throw ParserError(line, "Expected ',' or ')', found '"+token+"'");
         break;

         
//end index
      case State::TableName:
         if (tok.size()==1 && tok[0]==literal::ParenthesisLeft)
            state=State::CreateTableBegin;
         else
            throw ParserError(line, "Expected '(', found '"+token+"'");
         break;
      case State::Separator: /* fallthrough */
      case State::CreateTableBegin:
         if (tok.size()==1 && tok[0]==literal::ParenthesisRight) {
            state=State::CreateTableEnd;
         } else if (tok==keyword::Primary) {
            state=State::Primary;
         } else if (isIdentifier(tok)) {
            schema.relations.back().attributes.push_back(SQL::Schema::Relation::Attribute());
            schema.relations.back().attributes.back().name = token;
            state=State::AttributeName;
         } else {
            throw ParserError(line, "Expected attribute definition, primary key definition or ')', found '"+token+"'");
         }
         break;
      
      
      case State::CreateTableEnd:
         if (tok.size()==1 && tok[0]==literal::Semicolon)
            state=State::Semicolon;
         else
            throw ParserError(line, "Expected ';', found '"+token+"'");
         break;
      case State::Primary:
         if (tok==keyword::Key)
            state=State::Key;
         else
            throw ParserError(line, "Expected 'KEY', found '"+token+"'");
         break;
      case State::Key:
         if (tok.size()==1 && tok[0]==literal::ParenthesisLeft)
            state=State::KeyListBegin;
         else
            throw ParserError(line, "Expected list of key attributes, found '"+token+"'");
         break;
      case State::KeyListBegin:
         if (isIdentifier(tok)) {
            struct AttributeNamePredicate {
               const std::string& name;
               AttributeNamePredicate(const std::string& name) : name(name) {}
               bool operator()(const SQL::Schema::Relation::Attribute& attr) const {
                  return attr.name == name;
               }
            };
            const auto& attributes = schema.relations.back().attributes;
            AttributeNamePredicate p(token);
            auto it = std::find_if(attributes.begin(), attributes.end(), p);
            if (it == attributes.end())
               throw ParserError(line, "'"+token+"' is not an attribute of '"+schema.relations.back().name+"'");
            schema.relations.back().primaryKey.insert(schema.relations.back().primaryKey.begin(), (std::distance(attributes.begin(), it)));
            state=State::KeyName;
         } else {
            throw ParserError(line, "Expected key attribute, found '"+token+"'");
         }
         break;
      case State::KeyName:
         if (tok.size()==1 && tok[0] == literal::Comma)
            state=State::KeyListBegin;
         else if (tok.size()==1 && tok[0] == literal::ParenthesisRight)
            state=State::KeyListEnd;
         else
            throw ParserError(line, "Expected ',' or ')', found '"+token+"'");
         break;
      case State::KeyListEnd:
         if (tok.size()==1 && tok[0] == literal::Comma)
            state=State::Separator;
         else if (tok.size()==1 && tok[0] == literal::ParenthesisRight)
            state=State::CreateTableEnd;
         else
            throw ParserError(line, "Expected ',' or ')', found '"+token+"'");
         break;
      case State::AttributeName:
         if (tok==keyword::Integer) {
            schema.relations.back().attributes.back().type=Types::Tag::Integer;
            state=State::AttributeTypeInt;
         } else if (tok==keyword::Timestamp) {
             schema.relations.back().attributes.back().type=Types::Tag::Timestamp;
            state=State::AttributeTypeTimestamp;
         } else if (tok==keyword::Varchar) {
            schema.relations.back().attributes.back().type=Types::Tag::Varchar;
            state=State::AttributeTypeVarchar;
         } else if (tok==keyword::Char) {
            schema.relations.back().attributes.back().type=Types::Tag::Char;
            state=State::AttributeTypeChar;
         } else if (tok==keyword::Numeric) {
            schema.relations.back().attributes.back().type=Types::Tag::Numeric;
            state=State::AttributeTypeNumeric;
         }
         else throw ParserError(line, "Expected type after attribute name, found '"+token+"'");
         break;
//ADD Varchar
      case State::AttributeTypeVarchar:
         if (tok.size()==1 && tok[0]==literal::ParenthesisLeft)
            state=State::VarcharBegin;
         else
            throw ParserError(line, "Expected '(' after 'VARCHAR', found'"+token+"'");
         break;
      case State::VarcharBegin:
         if (isInt(tok)) {
            schema.relations.back().attributes.back().len=std::atoi(tok.c_str());
            state=State::VarcharValue;
         } else {
            throw ParserError(line, "Expected integer after 'VARCHAR(', found'"+token+"'");
         }
         break;
      case State::VarcharValue:
         if (tok.size()==1 && tok[0]==literal::ParenthesisRight)
            state=State::VarcharEnd;
         else
            throw ParserError(line, "Expected ')' after length of VARCHAR, found'"+token+"'");
         break;
//END Varchar
      case State::AttributeTypeChar:
         if (tok.size()==1 && tok[0]==literal::ParenthesisLeft)
            state=State::CharBegin;
         else
            throw ParserError(line, "Expected '(' after 'CHAR', found'"+token+"'");
         break;
      case State::CharBegin:
         if (isInt(tok)) {
            schema.relations.back().attributes.back().len=std::atoi(tok.c_str());
            state=State::CharValue;
         } else {
            throw ParserError(line, "Expected integer after 'CHAR(', found'"+token+"'");
         }
         break;
      case State::CharValue:
         if (tok.size()==1 && tok[0]==literal::ParenthesisRight)
            state=State::CharEnd;
         else
            throw ParserError(line, "Expected ')' after length of CHAR, found'"+token+"'");
         break;
      case State::AttributeTypeNumeric:
         if (tok.size()==1 && tok[0]==literal::ParenthesisLeft)
            state=State::NumericBegin;
         else
            throw ParserError(line, "Expected '(' after 'NUMERIC', found'"+token+"'");
         break;
      case State::NumericBegin:
         if (isInt(tok)) {
            schema.relations.back().attributes.back().len1=std::atoi(tok.c_str());
            state=State::NumericValue1;
         } else {
            throw ParserError(line, "Expected integer after 'NUMERIC(', found'"+token+"'");
         }
         break;
      case State::NumericValue1:
         if (tok.size()==1 && tok[0]==literal::Comma)
            state=State::NumericSeparator;
         else
            throw ParserError(line, "Expected ',' after first length of NUMERIC, found'"+token+"'");
         break;
      case State::NumericValue2:
         if (tok.size()==1 && tok[0]==literal::ParenthesisRight)
            state=State::NumericEnd;
         else
            throw ParserError(line, "Expected ')' after second length of NUMERIC, found'"+token+"'");
         break;
      case State::NumericSeparator:
         if (isInt(tok)) {
            schema.relations.back().attributes.back().len2=std::atoi(tok.c_str());
            state=State::NumericValue2;
         } else {
            throw ParserError(line, "Expected second length for NUMERIC type, found'"+token+"'");
         }
         break;
      case State::VarcharEnd: /* fallthrough */
      case State::CharEnd: /* fallthrough */
      case State::NumericEnd: /* fallthrough */
      case State::AttributeTypeTimestamp: /* fallthrough */
      case State::AttributeTypeInt:
         if (tok.size()==1 && tok[0]==literal::Comma)
            state=State::Separator;
         else if (tok==keyword::Not)
            state=State::Not;
         else if (tok.size()==1 && tok[0]==literal::ParenthesisRight)
				state=State::CreateTableEnd;
         else throw ParserError(line, "Expected ',' or 'NOT NULL' after attribute type, found '"+token+"'");
         break;
      case State::Not:
         if (tok==keyword::Null) {
            schema.relations.back().attributes.back().notNull=true;
            state=State::Null;
         }
         else throw ParserError(line, "Expected 'NULL' after 'NOT' name, found '"+token+"'");
         break;
      case State::Null:
         if (tok.size()==1 && tok[0]==literal::Comma)
            state=State::Separator;
         else if (tok.size()==1 && tok[0]==literal::ParenthesisRight)
            state=State::CreateTableEnd;
         else throw ParserError(line, "Expected ',' or ')' after attribute definition, found '"+token+"'");
         break;
      default:
         throw;
   }
}

}
