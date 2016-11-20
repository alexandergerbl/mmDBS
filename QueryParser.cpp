#include "QueryParser.hpp"
#include "parser/Schema.hpp"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <cstdlib>

namespace QueryParser{

namespace keyword {
   const std::string Select = "select";
   const std::string From = "from";
   const std::string Where = "where";
   const std::string And = "and";
}

namespace literal {
   const char Semicolon = ';';
   const char Comma = ',';
   const char QuotationMark = '\"';
   const char Equal = '=';
}

std::unique_ptr<SQL::Schema> QueryParser::parse(std::string const& query) {
   std::string token = query;
   unsigned line=1;
   std::unique_ptr<SQL::Schema> s(new SQL::Schema());
   
   while (token.size() != 0) {
      //TODO
       auto it = token.find_first_not_of("abcdefghijklmnopqrstuvwxyz_1234567890");
       auto tmp = token.substr(0, it);
       
       nextToken(line, tmp, *s);
       if(token.size() > it)
           token.erase(0, it+1);
   }
   
   return std::move(s);
}

static bool isQueryIdentifier(const std::string& str) {
   if (
      str==keyword::Select ||
      str==keyword::From ||
      str==keyword::Where ||
      str==keyword::And 
   )
      return false;
   return str.find_first_not_of("abcdefghijklmnopqrstuvwxyz_1234567890") == std::string::npos;
}


static bool isInt(const std::string& str) {
   return str.find_first_not_of("0123456789") == std::string::npos;
}

void QueryParser::nextToken(unsigned line, const std::string& token, SQL::Schema& schema) {
	if (getenv("DEBUG"))
		std::cerr << line << ": " << token << std::endl;
   if (token.empty())
      return;
   std::string tok;
   std::transform(token.begin(), token.end(), std::back_inserter(tok), tolower);
   switch(state) {
      case State::Semicolon: /* fallthrough */
      case State::Init:
         if (tok==keyword::Select)
            state=State::Select;
         else
            throw QueryParserError(line, "Expected 'SELECT', found '"+token+"'");
         break;
      case State::FurtherAttribute: /*fallthrough*/
      case State::Select:
         if (isQueryIdentifier(tok))
         {
            //check if attribute exists
            if(this->schema->isAttribute(tok))
            {
                //read possibly further attributes;
                this->selectedAttributes.emplace_back(tok);
                state=State::AttributeName;   
            }
            else
                    throw QueryParserError(line, "Expected Attribute, found '"+token+"'");
            
         }
         else
             throw QueryParserError(line, "Expected Attribute, found '"+token+"'");
         break;
      case State::AttributeName:
          //std::string tmp;
          if(tok[0] == literal::Comma)
          {
                state=State::FurtherAttribute;
                break;
          }
         if (tok == keyword::From) {
            state=State::From;
         } else {
            throw QueryParserError(line, "Expected 'FROM' or ',', found '"+token+"'");
         }
         break;
//Added for index
      case State::FurtherTableName: /*fallthrough*/
      case State::From:
          if (isQueryIdentifier(tok))
          {
                //check if tablename exists
              if(this->schema->isTableName(tok))
              {
                  //nothing to do, just check whether table exists
                  this->tables.insert(tok);
                  state=State::TableName;
              }
              else
                throw QueryParserError(line, "Expected TableName, found '"+token+"'");
              
          }
          else
              throw QueryParserError(line, "Expected TableName, found '"+token+"'");
          break;
      case State::TableName:
          if(tok[0] == literal::Comma)
          {
                state=State::FurtherTableName;
                break;
          }
         if (tok == keyword::Where) {
            state=State::Where;
         } else {
            throw QueryParserError(line, "Expected 'WHERE' or ',', found '"+token+"'");
         }
          break;
      case State::WhereFurtherAttribute: /* fallthrough*/
      case State::Where:
          if(isQueryIdentifier(tok))
          {
                if(this->schema->isAttribute(tok))
                {
                    //read possibly further attributes;
                    auto tableName = this->schema->getTableName(tok);
                    
                    Clause clause = {tableName, tok, "", "", false};
                    
                    this->whereClauses.emplace_back(clause);
                    state=State::WhereAttributeName;   
                }
                else
                        throw QueryParserError(line, "Expected Attribute, found '"+token+"'");
                
          }
          break;
      case State::WhereAttributeName:
          if (tok.size()==1 && tok[0]==literal::Equal)
            state=State::Equal; //TODO 
          else
            throw QueryParserError(line, "Expected '=', found '"+token+"'");
          break;
      case State::Equal:
          //either constant or attribute
          if(isQueryIdentifier(tok))
          {
                if(this->schema->isAttribute(tok))
                {
                    //read possibly further attributes;
                    auto tableName = this->schema->getTableName(tok);
                    
                    this->whereClauses.back().table_name2 = tableName;
                    this->whereClauses.back().value = tok;
                }
                else
                        throw QueryParserError(line, "Expected Attribute, found '"+token+"'");
          } else
          {
              // must be constant
            this->whereClauses.back().isConstant = true;
            this->whereClauses.back().value = tok;          
          }
          state=State::WhereAttributeValue;   
         break;
      case State::WhereAttributeValue:
         if (tok == keyword::And)
            state=State::WhereFurtherAttribute;
         else if(tok[0] == literal::Semicolon)
             return;
         else
            throw QueryParserError(line, "Expected 'and' or ';', found '"+token+"'");
         break;
      default:
         throw;
   }
}

std::string QueryParser::generateCPP() const
{
    //TODO
    std::stringstream out;
    
    for(auto it = this->tables.begin(); it != this->tables.end(); it++)
    {
        out << "std::vector<Attribute> attributes_" << (*it) << " = {";
        
        //get all attributes and print e.g. {"m_customer", "c_id", "Integer"}
        for(auto i = 0; i < this->schema->relations.size(); i++)
        {
            if(this->schema->relations[i].name == (*it))
            {
                for(auto ii = 0; ii < this->schema->relations[i].attributes.size(); ii++)
                {
                    out << "{\"m_" << this->schema->relations[i].name << "\", \"" << this->schema->relations[i].attributes[ii].name << "\", \"" << SQL::type(this->schema->relations[i].attributes[ii]) << "\"}";
                    if(ii < this->schema->relations[i].attributes.size()-1)
                        out << ", ";
                }
                out << "auto " << this->schema->relations[i].name << " = std::make_shared<TableScan>(m_" << this->schema->relations[i].name << ", attributes_" << this->schema->relations[i].name << ");";
                std::cout << std::endl;
            }
        }
        
        out << "};";
    }
    
    out << "\n\n//what attributes to print\n";
    out << "std::vector<Attribute> print_attributes = {";
    for(auto const& attr : this->selectedAttributes)
    {
        out << "{\"m_" << this->schema->getTableName(attr) << "\", \"" << attr << "\", \"" << SQL::type(this->schema->getAttribute(attr)) << "\"}";
    }
    out << "};\n";
    out << "auto root = std::make_shared<Print>(";
    out << "warehouse";
    out << ", print_attributes";
    out << ");\nroot->setParent(nullptr);\nroot->produce(std::shared_ptr<AlgebraOperator>(nullptr));\n";
    
    return out.str();
}

}


