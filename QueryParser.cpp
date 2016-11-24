#include "QueryParser.hpp"
#include "parser/Schema.hpp"
#include "AlgebraOperator.hpp"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <unordered_map>
#include <sstream>
#include <exception>




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

static bool isInt(const std::string& str) {
   return str.find_first_not_of("0123456789") == std::string::npos;
}

/*
 * check whether join attributes exists
 * 
 */
bool QueryParser::requiresCrossProduct() const
{
    //create a pair for every join that must exist e.g. (A x B), (B x C),...
    std::vector<std::pair<std::string, std::string>> joins;
 
    for(auto i = 0; i < this->tables.size()-1; i++)
    {
        joins.push_back(std::make_pair(this->tables[i], this->tables[i+1]));
    }
    //check whether all table have a join attribute
    for(auto i = 0; i < this->whereClauses.size(); i++)
    {
        if(!this->whereClauses[i].isConstant)
        {
            joins.erase(std::remove_if(joins.begin(), joins.end(), [&](std::pair<std::string, std::string> const& tmp){ return ((tmp == std::make_pair(this->whereClauses[i].table_name, this->whereClauses[i].table_name2)) || (std::make_pair(this->whereClauses[i].table_name, this->whereClauses[i].table_name2) == tmp)); } ), joins.end());
        }
    }
    
    return !joins.empty();
}

