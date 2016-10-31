#include "Schema.hpp"

#include <iostream>
#include <sstream>

static std::string type(const Schema::Relation::Attribute& attr) {
   Types::Tag type = attr.type;
   switch(type) {
      case Types::Tag::Integer:
         return "Integer";
      case Types::Tag::Timestamp:
          return "Timestamp";
      case Types::Tag::Varchar:{
         std::stringstream ss;
         ss << "Varchar<" << attr.len << ">";
         return ss.str();
      }
      case Types::Tag::Numeric: {
         std::stringstream ss;
         ss << "Numeric<" << attr.len1 << ", " << attr.len2 << ">";
         return ss.str();
      }
      case Types::Tag::Char: {
         std::stringstream ss;
         ss << "Char<" << attr.len << ">";
         return ss.str();
      }
   }
   throw;
}

std::string Schema::toString() const {
   std::stringstream out;  
   
   for (const Schema::Relation& rel : relations) {
      out << rel.name << std::endl;
      out << "\tPrimary Key:";
      
      for (unsigned keyId : rel.primaryKey)
         out << ' ' << rel.attributes[keyId].name;
      out << std::endl;
      for (const auto& attr : rel.attributes)
         out << '\t' << attr.name << '\t' << type(attr) << (attr.notNull ? " not null" : "") << std::endl;
   }
   return out.str();
}

std::string Schema::toCPP() const
{
   std::stringstream out;  
   //header
   out << "#include \"ColumnStore.hpp\"\n#include <cstdlib>\n#include <tuple>\n\n";
   
   for (const Schema::Relation& rel : relations) {
      out << "class " << rel.name << " : public ColumnStore<" << rel.primaryKey.size();
      for (const auto& attr : rel.attributes)
         out << ',' << type(attr);
      //Constructor
      out << ">\n{\npublic:\n\t" << rel.name << "(std::string file) : ColumnStore(file){}\n\n"; 
      
      //getter&setter
      int j = 0;
      for (const auto& attr : rel.attributes)
      {
         out << "\tinline auto& " << attr.name << "(){   return std::get<" << j << ">(this->data); }\n";
         j++;
      }
      out << std::endl;
      
      //getByPrimaryKey
      out << "\tTid find(";
       for (auto i = 0; i < rel.primaryKey.size(); i++)
       {
         out << "const " << type(rel.attributes[rel.primaryKey[i]]) <<"& " << rel.attributes[rel.primaryKey[i]].name;
         if(i != rel.primaryKey.size()-1)
             out << ", ";
       }
       out << ") const\n\t{\n\t\treturn keys[std::make_tuple(";
       for (auto i = 0; i < rel.primaryKey.size(); i++)
       {
         out << rel.attributes[rel.primaryKey[i]].name;
         if(i != rel.primaryKey.size()-1)
             out << ", ";
       }
             
       out << ")];\n\t}\n";
      
        
       /**
         * size()
         */    
        out << "\tstd::size_t size() const\n\t{\n\t\treturn std::get<0>(data).size();\n\t}\n";

        
        /**
         * Insert
         */
        out << "\tvoid insert(";
        
        
        for (auto i = 0; i < rel.attributes.size(); i++)
        {
            out << type(rel.attributes[i]) << " " << rel.attributes[i].name;
            if(i != rel.attributes.size()-1)
                out << ", ";
            
        }
        out << ")\n\t{\n\t\tauto tid = this->size();\n\n\t\tif(this->keys.keys.count(std::make_tuple(";
        for (auto i = 0; i < rel.primaryKey.size(); i++)
        {
            out << rel.attributes[rel.primaryKey[i]].name;
            if(i != rel.primaryKey.size()-1)
                out << ", ";
        }        
        out << ")) == 1)\n\t\t\tthrow std::invalid_argument(\"In relation " << rel.name << ": primary key already taken -> must be unique!\");\n\n";
        //add to each column
        for (auto i = 0; i < rel.attributes.size(); i++)
        {
            out << "\t\tthis->" << rel.attributes[i].name << "().emplace_back(" << rel.attributes[i].name << ");\n"; ;
        }
        
        out << "\n\t\tthis->keys[std::make_tuple(";
        for (auto i = 0; i < rel.primaryKey.size(); i++)
        {
            out << rel.attributes[rel.primaryKey[i]].name;
            if(i != rel.primaryKey.size()-1)
                out << ", ";
        }        
        out << ")] = tid;\n";
        out << "\t}\n\n";
        
        /**
         * Delete
         */
        out << "\tvoid deleteEntry(";
        for (auto i = 0; i < rel.primaryKey.size(); i++)
        {
         out << "const " << type(rel.attributes[rel.primaryKey[i]]) <<"& " << rel.attributes[rel.primaryKey[i]].name;
         if(i != rel.primaryKey.size()-1)
             out << ", ";
        }
        out << ")\n\t{\n";
        out << "\t\tauto tid = this->find(";
        for (auto i = 0; i < rel.primaryKey.size(); i++)
        {
         out << rel.attributes[rel.primaryKey[i]].name;
         if(i != rel.primaryKey.size()-1)
             out << ", ";
        }
        out << ");\n\n";
        
            //...for all elements
        for (auto i = 0; i < rel.attributes.size(); i++)
        {
            out << "\t\tstd::iter_swap(this->" << rel.attributes[i].name << "().begin()+tid, this->" << rel.attributes[i].name << "().end()-1);\n";
            out << "\t\tthis->" << rel.attributes[i].name << "().pop_back();\n";    
        }
       
        out << "\n\t\tthis->keys.keys.erase(std::make_tuple(";
        for (auto i = 0; i < rel.primaryKey.size(); i++)
        {
            out << rel.attributes[rel.primaryKey[i]].name;
            if(i != rel.primaryKey.size()-1)
                out << ", ";
        }
        out << "));\n";
        out << "\n";
    
        //update tid of swapped entry
        out << "\n\t\tthis->keys.keys[std::make_tuple(";
        for (auto i = 0; i < rel.primaryKey.size(); i++)
        {
            out << "this->" << rel.attributes[rel.primaryKey[i]].name << "()[tid]";
            if(i != rel.primaryKey.size()-1)
                out << ", ";
        }
        out << ")] = tid;\n";
        out << "\n";
            
        out << "\t}\n\n";
        
        
        
      out << "};\n\n";
      
   }
   
   //print test main
      out << "\n\nint main()\n{\n\twarehouse w{\"./task1/tpcc_warehouse.tbl\"};\n\tstd::cout << \"Warehouse size = \" << w.size() << std::endl;\n\n\treturn 0;\n}";
   
   return out.str();
}