std::string QueryParser::parse(std::string const& query) {
   std::string token = query;
   unsigned line=1;
   std::unique_ptr<SQL::Schema> s(new SQL::Schema());
   
   while (token.size() != 0) {
      //TODO
       if(token[0] == ',')
       {
           token.erase(0, 1);
           nextToken(line, ",", *s);
       }
       if(token.substr(0, 1) == " ")
           token.erase(0, 1);
       
       auto it = token.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_1234567890=\'");
       auto tmp = token.substr(0, it);
       
       //std::cout << "Tmp = " << tmp << std::endl;
       
       if(token.size() > it)
                token.erase(0, it);
       
       
       if(token.substr(0, 1) == " ")
           token.erase(0, 1);
       
       if(token == ";")
       {
           nextToken(line, tmp, *s);
           nextToken(line, ";", *s);
           token.clear();
       }
       else
           nextToken(line, tmp, *s);
       
       
   }
   /*
   std::cout << "this stack size = " << this->stack.size() << std::endl;
   for(auto i = 0; i < this->stack.size(); i++)
   {
       if(std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack[i]) != nullptr)
           std::cout << i << " HashJoin " << std::endl;
       if(std::dynamic_pointer_cast<AlgebraOperator::Print>(this->stack[i]) != nullptr)
           std::cout << i << " Print " << std::endl;
       if(std::dynamic_pointer_cast<AlgebraOperator::TableScan>(this->stack[i]) != nullptr)
           std::cout << i << " TableScan " << std::endl;
       if(std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i]) != nullptr)
           std::cout << i << " Selection " << std::endl;
   }
   */
   //get first and last hashJoin
   auto firstHJ = std::find_if(this->stack.begin(), this->stack.end(), [&](std::shared_ptr<AlgebraOperator::AlgebraOperator>& a) { return (std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(a) != nullptr); });
   //reverse range of hashJoins
   
   std::unordered_map<std::string, std::shared_ptr<AlgebraOperator::AlgebraOperator>> lastUsed;
   
   if(firstHJ != this->stack.end())
   {
       auto last = firstHJ;
        for(auto it = firstHJ; std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(*it) != nullptr; it++)
        {
            last = it;
        }
        //reverse that range
        std::reverse(firstHJ, last);
   }
   for(auto i = 0; i < this->stack.size(); i++)
   {
       if(std::dynamic_pointer_cast<AlgebraOperator::Print>(this->stack[i]) != nullptr)
           std::dynamic_pointer_cast<AlgebraOperator::Print>(this->stack[i])->input = this->stack[i+1];
       else if((std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack[i]) != nullptr) && (std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack[i+1]) != nullptr))
       {
                std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack[i])->left = std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack[i+1]);

                lastUsed[std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack[i])->tablename_right] = std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack[i+1]);
       }
       else
       {
           //only selections and tablescans left
            if((std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack[i]) != nullptr))
            {
                    //last HashJoin
                lastUsed[std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack[i])->tablename_left] = std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack[i]);
                lastUsed[std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack[i])->tablename_right] = std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack[i]);
            }
            /*
            * 
            * if is selection 
            * 
            * 
            */
           if((std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i]) != nullptr))
           {
               
               auto lastOp = lastUsed[std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i])->clauses[0].table_name];
               //check if was HashJoin then check whether to add to left or right side
               if(std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(lastOp) != nullptr)
               {
                   
                   if(std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(lastOp)->tablename_left == std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i])->clauses[0].table_name)
                   {
                       std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(lastOp)->left = std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i]);
                       lastUsed[std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(lastOp)->tablename_left] = std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i]);
                   }
                   else
                   {
                       std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(lastOp)->right = std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i]);
                       lastUsed[std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(lastOp)->tablename_right] = std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i]);
                   }
               }
               //if selection
               if(std::dynamic_pointer_cast<AlgebraOperator::Selection>(lastOp) != nullptr)
               {
                   
                   std::dynamic_pointer_cast<AlgebraOperator::Selection>(lastOp)->input = std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i]);
                   lastUsed[std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i])->clauses[0].table_name] = this->stack[i];
               }
               //if print
               if(std::dynamic_pointer_cast<AlgebraOperator::Print>(lastOp) != nullptr)
               {
                   
                   std::dynamic_pointer_cast<AlgebraOperator::Print>(lastOp)->input = std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i]);
                   lastUsed[std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i])->clauses[0].table_name] = this->stack[i];
               }
               //Table
               if(std::dynamic_pointer_cast<AlgebraOperator::TableScan>(lastOp) != nullptr)
               {
                   
                   std::dynamic_pointer_cast<AlgebraOperator::TableScan>(lastOp)->input = std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i]);
                   lastUsed[std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i])->clauses[0].table_name] = this->stack[i];
               }
           }
           //if TableScan just add to last last use 
           /*
            * 
            * if is tablescan 
            * 
            * 
            */
           if((std::dynamic_pointer_cast<AlgebraOperator::TableScan>(this->stack[i]) != nullptr))
           {
               //wont reach this point
               auto lastOp = lastUsed[std::dynamic_pointer_cast<AlgebraOperator::TableScan>(this->stack[i])->table_name];
               
               if(std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(lastOp) != nullptr)
               {
                   if(std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(lastOp)->tablename_left == std::dynamic_pointer_cast<AlgebraOperator::TableScan>(this->stack[i])->table_name)
                   {
                       std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(lastOp)->left = std::dynamic_pointer_cast<AlgebraOperator::TableScan>(this->stack[i]);
                   }
                   else
                   {
                       std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(lastOp)->right = std::dynamic_pointer_cast<AlgebraOperator::TableScan>(this->stack[i]);
                   }
               }
               //if selection
               if(std::dynamic_pointer_cast<AlgebraOperator::Selection>(lastOp) != nullptr)
               {
                   std::dynamic_pointer_cast<AlgebraOperator::Selection>(lastOp)->input = std::dynamic_pointer_cast<AlgebraOperator::TableScan>(this->stack[i]);
                   lastUsed[std::dynamic_pointer_cast<AlgebraOperator::TableScan>(this->stack[i])->table_name] = this->stack[i];
               }
               //if print
               if(std::dynamic_pointer_cast<AlgebraOperator::Print>(lastOp) != nullptr)
               {
                   std::dynamic_pointer_cast<AlgebraOperator::Print>(lastOp)->input = std::dynamic_pointer_cast<AlgebraOperator::TableScan>(this->stack[i]);
                   lastUsed[std::dynamic_pointer_cast<AlgebraOperator::TableScan>(this->stack[i])->table_name] = this->stack[i];
               }
           }
       }
   }
   
   
   // THIS STACK 
   /*
   for(int i = 0; i < this->stack.size(); i++)
   {
       if(std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack[i]) == nullptr)
       {
           //no hashjoin
           if(std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i]) != nullptr)
           {
               std::dynamic_pointer_cast<AlgebraOperator::Selection>(this->stack[i])->input = this->stack[i+1];
           }
           if(std::dynamic_pointer_cast<AlgebraOperator::Print>(this->stack[i]) != nullptr)
           {
               std::dynamic_pointer_cast<AlgebraOperator::Print>(this->stack[i])->input = this->stack[i+1];
           }
       }
       else
       {
           std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack[i])->left = this->stack[i+1];
           std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack[i])->right = this->stack[i+2];
       }
   }
   */
  std::stringstream os;
   this->stack[0]->produce(std::shared_ptr<AlgebraOperator::AlgebraOperator>(nullptr), os);
   
   
   
   
   if(this->requiresCrossProduct())
   {
       throw MissingJoinAttributeException();
   }
   
   return os.str();
}

static bool isQueryIdentifier(const std::string& str) {
   if (
      str==keyword::Select ||
      str==keyword::From ||
      str==keyword::Where ||
      str==keyword::And 
   )
      return false;
   return str.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_1234567890") == std::string::npos;
}

std::string typeToString(const SQL::Schema::Relation::Attribute& attr) {
   SchemaParser::Types::Tag type = attr.type;
   switch(type) {
      case SchemaParser::Types::Tag::Integer:
         return "Integer";
      case SchemaParser::Types::Tag::Timestamp:
          return "Timestamp";
      case SchemaParser::Types::Tag::Varchar:{
         std::stringstream ss;
         ss << "Varchar<" << attr.len << ">";
         return ss.str();
      }
      case SchemaParser::Types::Tag::Numeric: {
         std::stringstream ss;
         ss << "Numeric<" << attr.len1 << ", " << attr.len2 << ">";
         return ss.str();
      }
      case SchemaParser::Types::Tag::Char: {
         std::stringstream ss;
         ss << "Char<" << attr.len << ">";
         return ss.str();
      }
   }
   throw;
}


void QueryParser::nextToken(unsigned line, const std::string& token, SQL::Schema& schema) {

	if (getenv("DEBUG"))
		std::cerr << line << ": " << token << std::endl;
   if (token.empty())
      return;
   std::string tok;
   if(token[0] != '\'')
       std::transform(token.begin(), token.end(), std::back_inserter(tok), tolower);
   else
       tok = token;
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
                AlgebraOperator::Attribute tmp = {this->schema->getTableName(tok), tok, this->schema->getType(this->schema->getTableName(tok), tok)};
                this->selectedAttributes.emplace_back(tmp);
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
             //create print
             this->stack.push_back(std::make_shared<AlgebraOperator::Print>(this->selectedAttributes));
             
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
                  this->tables.push_back(tok);
                  state=State::TableName;
              }
              else
                throw QueryParserError(line, "Expected TableName, found '"+token+"'");
              
          }
          else if(tok[0] == literal::Semicolon)
          {
                //TABLESCAN MISSING 
              
                for(auto const& t : this->tables)
                {
                    std::vector<AlgebraOperator::Attribute> attributes;
                    
                    for(auto const& rel: this->schema->relations)
                    {
                        if(rel.name == t)
                        {
                            for(auto const& attr : rel.attributes)
                            {
                                attributes.push_back({t, attr.name, typeToString(attr)});
                            }
                        }
                    }
                    this->stack.push_back(std::make_shared<AlgebraOperator::TableScan>(t, attributes));
                }
              
                 
                 
            return;
              
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
             //push hashjoins on stack
             if(this->tables.size() != 1)
             {
             
                for(int i = 0; i < this->tables.size()-1; i++)
                {
                    auto table_n1 = this->tables[i];
                    auto table_n2 = this->tables[i+1];
                    
                    if(table_n1 < table_n2)
                        this->stack.push_back(std::make_shared<AlgebraOperator::HashJoin>(table_n1, table_n2));
                    else
                        this->stack.push_back(std::make_shared<AlgebraOperator::HashJoin>(table_n2, table_n1));
                    
                                        
                    if(table_n1 < table_n2)
                        this->getHashJoin[std::make_pair(table_n1, table_n2)] = std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack.back());
                    else
                        this->getHashJoin[std::make_pair(table_n2, table_n1)] = std::dynamic_pointer_cast<AlgebraOperator::HashJoin>(this->stack.back());
                }
                    
             }
            state=State::Where;
         } else if(tok[0]==literal::Semicolon)
         {
            state = State::End;
            
            //TABLESCANS
            //TABLESCAN MISSING TODO
                for(auto const& t : this->tables)
                {
                    std::vector<AlgebraOperator::Attribute> attributes;
                    
                    for(auto const& rel: this->schema->relations)
                    {
                        if(rel.name == t)
                        {
                            for(auto const& attr : rel.attributes)
                            {
                                attributes.push_back({t, attr.name, typeToString(attr)});
                            }
                        }
                    }
                    this->stack.push_back(std::make_shared<AlgebraOperator::TableScan>(t, attributes));
                }
            return;
         }
         else {
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
          //remove '
          if(tok[0] == '\'' && tok.back() == '\'')
          {
              tok = tok.substr(1, std::distance(tok.begin(), tok.end()) - 2);
          }
          if(isQueryIdentifier(tok))
          {
              
                if(this->schema->isAttribute(tok))
                {
                    //read possibly further attributes;
                    auto tableName = this->schema->getTableName(tok);
                    
                    this->whereClauses.back().table_name2 = tableName;
                    this->whereClauses.back().value = tok;
                }
                else if(isInt(tok))
                {
                    this->whereClauses.back().isConstant = true;
                    this->whereClauses.back().value = tok;          
                }
                else
                {
                        //throw QueryParserError(line, "Expected Attribute, found '"+token+"'");
                    //must be string param
                    this->whereClauses.back().isConstant = true;
                    this->whereClauses.back().value = "\"" + tok + "\"";
                }
          }
          
          state=State::WhereAttributeValue;   
         break;
      case State::WhereAttributeValue:
         if (tok == keyword::And)
            state=State::WhereFurtherAttribute;
         else if(tok[0] == literal::Semicolon)
         {
             
             for(auto const& clause : this->whereClauses)
             {
                 if(clause.isConstant)
                 {
                     //selection
                     std::vector<Clause> attr;
                     attr.push_back(clause);
                     this->stack.push_back(std::make_shared<AlgebraOperator::Selection>(attr));
                 }
                 else
                 {
                     std::vector<Clause> attr;
                     attr.push_back({clause.table_name, clause.attribute, clause.table_name2, clause.value, false});
                     
                     //search specific HashJoin and add joinattribute
                     if(clause.table_name < clause.table_name2)
                         this->getHashJoin[std::make_pair(clause.table_name, clause.table_name2)]->join_attributes.emplace_back(std::make_pair<AlgebraOperator::Attribute, AlgebraOperator::Attribute>({clause.table_name, clause.attribute, this->schema->getType(clause.table_name, clause.attribute)}, {clause.table_name2, clause.value, this->schema->getType(clause.table_name, clause.attribute)}));
                     else
                         this->getHashJoin[std::make_pair(clause.table_name2, clause.table_name)]->join_attributes.emplace_back(std::make_pair<AlgebraOperator::Attribute, AlgebraOperator::Attribute>({clause.table_name, clause.attribute, this->schema->getType(clause.table_name, clause.attribute)}, {clause.table_name2, clause.value, this->schema->getType(clause.table_name, clause.attribute)}));
                     
                 }
                 
             }
             
             //TABLESCAN MISSING
                 for(auto const& t : this->tables)
                {
                    std::vector<AlgebraOperator::Attribute> attributes;
                    
                    for(auto const& rel: this->schema->relations)
                    {
                        if(rel.name == t)
                        {
                            for(auto const& attr : rel.attributes)
                            {
                                attributes.push_back({t, attr.name, typeToString(attr)});
                            }
                        }
                    }
                    this->stack.push_back(std::make_shared<AlgebraOperator::TableScan>(t, attributes));
                }
             return;
         }
         else
            throw QueryParserError(line, "Expected 'and' or ';', found '"+token+"'");
         break;
      default:
         throw;
   }
}

void QueryParser::generateOperatorTree() const
{
    //0. Print
    //1. HashJoin
    //2. Selection
    //3. TableScan
/*    
    auto root = std::make_shared<AlgebraOperator::Print>(join_customer_order_orderline, attributes);
    
    root->setParent(nullptr);
    root->produce(std::shared_ptr<AlgebraOperator::AlgebraOperator>(nullptr));
  */  
  /*  
    //maps the tablename to last variable name of the table
    std::unordered_map<std::string, std::string> lastVarname;
    //TODO
    std::stringstream out;
    //First Tables scans
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
                lastVarname["m_" + this->schema->relations[i].name] = this->schema->relations[i].name;
            }
        }
        
        out << "};";
    }
    //2. Selection 
    auto selectionAttributes = this->getSelectionClauses();
    for(auto it = this->tables.begin(); it != this->tables.end(); it++)
    {
        std::vector<Clause> tmp_clause;
        std::for_each(selectionAttributes.begin(), selectionAttributes.end(), [&](auto const& a){ if( (*it) == a.table_name) tmp_clause.push_back(a); });
        
        if(!tmp_clause.empty())
        {
            //TODO create selection operator
        }
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
    
    return out.str();*/
}

std::vector<Clause> QueryParser::getJoinAttributes(std::string table_name1, std::string table_name2) const
{
    std::vector<Clause> result;
    
    //for all whereClauses 
    for(auto const& clause : this->whereClauses)
    {
        if((clause.table_name == table_name1 && clause.table_name2 == table_name2) || (clause.table_name == table_name2 && clause.table_name2 == table_name1))
        {
            result.emplace_back(clause);
        }
    }
    
    return result;
}

std::vector<Clause> QueryParser::getSelectionClauses() const
{
    std::vector<Clause> result;
    
    std::for_each(this->whereClauses.begin(), this->whereClauses.end(), [&](Clause const& c){ if(c.isConstant) result.push_back(c); });
    
    return result;
}

}


