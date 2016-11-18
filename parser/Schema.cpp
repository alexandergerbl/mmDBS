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


std::string Schema::toTestCase() const
{
    std::stringstream out;
    
    for(const Schema::Relation& rel : relations)
    {
        out << std::endl;
        out << "TEST(DatabaseFind, " << rel.name << ")\n{\n";
        
        out << "\t" << rel.name << " " << rel.name.substr(0, 2) << "{\"../../task1/tpcc_" << rel.name << ".tbl\"};\n";
        
        //TODO
        
        out << "}";
        out << std::endl;
    }
    return out.str();
}



/*
 * 
 * 
 * create cpp - code for schema
 * 
 * 
 * 
 * 
 * 
 */

std::string Schema::toCPP() const
{
   std::stringstream out;  
   //header
   out << "#include \"ColumnStore.hpp\"\n#include <cstdlib>\n#include <tuple>\n#include<chrono>\n#include<cmath>\n#include<map>\n#include <utility>\n#include <iterator>\n#include <atomic>\n#include <iostream>\n#include <signal.h>\n#include <sys/types.h>\n#include <sys/wait.h>\n#include <unistd.h>\n#include <exception>\n#include <algorithm>\n#include <iostream>\n#include <tuple>\n#include <utility>\n#include <memory>\n#include <map>\n#include <unordered_map>\n#include <typeinfo>\n#include <exception>\n#include <set>\n\n";
   
   for (const Schema::Relation& rel : relations) {
      out << "class " << rel.name << " : public ColumnStore<";
      for (auto i = 0; i < rel.attributes.size(); i++)
      {
          out << type(rel.attributes[i]);
          if(i != rel.attributes.size()-1)
              out << ", ";
         
      }
      out << ">\n{\n" << ((rel.name == "neworder") ? "public:\n":"");
      
      
      //create primarykey
      if(!rel.primaryKey.empty())
      {
          out << "\nstd::" << ((rel.name == "neworder") ? "map" : "unordered_map") << "<std::tuple<";
          //create multimap for non-primary key 
          for(auto i = 0; i < rel.primaryKey.size(); i++)
          {
              out << type(rel.attributes[rel.primaryKey[i]]);
              if(i != rel.primaryKey.size()-1)
                  out << ", ";
          }
          out << ">, Tid";
          if(rel.name == "neworder")
          {
            out << "> keys;\n\n"; 
          }
          else
          {
             out << ", KeyCompare<" << rel.primaryKey.size() << ", " << "std::tuple<";
            //create multimap for non-primary key 
            for(auto i = 0; i < rel.primaryKey.size(); i++)
            {
                out << type(rel.attributes[rel.primaryKey[i]]);
                if(i != rel.primaryKey.size()-1)
                    out << ", ";
            }
            out << ">>> keys;\n\n";  
          }
          
      }
      
      
      //if foreign key 
      
      if(!rel.nonPrimaryKey.empty())
      {
          out << "\nstd::multimap<std::tuple<";
          //create multimap for non-primary key 
          for(auto i = 0; i < rel.nonPrimaryKey.size(); i++)
          {
              out << type(rel.attributes[rel.nonPrimaryKey[i]]);
              if(i != rel.nonPrimaryKey.size()-1)
                  out << ", ";
          }
          out << ">, Tid> " << rel.nonPrimaryKeyName << ";\n\n";
      }
      
      
      //Constructor
      
      //load PrimaryKey
        out << "public:\n\t" << rel.name << "(std::string file) : ColumnStore(file)\n\t{\n";
        if(!rel.primaryKey.empty())
        {
            out << "\t\tfor(auto i = 0; i < this->size(); i++)\n\t\t{\n";
            //read all nonPrimaryKey attributes from row i 
                out << "\t\t\tthis->keys.emplace(std::make_tuple(";
                for(auto j = 0; j < rel.primaryKey.size(); j++)
                {
                    out << "std::get<" << rel.primaryKey[j] << ">(this->data)[i]";
                    if(j != rel.primaryKey.size()-1)
                        out << ", ";
                }
                out << "), i);";
            out << "\n\t\t}\n";
        }
        //load nonprimarykey
        if(!rel.nonPrimaryKey.empty())
        {
            out << "\t\tfor(auto i = 0; i < this->size(); i++)\n\t\t{\n";
            //read all nonPrimaryKey attributes from row i 
                out << "\t\t\tthis->" << rel.nonPrimaryKeyName << ".emplace(std::make_tuple(";
                for(auto j = 0; j < rel.nonPrimaryKey.size(); j++)
                {
                    out << "std::get<" << rel.nonPrimaryKey[j] << ">(this->data)[i]";
                    if(j != rel.nonPrimaryKey.size()-1)
                        out << ", ";
                }
                out << "), i);";
            out << "\n\t\t}";
        }
        out << "\n\t}\n\n"; 

      
      
      //getter&setter
      int j = 0;
      for (const auto& attr : rel.attributes)
      {
         out << "\tinline auto& " << attr.name << "(){   return std::get<" << j << ">(this->data); }\n";
         j++;
      }
      out << std::endl;
      
      if(!rel.primaryKey.empty())
      {
            //find
            out << "\tTid find(";
            for (auto i = 0; i < rel.primaryKey.size(); i++)
            {
                out << "const " << type(rel.attributes[rel.primaryKey[i]]) <<"& " << rel.attributes[rel.primaryKey[i]].name;
                if(i != rel.primaryKey.size()-1)
                    out << ", ";
            }
            out << ") \n\t{\n\t\treturn this->keys[std::make_tuple(";
            for (auto i = 0; i < rel.primaryKey.size(); i++)
            {
                out << rel.attributes[rel.primaryKey[i]].name;
                if(i != rel.primaryKey.size()-1)
                    out << ", ";
            }
                    
            out << ")];\n\t}\n"; 
      }
      
        
       /**
         * size()
         */    
        out << "\tstd::size_t size() const\n\t{\n\t\treturn std::get<0>(data).size();\n\t}\n\n";

        
        /*******************************************************************
         * Insert
         *******************************************************************/
        out << "\tvoid insert(";
        
        
        for (auto i = 0; i < rel.attributes.size(); i++)
        {
            out << type(rel.attributes[i]) << " " << rel.attributes[i].name;
            if(i != rel.attributes.size()-1)
                out << ", ";
            
        }
        out << ")\n\t{\n\t\tauto tid = this->size();\n\n\t\t//if(this->keys.count(std::make_tuple(";
        for (auto i = 0; i < rel.primaryKey.size(); i++)
        {
            out << rel.attributes[rel.primaryKey[i]].name;
            if(i != rel.primaryKey.size()-1)
                out << ", ";
        }        
        out << ")) == 1)\n\t\t\t//throw std::invalid_argument(\"In relation " << rel.name << ": primary key already taken -> must be unique!\");\n\n";
        //add to each column
        for (auto i = 0; i < rel.attributes.size(); i++)
        {
            out << "\t\tthis->" << rel.attributes[i].name << "().emplace_back(" << rel.attributes[i].name << ");\n"; ;
        }
        
        if(!rel.primaryKey.empty())
        {
            out << "\n\t\tthis->keys[std::make_tuple(";
            for (auto i = 0; i < rel.primaryKey.size(); i++)
            {
                out << rel.attributes[rel.primaryKey[i]].name;
                if(i != rel.primaryKey.size()-1)
                    out << ", ";
            }        
            out << ")] = tid;\n";
        }
        
        
        if(!rel.nonPrimaryKey.empty())
        {
            //update nonprimarykey
            out << "\n\t\tthis->" << rel.nonPrimaryKeyName << ".emplace(std::make_tuple(";
            for (auto i = 0; i < rel.nonPrimaryKey.size(); i++)
            {
                out << rel.attributes[rel.nonPrimaryKey[i]].name;
                if(i != rel.nonPrimaryKey.size()-1)
                    out << ", ";
            }        
            out << "), tid);\n";
            
        }
        
        
        
        out << "\t}\n\n";
        
        /********************************************************
         * Delete
         ********************************************************/
        
        if(!rel.primaryKey.empty())
        {
        
            
        out << "\tvoid deleteEntry(";
        for (auto i = 0; i < rel.primaryKey.size(); i++)
        {
         out << "const " << type(rel.attributes[rel.primaryKey[i]]) <<"& " << rel.attributes[rel.primaryKey[i]].name;
         if(i != rel.primaryKey.size()-1)
             out << ", ";
        }
        out << ")\n\t{\n";
        if(!rel.primaryKey.empty())
        {
            out << "\t\tauto tid = this->find("; 
            for (auto i = 0; i < rel.primaryKey.size(); i++)
            {
                out << rel.attributes[rel.primaryKey[i]].name;
                if(i != rel.primaryKey.size()-1)
                    out << ", ";
            }
            out << ");\n\n";
        
        }
        
        
        //if nonPrimaryKey
        if(!rel.nonPrimaryKey.empty())
        {
            out << "\n\t\t//remove index from nonprimarykey before deletion\n\t\tauto iterpair = this->"<< rel.nonPrimaryKeyName << ".equal_range(std::make_tuple(";
            //for every nonprimary key attribute
            for(auto j = 0; j < rel.nonPrimaryKey.size(); j++)
                {
                    out << "std::get<" << rel.nonPrimaryKey[j] << ">(this->data)[tid]";
                    if(j != rel.nonPrimaryKey.size()-1)
                        out << ", ";
                }
            out << "));\n";
            out << "\t\tauto it = iterpair.first;\n        for (; it != iterpair.second; ++it) {\n            if (it->second == tid) { \n                this->" << rel.nonPrimaryKeyName <<  ".erase(it);\n                break;\n            }\n        }\n";
            
        }
        
        
            //...for all elements
        for (auto i = 0; i < rel.attributes.size(); i++)
        {
            out << "\t\tstd::iter_swap(this->" << rel.attributes[i].name << "().begin()+tid, this->" << rel.attributes[i].name << "().end()-1);\n";
            out << "\t\tthis->" << rel.attributes[i].name << "().pop_back();\n";    
        }
       
        if(!rel.primaryKey.empty())
        {
            out << "\n\t\tthis->keys.erase(std::make_tuple(";
            for (auto i = 0; i < rel.primaryKey.size(); i++)
            {
                out << rel.attributes[rel.primaryKey[i]].name;
                if(i != rel.primaryKey.size()-1)
                    out << ", ";
            }
            out << "));\n";
            //update tid of swapped entry
            out << "\n\t\tthis->keys[std::make_tuple(";
            for (auto i = 0; i < rel.primaryKey.size(); i++)
            {
                out << "this->" << rel.attributes[rel.primaryKey[i]].name << "()[tid]";
                if(i != rel.primaryKey.size()-1)
                    out << ", ";
            }
            out << ")] = tid;\n";
            out << "\n";
                
        }
        
        //if nonPrimaryKey
        if(!rel.nonPrimaryKey.empty())
        {
            //update swapped nonprimarykey
            out << "\n\t\tauto iterpair2 = this->"<< rel.nonPrimaryKeyName << ".equal_range(std::make_tuple(";
            //for every nonprimary key attribute
            for(auto j = 0; j < rel.nonPrimaryKey.size(); j++)
                {
                    out << "std::get<" << rel.nonPrimaryKey[j] << ">(this->data)[tid]";
                    if(j != rel.nonPrimaryKey.size()-1)
                        out << ", ";
                }
            out << "));\n";
            out << "\t\tauto it2 = iterpair2.first;\n        for (; it2 != iterpair2.second; ++it2) {\n            if (it2->second == this->size()) { \n                it2->second = tid;\n                break;\n            }\n        }\n";
            
        }
        
        
        out << "\t}\n\n";
        
            
        }
                
        
      out << "};\n\n";
      
   }
   
   
   out << "\tstatic std::atomic<bool> childRunning_local;\t\t\n\t\t\t\n\tstatic void SIGCHLD_handler(int /*sig*/) {\t\t\n\t   int status;\t\t\n\t   pid_t childPid=wait(&status);\t\t\n\t   // now the child with process id childPid is dead\t\t\n\t   childRunning_local=false;\t\t\n\t}";
   
   
   /*
    * 
    * Create Query AlgebraOperators
    * 
    * 
    */
   out << "\nstruct clause\n{\n    std::string table_name;\n   std::string attribute;\n   std::string table_name2;\n   std::string value;\n   //tells whether second value is a constant\n   bool isConstant;\n};\n\nstruct Attribute\n{\n    std::string table_name;\n    std::string attribute_name;\n    std::string type;\n    \n    bool operator<(Attribute const& rhs) const\n    {\n        if(this->table_name == rhs.table_name)\n            return (this->attribute_name < rhs.attribute_name);\n        return (this->table_name < rhs.table_name);\n    }\n    \n    bool operator==(Attribute const& rhs) const\n    {\n        return (this->table_name == rhs.table_name) && (this->attribute_name == rhs.attribute_name);\n    }\n};\n\nclass Print;\n\nclass AlgebraOperator\n{\npublic:\n    std::shared_ptr<AlgebraOperator> input;\n    std::weak_ptr<AlgebraOperator> parent;\n\n\n    virtual void produce(std::shared_ptr<AlgebraOperator> parent) = 0;\n    virtual void consume(std::shared_ptr<AlgebraOperator> curr) = 0;\n    //look down the algebra tree and look whats pushed upwards\n    virtual std::vector<Attribute> producesAttr() = 0;\n    //look up the algebra tree and get what the operators above want you to produce\n    virtual std::vector<Attribute> requires() = 0;\n    \n    //avoid empty weak_ptr \n    virtual void setParent(std::shared_ptr<AlgebraOperator> sp) = 0;\n};\n\n\n/*\n * \n * HashJoin\n * \n * \n * \n */\nclass HashJoin : public std::enable_shared_from_this<HashJoin>, public AlgebraOperator\n{\npublic:\n    std::shared_ptr<AlgebraOperator> left;\n    std::shared_ptr<AlgebraOperator> right;\n    \n    std::weak_ptr<AlgebraOperator> parent;\n    \n    std::vector<std::pair<Attribute, Attribute>> join_attributes;\n    \n    HashJoin(std::shared_ptr<AlgebraOperator> left, std::shared_ptr<AlgebraOperator> right, std::vector<std::pair<Attribute, Attribute>> join_attributes) : left{left}, right{right}, join_attributes{join_attributes} {}\n\n    \n    void setParent(std::shared_ptr<AlgebraOperator> sp) override\n    {\n        this->parent = sp;\n        this->left->setParent(shared_from_this());\n        this->right->setParent(shared_from_this());\n    }\n    \n    \n    std::vector<Attribute> producesAttr() override\n    {\n        std::set<Attribute> produced;\n     \n        //get produced attributes\n        auto left_tmp = left->producesAttr();\n        auto right_tmp = right->producesAttr();\n        \n        //if left child is join, then only left side\n        if(std::dynamic_pointer_cast<HashJoin>(left) == nullptr)\n        {\n            std::for_each(left_tmp.begin(), left_tmp.end(), [&](Attribute attr){ produced.insert(attr);});\n        \n        \n            std::for_each(right_tmp.begin(), right_tmp.end(), [&](Attribute const& attr){ produced.insert(attr);});\n        }\n        else\n        {\n                   std::for_each(left_tmp.begin(), left_tmp.end(), [&](Attribute attr){ produced.insert(attr);});\n        \n        }\n        \n        std::set<Attribute> required;\n        //get attributes that parents require\n        if(auto sp_parent = this->parent.lock())\n        {\n            auto tmp = sp_parent->requires();\n            std::for_each(tmp.begin(), tmp.end(), [&](Attribute const& attr){required.insert(attr);});   \n        }\n        else\n        {\n         std::cout << \"\\n\\nERRORRRRRRRR print weakptr\" << \"\\n\" << std::endl;         \n        }\n        \n        std::vector<Attribute> intersection;\n\n        std::set_intersection(produced.begin(), produced.end(), required.begin(), required.end(), std::back_inserter(intersection));\n        \n    \n        return intersection;\n    }\n    \n    std::vector<Attribute> requires() override\n    {\n        if(auto sp_parent = this->parent.lock())\n            return sp_parent->requires();\n        throw std::bad_exception();\n    }\n    \n    void produce(std::shared_ptr<AlgebraOperator> parent) override\n    {\n        this->parent = parent;\n        \n        //TODO: Key Compare combine_hash() as templated Functor \n        std::cout << \"std::unordered_multimap<std::tuple<\";\n        //types\n        for(auto i = 0; i < join_attributes.size(); i++)\n        {\n            auto tmp_attr = join_attributes[i].first;\n            std::cout << tmp_attr.type;\n            if(i < join_attributes.size()-1)\n                std::cout << \", \";\n        }\n        //std::cout << \"Integer, Integer, Integer\";\n        \n        //end types\n        std::cout << \">, \";\n        \n        //change to tuple<....> <- specifiy which attributes are needed\n        std::cout << \"std::tuple<\";\n        \n        std::vector<Attribute> value_attributes;\n        if(auto sp_parent = this->parent.lock())\n            value_attributes = sp_parent->requires();\n        else\n            throw std::bad_exception();\n        /*\n         * WATCH OUT PARENT MUST BE SET BEFORE\n         * \n         */\n        auto input_attr_l = this->left->producesAttr();\n       // auto input_attr_r = this->right->producesAttr();\n \n        //remove attributes that arent produced by input\n        value_attributes.erase(std::remove_if(value_attributes.begin(), value_attributes.end(), [&](Attribute const& attr){ \n            auto left_search = std::find(input_attr_l.begin(), input_attr_l.end(), attr);            \n         //   auto right_search = std::find(input_attr_r.begin(), input_attr_r.end(), attr);\n \n            return (left_search == input_attr_l.end());// && (right_search == input_attr_r.end());\n        }), value_attributes.end());\n        \n        for(auto i = 0; i < value_attributes.size(); i++)\n        {\n            std::cout << \" \" << value_attributes[i].type;\n            if(i < value_attributes.size() - 1)\n                std::cout << \", \";\n        }\n        \n        // Insert RESULT Types \n        std::cout << \">\";\n        \n        std::cout << \", IntIntIntHash> \";//TODO combine_hash from num Int\n        \n        std::cout << \"join_\";\n        std::cout << this->join_attributes[0].first.table_name << \"_\";\n        std::cout << this->join_attributes[0].second.table_name << \"_HT;\" << std::endl;\n        left->produce(shared_from_this());\n        right->produce(shared_from_this());\n    \n    }\n    \n    \n    \n    \n    \n    std::vector<Attribute> addToValueOfHT(std::shared_ptr<HashJoin> curr)\n    {\n          std::vector<Attribute> value_attributes;\n        if(auto sp_parent = curr->parent.lock())\n            value_attributes = sp_parent->requires();\n        else\n            throw std::bad_exception();\n        /*\n         * WATCH OUT PARENT MUST BE SET BEFORE\n         * \n         */\n        auto input_attr_l = curr->left->producesAttr();\n        //auto input_attr_r = this->right->producesAttr();\n      \n        //remove attributes that arent produced by input\n        value_attributes.erase(std::remove_if(value_attributes.begin(), value_attributes.end(), [&](Attribute const& attr){ \n            auto left_search = std::find(input_attr_l.begin(), input_attr_l.end(), attr);            \n            //auto right_search = std::find(input_attr_r.begin(), input_attr_r.end(), attr);\n    \n            return (left_search == input_attr_l.end());// && (right_search == input_attr_r.end());\n        }), value_attributes.end());\n  \n        return value_attributes;\n \n    }\n    \n    \n    \n    \n    void consume(std::shared_ptr<AlgebraOperator> curr) override\n    {\n        //only for left\n        if(curr == left)\n        {\n            //if(std::dynamic_pointer_cast<HashJoin>(left) == nullptr)\n            {\n                std::cout << \"join_\";\n                std::cout << this->join_attributes[0].first.table_name << \"_\";\n                std::cout << this->join_attributes[0].second.table_name << \"_HT\";\n                std::cout << \".emplace(std::make_tuple(\";\n                for(auto i = 0; i < this->join_attributes.size(); i++)\n                {\n                    std::cout << \"db.\" << this->join_attributes[i].first.table_name << \".\";\n                    std::cout << this->join_attributes[i].first.attribute_name << \"()[tid]\";\n                    if(i < this->join_attributes.size()-1)\n                        std::cout << \", \";\n                }\n                std::cout << \"), \";\n            if(std::dynamic_pointer_cast<HashJoin>(left) != nullptr)\n            {\n                std::cout << \"std::tuple_cat(it->second, \";\n            }\n                std::cout << \"std::make_tuple(\";\n                //TODO\n    //----------------------------------------------------------\n    // get attributes that will be generated by this operator\n    //----------------------------------------------------------\n      \n        std::vector<Attribute> new_value_attributes = addToValueOfHT(shared_from_this());\n                \n        auto childHash = std::dynamic_pointer_cast<HashJoin>(left);\n          if(childHash != nullptr)\n          {\n              //remove attributes that have been added previously to only add new attrs.\n              std::vector<Attribute> new_value_attributes_old = addToValueOfHT(childHash);\n              \n              //get two sets\n              std::set<Attribute> prev_produced;\n                \n                std::for_each(new_value_attributes_old.begin(), new_value_attributes_old.end(), [&](Attribute attr){ prev_produced.insert(attr);});\n            \n                std::set<Attribute> now_produced;\n                //get attributes that parents require\n                if(auto sp_parent = this->parent.lock())\n                {\n                    std::for_each(new_value_attributes.begin(), new_value_attributes.end(), [&](Attribute const& attr){now_produced.insert(attr);});   \n                }\n              //difference \n              std::vector<Attribute> diff;\n              \n              std::set_difference(now_produced.begin(), now_produced.end(), prev_produced.begin(), prev_produced.end(), std::back_inserter(diff));\n              //remove all that are not element of difference               \n              \n              new_value_attributes.erase(std::remove_if(new_value_attributes.begin(), new_value_attributes.end(), [&](Attribute const& attr){ \n                auto left_search = std::find(diff.begin(), diff.end(), attr);            \n           \n    \n                return (left_search == diff.end());// && (right_search == input_attr_r.end());\n            }), new_value_attributes.end());\n            \n              \n          }\n                \n        for(auto i = 0; i < new_value_attributes.size(); i++)\n        {\n            std::cout << \"db.\" << new_value_attributes[i].table_name << \".\" << new_value_attributes[i].attribute_name << \"()[tid]\";\n            if(i < new_value_attributes.size() - 1)\n                std::cout << \", \";\n        }\n        //----------------------------------------------------------\n                std::cout << \"))\";\n                if(std::dynamic_pointer_cast<HashJoin>(left) != nullptr)\n                    std::cout << \")\" << std::endl;\n                std::cout << \";\" << std::endl;\n            }\n        }\n        else\n        {\n            std::cout <<\"auto range = \";\n            std::cout << \"join_\";\n            std::cout << this->join_attributes[0].first.table_name << \"_\";\n            std::cout << this->join_attributes[0].second.table_name << \"_HT\";\n            \n            std::cout << \".equal_range(std::make_tuple(\";\n            \n            for(auto i = 0; i < this->join_attributes.size(); i++)\n            {\n                std::cout << \"db.\" << this->join_attributes[i].second.table_name << \".\";\n                std::cout << this->join_attributes[i].second.attribute_name << \"()[tid]\";\n                if(i < this->join_attributes.size()-1)\n                    std::cout << \", \";\n            }\n             \n            \n            std::cout << \"));\" << std::endl;\n            std::cout << \"for(auto it = range.first; it != range.second; ++it)\\n{\\n\" << std::endl;\n            \n            \n            if(auto sp_parent = this->parent.lock())\n            {\n                //if(std::dynamic_pointer_cast<Print>(sp_parent) == nullptr)\n                 //       std::cout << \"store t_tmp and t_b in HT_b\" << std::endl;\n                \n                sp_parent->consume(shared_from_this());                \n            }\n            std::cout << \"}\\n\" << std::endl; \n        }\n    }\n};\n\n\n\n\ntypedef uint16_t UI;\nUI counter = 0;\n//key: <table_name, attribute_name>\n//value: UI\nstd::map<UI, std::tuple<std::string, std::string>> uis;\n\n/*\n * \n * TableScan\n * \n * \n * \n */\nclass TableScan : public std::enable_shared_from_this<TableScan>, public AlgebraOperator\n{\npublic:\n    std::weak_ptr<AlgebraOperator> parent;\n    std::string table_name;\n\n    //all attributes of the table\n    std::vector<Attribute> attributes;\n    \n    TableScan(std::string table_name, std::vector<Attribute> attributes) : table_name{table_name}, attributes{attributes} {}\n    \n    void setParent(std::shared_ptr<AlgebraOperator> sp) override\n    {\n        this->parent = sp;\n    }\n    \n    std::vector<Attribute> producesAttr() override\n    {\n        //all attributes of this table\n        return this->attributes;\n    }\n    \n    std::vector<Attribute> requires() override\n    {\n        //always leaf -> should never be called\n        throw std::bad_exception();\n    }\n    \n    void produce(std::shared_ptr<AlgebraOperator> parent) override\n    {\n        this->parent = parent;\n        std::cout << \"for(auto tid = 0; tid < db.\";\n        std::cout << this->table_name << \".size(); tid++)\\n{\" << std::endl;\n        if(auto sp_parent = this->parent.lock())\n            sp_parent->consume(shared_from_this());\n        std::cout << \"}\\n\" << std::endl;\n    }\n    void consume(std::shared_ptr<AlgebraOperator> curr) override\n    {\n        //Nothing to do here\n    }\n};\n\n/*\n * \n * Selection\n * \n * \n * \n */\nclass Selection : public std::enable_shared_from_this<Selection>, public AlgebraOperator\n{\npublic:\n    std::shared_ptr<AlgebraOperator> input;\n    std::weak_ptr<AlgebraOperator> parent;\n    \n    std::vector<clause> clauses;\n    \n    Selection(std::shared_ptr<AlgebraOperator> input, std::vector<clause> clauses) : input{input}, clauses{clauses} {}\n    \n    void setParent(std::shared_ptr<AlgebraOperator> sp) override\n    {\n        this->parent = sp;\n        input->setParent(shared_from_this());\n    }\n    \n    \n    std::vector<Attribute> producesAttr() override\n    {\n        return input->producesAttr();\n    }\n    \n    std::vector<Attribute> requires() override\n    {\n        if(auto sp_parent = this->parent.lock())\n            return sp_parent->requires();\n        else\n            throw std::bad_exception();\n    }\n    \n    void produce(std::shared_ptr<AlgebraOperator> parent) override\n    {\n        this->parent = parent;\n        input->produce(shared_from_this());\n    }\n    void consume(std::shared_ptr<AlgebraOperator> curr) override\n    {\n        std::cout << \"\\tif(\";\n        \n        for(int i = 0; i < this->clauses.size(); i++)\n        {\n                //first always attrobute-name\n                std::cout << \"db.\" << this->clauses[i].table_name << \".\" << this->clauses[i].attribute << \"()[tid]\";\n                std::cout << \" == \";\n                if(this->clauses[i].isConstant)\n                    std::cout << this->clauses[i].value;\n                else\n                    std::cout << \"db.\" << this->clauses[i].table_name2 << \".\" << this->clauses[i].value << \"()[tid]\";\n                \n                if(i < this->clauses.size()-1)\n                    std::cout << \" && \";\n        }\n        \n        std::cout << \")\\n\\t{\" << std::endl;\n        if(auto sp_parent = parent.lock())\n            sp_parent->consume(shared_from_this());\n        std::cout << \"\\n\\t}\" << std::endl;\n    };\n};\n\n/*\n * Root Operator \n * \n * \n */\nclass Print : public std::enable_shared_from_this<Print>, public AlgebraOperator\n{\n    std::shared_ptr<AlgebraOperator> input;\n    std::weak_ptr<AlgebraOperator> parent;\n    //attributes that should be printed\n    std::vector<Attribute> attributes;\npublic:\n    Print(std::shared_ptr<AlgebraOperator> input, std::vector<Attribute> attributes) : input{input}, attributes{attributes} \n    {\n        \n    }\n    \n    void setParent(std::shared_ptr<AlgebraOperator> sp) override\n    {\n        this->parent = sp;\n        input->setParent(shared_from_this());\n    }\n    \n    //dummy wont ever get called, root always topmost\n    std::vector<Attribute> producesAttr() override\n    {\n        return std::vector<Attribute>();\n    }\n    \n    std::vector<Attribute> requires() override\n    {\n        return this->attributes;\n    }\n    \n    \n    void produce(std::shared_ptr<AlgebraOperator> parent) override\n    {\n        parent = parent;\n        input->produce(shared_from_this());\n    };\n    void consume(std::shared_ptr<AlgebraOperator> curr) override\n    {\n        std::cout << \"\\t\\tstd::cout\";\n        \n        auto produced_by_child = input->producesAttr();\n        \n        for(auto i = 0; i < produced_by_child.size(); i++)\n        {\n            std::cout << \" << std::get<\" << i << \">(it->second) << \\\" \\\" \";\n            \n        }\n        \n        for(auto i = produced_by_child.size(); i < this->attributes.size(); i++)\n        {\n            std::cout << \" << db.\" << this->attributes[i].table_name << \".\" << this->attributes[i].attribute_name << \"()[\" << \"tid\" << \"] << \\\" \\\"\";            \n        }\n        std::cout << \" << std::endl;\" << std::endl;\n    };\n};\n";
   
   /*
    * 
    * Database Class
    * 
    */
   //create DatabaseColumn class
   out << "\n\nclass DatabaseColumn\n{\npublic:\n";
   
   for(auto & rel : relations)
   {
       out << "\t" << rel.name << " m_" << rel.name << "{\"./task1/tpcc_" << rel.name << ".tbl\"};\n";
   }
   out << std::endl << "const int32_t warehouses=5;";
   out << std::endl << std::endl;
   //Constructor
   out << "\tDatabaseColumn(){\n";
   
   
   out << "std::map<std::tuple<std::string, std::string>, int> getTableIndex;" << std::endl;
   //HashMap that maps <table_name, attribute_name> -> index within table of attribute
   for(auto& rel: relations)
   {
       int index_generator = 0;
        for(auto& attr : rel.attributes)
        {
            out << "\tgetTableIndex.emplace(std::make_tuple<std::string, std::string>(";
            out << "\"" << rel.name << "\", \"" << attr.name << "\"";
            out << "), " << index_generator << " );" << std::endl;
            index_generator++;
        }
   }
   
   
    out << "\t};\n\n";
   
   
   /*
    * printInfo
    */ 
   out << "\tvoid printInfo() const\n\t{\n";
   
   for(auto & rel : relations)
   {
       out << "\t\tstd::cout << \"" << rel.name << " #entires: \" << m_" << rel.name << ".size() << std::endl;\n";
   }
   
   out << "\t}\n\n";
   
   /*
    * Delivery Transaction
    * 
    */
   
   out << "\tvoid delivery(Integer w_id, Integer o_carrier_id, Timestamp datetime)\n    {\n        Integer d_id{1};\n        for(;d_id <= Integer{10};d_id = d_id + 1)\n        {\n            auto lower_bound = this->m_neworder.keys.lower_bound(std::make_tuple(w_id, d_id, Integer{INT32_MIN}));\n            auto upper_bound = this->m_neworder.keys.lower_bound(std::make_tuple(w_id, d_id, Integer{INT32_MAX}));\n            \n            Integer o_id;\n            \n            if(lower_bound != upper_bound)\n            {\n                //tid\n                auto result_pair = *lower_bound;\n                o_id = m_neworder.no_d_id()[result_pair.second];\n            }else\n            {\n                continue;\n            }\n                \n            m_neworder.deleteEntry(o_id, d_id, w_id); \n        \n            auto o_tid = m_order.find(o_id, d_id, w_id);\n            auto o_ol_cnt = m_order.o_ol_cnt()[o_tid];\n            auto o_c_id = m_order.o_c_id()[o_tid];\n            \n            m_order.o_carrier_id()[o_tid] = o_carrier_id;\n        \n            Numeric<6, 2> ol_total = 0;\n            for(Integer ol_number = 1; ol_number.value <= o_ol_cnt.value; ol_number = ol_number +1)\n            {\n                auto ol_tid = m_orderline.find(o_id, d_id, w_id, ol_number);\n                auto ol_amount = m_orderline.ol_amount()[ol_tid];\n                ol_total = ol_total + ol_amount;\n                m_orderline.ol_delivery_d()[ol_tid] = datetime;\n            }\n                 \n        //TODO achtung primary keys ordnen in schema.sql orderline ! ! !\n            auto tid = m_customer.find(o_c_id, d_id, w_id);\n            m_customer.c_balance()[tid] = m_customer.c_balance()[tid] + ol_total.castS<12>();\n        }\n    }\n\n";
   
   /*
    * 
    * newOrder 
    * 
    */
   out << "void newOrder(Integer w_id, Integer d_id, Integer c_id, int32_t items, int32_t* supware, int32_t* itemid, int32_t* qty, Timestamp datetime)\n{    \n    auto w_tax = m_warehouse.w_tax()[m_warehouse.find(w_id)];  \n\n    auto c_discount = m_customer.c_discount()[m_customer.find( c_id, d_id, w_id)];\n\n    auto district_tid = m_district.find(d_id, w_id);\n\n    auto o_id = m_district.d_next_o_id()[district_tid];\n    auto d_tax = m_district.d_tax()[district_tid];\n\n    //update\n    m_district.d_tax()[district_tid] = m_district.d_tax()[district_tid] + 1;\n    \n    //check if all are local -> std::any_if or SSE?\n    int32_t all_local = 1;\n    for(int32_t index = 0; index < items; index++)\n    {\n        if(w_id != supware[index])\n        all_local = 0;\n    }\n    //insert data into Order Table\n    m_order.insert(o_id, d_id, w_id, c_id, datetime, 0, Integer{items}, Integer{all_local});\n    //insert data into NewOrder Table\n    m_neworder.insert(o_id, d_id, w_id);\n    \n    for(int32_t index = 0; index < items; index++)\n    {\n            \n        auto i_price = m_item.i_price()[m_item.find(itemid[index])];\n            \n        auto stock_tid = m_stock.find(itemid[index], supware[index]);\n            \n        Char<24> s_dist;\n        \n        switch(d_id.value)\n        {\n        case(1):\n                s_dist = m_stock.s_dist_01()[stock_tid];\n                break;\n        case(2):\n                s_dist = m_stock.s_dist_02()[stock_tid];\n                break;\n        case(3):\n                s_dist = m_stock.s_dist_03()[stock_tid];\n                break;\n        case(4):\n                s_dist = m_stock.s_dist_04()[stock_tid];\n                break;\n        case(5):\n                s_dist = m_stock.s_dist_05()[stock_tid];\n                break;\n        case(6):\n                s_dist = m_stock.s_dist_06()[stock_tid];\n                break;\n        case(7):\n                s_dist = m_stock.s_dist_07()[stock_tid];\n                break;\n        case(8):\n                s_dist = m_stock.s_dist_08()[stock_tid];\n                break;\n        case(9):\n                s_dist = m_stock.s_dist_09()[stock_tid];\n                break;\n        case(10):\n                s_dist = m_stock.s_dist_10()[stock_tid];\n                break;\n            default:\n                break;\n        }\n            \n        if(m_stock.s_quantity()[stock_tid] > qty[index])\n        {\n            m_stock.s_quantity()[stock_tid] = m_stock.s_quantity()[stock_tid] - qty[index];\n        }\n        else\n        {\n            m_stock.s_quantity()[stock_tid] = m_stock.s_quantity()[stock_tid] + 91 - qty[index];\n            \n            if(supware[index] != w_id.value)\n            {\n                m_stock.s_remote_cnt()[stock_tid] = m_stock.s_remote_cnt()[stock_tid] + 1;\n            }else\n            {\n                m_stock.s_remote_cnt()[stock_tid] = m_stock.s_order_cnt()[stock_tid] + 1;\n            }\n             \n            Numeric<4, 0> one{1};\n            Numeric<4, 0> one2{1};\n            Numeric<4, 0> one3{1};\n\n\n            auto a = (Numeric<6, 0>{qty[index]}.castP2() * i_price.castS<6>()).castS<8>();//3457,8\n            auto b = (one2.castP2().castP2() + w_tax + d_tax).castS<8>();//1,2141\n            auto c = ((one.castP2().castP2()  - c_discount)).castS<8>();\n\n            auto tmp_result = (b * c).castM2<8>().castM2<8>() * a;\n            \n            \n            Numeric<6, 2> ol_amount = tmp_result.castM2<8>().castM2<8>().castM2<6>();\n            //due to precision instead of 2001,71 its 2430.14\n            //            std::cout << \"a * b * c =  2001,72 = \" << ol_amount << std::endl;          \n            //\n            m_orderline.insert(o_id, d_id, w_id, Integer{index+1}, Integer{itemid[index]}, Integer{supware[index]}, Timestamp{0}, Numeric<2, 0>(qty[index]), ol_amount, s_dist);\n        }\n        \n    }    \n\n}\n";
   
   
   /*
    * urand
    */
   out << "int32_t urand(int32_t min,int32_t max) {\n\treturn (random()%(max-min+1))+min;\n}\n";
   
   /*
    * urandexcept
    */
   out << "int32_t urandexcept(int32_t min,int32_t max,int32_t v) {\n    if (max<=min)\n\treturn min;\n    int32_t r=(random()%(max-min))+min;\n    if (r>=v)\n\treturn r+1; else\n\treturn r;\n}\n\n";
   
   /*
    * nurand
    */
   out << "int32_t nurand(int32_t A,int32_t x,int32_t y) {\n    return ((((random()%A)|(random()%(y-x+1)+x))+42)%(y-x+1))+x;\n}\n\n";
   
   /*
    * newOrderRandom
    */
   out << "\tvoid newOrderRandom(Timestamp now,int32_t w_id) {\n    int32_t d_id=urand(1,10);\n    int32_t c_id=nurand(1023,1,3000);\n    int32_t ol_cnt=urand(5,15);\n\n    int32_t supware[15];\n    int32_t itemid[15];\n    int32_t qty[15];\n    for (int32_t i=0; i<ol_cnt; i++) {\n\tif (urand(1,100)>1)\n\t  supware[i]=w_id; else\n\t  supware[i]=urandexcept(1,warehouses,w_id);\n\titemid[i]=nurand(8191,1,100000);\n\tqty[i]=urand(1,10);\n    }\n\n    newOrder(w_id,d_id,c_id,ol_cnt,supware,itemid,qty,now);\n}\n\n";
   
   /*
    * printInfoTask1
    */
   out << "\tvoid printTask1Info(int numRepeat)\n\t{";
   
   out << "    std::cout << \"Database - ColumnStore\\n\" << std::endl;\n    std::cout << \"\\tNewOrder has \" << m_neworder.size() << \" rows!\" << std::endl;\n    std::cout << \"\\tOrder has \" << m_order.size() << \" rows!\" << std::endl;\n    std::cout << \"\\tOrderLine has \" << m_orderline.size() << \" rows!\\n\" << std::endl;\n\n";
   
   out << "\tstd::chrono::time_point<std::chrono::system_clock> start, end;\n  start = std::chrono::system_clock::now();\n  \n\tfor(auto i = 0; i < numRepeat; i++)\n\t{\n\t\t      newOrderRandom(Timestamp{static_cast<uint64_t>( 40+i)}, 1+(i%5));\n  }\n\tend = std::chrono::system_clock::now();\n\n\tstd::cout << \"\\t\" << ((double) numRepeat*1000000) / std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << \" newOrderRandom/s\\n\" << std::endl;\n  \n    std::cout << \"\\tNewOrder has \" << m_neworder.size() << \" rows!\" << std::endl;\n    std::cout << \"\\tOrder has \" << m_order.size() << \" rows!\" << std::endl;\n    std::cout << \"\\tOrderLine has \" << m_orderline.size() << \" rows!\\n\" << std::endl;\n  ";
   
   out << "\t}\n\n";
   
   
   /*
    * deliveryRandom
    */
   out << "\n    void deliveryRandom(Timestamp now) {\n        delivery(urand(1,warehouses),urand(1,10),now);\n    }\n   ";
   
   /*
    * 
    * printInfoTask2
    * 
    */
   out << "\n    void oltp(Timestamp now, int i) {\n        int rnd=urand(1,100);\n        if (rnd<=10) {\n            deliveryRandom(now);\n        } else {\n            newOrderRandom(now, i);\n        }\n    }\n    \n    void printTask2Info(int numRepeat)\n    {\n        std::cout << \"Database - ColumnStore - task 2\\n\" << std::endl;\n        std::chrono::time_point<std::chrono::system_clock> start, end;\n        start = std::chrono::system_clock::now();\n        \n        for(auto i = 0; i < numRepeat; i++)\n        {\n            oltp(Timestamp{static_cast<uint64_t>( 40+i)}, 1+(i%5));\n        }\n        end = std::chrono::system_clock::now();\n\n        std::cout << \"\\t\" << ((double) numRepeat*1000000) / std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << \" Ops/s\\n\" << std::endl;\n    }\n\n";
 
   
   /*
    * Task 3 - hashjoin impl
    * 
    */
   out << "    Numeric<12, 2> hashJoinTask3()\n    {       \n        \n        std::unordered_map<std::tuple<Integer, Integer, Integer>, std::pair<Tid, Tid>, IntIntIntHash> customer_HT;\n        \n        for(auto tid = 0; tid < m_customer.size(); tid++)\n        {\n            auto c_last_tmp = m_customer.c_last()[tid];\n            \n            if(c_last_tmp.value[0] == 'B')\n            {\n                auto c_id = m_customer.c_id()[tid];\n                auto c_d_id = m_customer.c_d_id()[tid];\n                auto c_w_id = m_customer.c_w_id()[tid];\n                \n                customer_HT.emplace(std::make_tuple(c_d_id, c_w_id, c_id), std::make_pair(tid, 0));\n            }\n        }\n        \n        \n            //std::pair<tid of customer, tid of order>\n        std::unordered_map<std::tuple<Integer, Integer, Integer>, std::pair<Tid, Tid>, IntIntIntHash> resultJoin1;    \n        \n        for(auto tid = 0; tid < m_order.size(); tid++)\n        {\n            auto o_c_id = m_order.o_c_id()[tid];\n            auto o_d_id = m_order.o_d_id()[tid];\n            auto o_w_id = m_order.o_w_id()[tid];\n            auto o_id = m_order.o_id()[tid];\n            \n            auto tid_customer_it = customer_HT.find(std::make_tuple(o_d_id, o_w_id, o_c_id));\n            \n            if(tid_customer_it != customer_HT.end())\n            {\n                resultJoin1.emplace(std::make_tuple(o_id, o_d_id, o_w_id), std::make_pair(tid_customer_it->second.first, tid)); \n            }\n        }\n        \n        //second join\n        Numeric<12, 2> sum{0};\n        \n        for(auto ol_tid = 0; ol_tid < m_orderline.size(); ol_tid++)\n        {\n            //get range of possible fitting keys\n            auto o_id = m_orderline.ol_o_id()[ol_tid];\n            auto o_d_id = m_orderline.ol_d_id()[ol_tid];\n            auto o_w_id = m_orderline.ol_w_id()[ol_tid];\n            \n            auto search = resultJoin1.find(std::make_tuple(o_id, o_d_id, o_w_id));\n            \n            if(resultJoin1.end() != search)\n            {\n                    // sum += ol_quality * ol_amount - c_balance*o_ol_cnt\n                    auto a = m_orderline.ol_quantity()[ol_tid].castP2().castS<12>() * m_orderline.ol_amount()[ol_tid].castS<12>();\n                    auto b = m_customer.c_balance()[search->second.first] * m_order.o_ol_cnt()[search->second.second].castS<12>().castP2();\n                \n                    sum = sum + a.castM2<12>() - b.castM2<12>();\n            }\n        \n        }\n        return sum;\n    }\n";
   
   
   
   /*
    * Fork - task3 - hashjoin - Snapshot Model
    * 
    * 
    */
   out << "\n    int fork_task3_hashjoin()\n    {\n            struct sigaction sa;\n            sigemptyset(&sa.sa_mask);\n            sa.sa_flags=0;\n            sa.sa_handler=SIGCHLD_handler;\n            sigaction(SIGCHLD,&sa,NULL);\n\n            int numTA = 1000000;\n            \n            std::chrono::time_point<std::chrono::system_clock> start, end;\n            \n            int64_t fork_avg = 0;\n            int64_t numFork = 0;\n            \n            \n            int fd[2];\n            int status = pipe(fd);\n            \n            if(status != 0)\n                throw std::runtime_error(\"Pipe couldn't be created!\");\n            \n            int query_avg = 0;\n            int numQueries = 0;\n            \n            int TA_time = 0;\n            \n            bool finishedTA = false;\n    \t    thread_local int ka = 0;        \n            while(!finishedTA) \n            {\n                childRunning_local=true;\n                \n                start = std::chrono::system_clock::now();\n                pid_t pid=fork();\n                end = std::chrono::system_clock::now();\n                \n                fork_avg += std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();\n                numFork++;\n                \n                //numQueries++;\n                \n                start = std::chrono::system_clock::now();\n                if (pid) { // parent\n                    //exec 1 mio transaction\n                    while(ka < numTA)\n                    {\n                        if(!childRunning_local)\n                        {\n                            //reading only\n                            close(fd[1]);\n                            decltype(std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()) val; \n                            status = read(fd[0], &val, sizeof(val));\n                            \n                            query_avg += val;\n                            numQueries++;\n                            break;\n                        }\n                        oltp(Timestamp{static_cast<uint64_t>( 40+ka)}, 1+(ka%5));\n                        \n                        ka++;\n                        \n                            \n                    }\n                    \n                    if(ka >= numTA)\n                        finishedTA = true;\n                } else { // forked child\n                    //only writing -> close read descriptor\n                    close(fd[0]);\n                    \n                    \n                    //start hashJoinTask3 - Query\n                    start = std::chrono::system_clock::now();\n                    this->hashJoinTask3();\n                    end = std::chrono::system_clock::now();\n                    auto val = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();\n                    \n                    status = write(fd[1], &val, sizeof(val));\n                    \n                    //close write descriptor\n                    close(fd[1]);\n                    \n                    return 0;\n                    // child is finished\n                }\n                end = std::chrono::system_clock::now();\n                TA_time += std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();\n            }\n            \n            //PRINT RESULT\n            std::cout << \"\\t\" << ((double)numTA*1000000)/TA_time << \" TA/sec\" << std::endl;\n            std::cout << \"\\t\" << (int)((double)query_avg)/numQueries << \"µs per query\" << std::endl;\n            std::cout << \"\\t\" << fork_avg/numFork << \"µs per fork\" << std::endl;\n\n    }\n";
   
   
   
   /*
    * 
    * printInfoTask3
    * 
    */
   out << "\n\tvoid printInfoTask3()\n    {\n        int numRepeat = 10;\n        \n        //auto result = hashJoinTask3(db);\n        \n        std::cout << \"Database - ColumnStore - task 3\\n\" << std::endl;\n        std::chrono::time_point<std::chrono::system_clock> start, end;\n        \n        for(auto i = 0; i < numRepeat; i++)\n        {\n            start = std::chrono::system_clock::now();\n        \n            auto result = hashJoinTask3();\n            \n            end = std::chrono::system_clock::now();\n            std::cout << (i+1) << \"\\t\" << result << \"\\t\" << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << \" µs/hashjoin\" << std::endl;\n        }\n        std::cout << \"\\nSnapshotting - Measurements\" << std::endl;\n        \n        fork_task3_hashjoin();\n        \n    }";
   
   /*
    * 
    * printInfoTask4
    * 
    */
   out << "\n\n    void printInfoTask4()\n    {\n        std::vector<Attribute> attributes_customer = {\n        {\"m_customer\", \"c_id\", \"Integer\"},\n        {\"m_customer\", \"c_d_id\", \"Integer\"},\n        {\"m_customer\", \"c_w_id\", \"Integer\"},\n        {\"m_customer\", \"c_first\", \"Varchar<16>\"},\n        {\"m_customer\", \"c_middle\", \"Char<2>\"},\n        {\"m_customer\", \"c_last\", \"Varchar<16>\"},\n        {\"m_customer\", \"c_street_1\", \"Varchar<20>\"},\n        {\"m_customer\", \"c_street_2\", \"Varchar<20>\"}, \n        {\"m_customer\", \"c_city\", \"Varchar<20>\"},\n        {\"m_customer\", \"c_state\", \"Char<2>\"},\n        {\"m_customer\", \"c_zip\", \"Char<9>\"},\n        {\"m_customer\", \"c_phone\", \"Char<16>\"},\n        {\"m_customer\", \"c_since\", \"Timestamp\"}, \n        {\"m_customer\", \"c_credit\", \"Char<2>\"}, \n        {\"m_customer\", \"c_credit_lim\", \"Numeric<12, 2>\"},\n        {\"m_customer\", \"c_discount\", \"Numeric<4, 4>\"},\n        {\"m_customer\", \"c_balance\", \"Numeric<12, 2>\"},\n        {\"m_customer\", \"c_ytd_paymenr\", \"Numeric<12, 2>\"},\n        {\"m_customer\", \"c_payment_cnt\", \"Numeric<4, 0>\"}, \n        {\"m_customer\", \"c_delivery_cnt\", \"Numeric<4, 0>\"},\n        {\"m_customer\", \"c_data\", \"Varchar<500>\"}\n        };\n        \n        std::vector<Attribute> attributes_order ={\n            {\"m_order\", \"o_id\", \"Integer\"},\n            {\"m_order\", \"o_d_id\", \"Integer\"},\n            {\"m_order\", \"o_w_id\", \"Integer\"},\n            {\"m_order\", \"o_c_id\", \"Integer\"},\n            {\"m_order\", \"o_entry_d\", \"Timestamp\"},\n            {\"m_order\", \"o_carrier_id\", \"Integer\"},\n            {\"m_order\", \"o_ol_cnt\", \"Numeric<2, 0>\"},\n            {\"m_order\", \"o_all_local\", \"Numeric<1, 0>\"}\n        };\n        \n        std::vector<Attribute> attributes_orderline = {\n            {\"m_orderline\", \"ol_o_id\", \"Integer\"},\n            {\"m_orderline\", \"ol_d_id\", \"Integer\"},\n            {\"m_orderline\", \"ol_w_id\", \"Integer\"},\n            {\"m_orderline\", \"ol_number\", \"Integer\"},\n            {\"m_orderline\", \"ol_i_id\", \"Integer\"},\n            {\"m_orderline\", \"ol_supply_w_id\", \"Integer\"},\n            {\"m_orderline\", \"ol_delivery_d\", \"Timestamp\"},\n            {\"m_orderline\", \"ol_quantity\", \"Numeric<2, 0>\"},\n            {\"m_orderline\", \"ol_amount\", \"Numeric<6, 2>\"},\n            {\"m_orderline\", \"ol_dist_info\", \"Char<24>\"}\n        };\n        \n        \n        auto customer = std::make_shared<TableScan>(\"m_customer\", attributes_customer);\n        auto order = std::make_shared<TableScan>(\"m_order\", attributes_order);\n        auto orderline = std::make_shared<TableScan>(\"m_orderline\", attributes_orderline);\n        \n        std::vector<clause> clauses = {{\"m_customer\", \"c_d_id\", \"\", \"1\", true}, {\"m_customer\", \"c_w_id\", \"\", \"1\", true}, {\"m_customer\", \"c_id\", \"\", \"322\", true}};\n        auto select = std::make_shared<Selection>(customer, clauses);\n\n        //1. Join customer and order\n        std::vector<std::pair<Attribute, Attribute>> join_attributes = {{{\"m_customer\", \"c_w_id\", \"Integer\"}, {\"m_order\", \"o_w_id\", \"Integer\"}}, {{\"m_customer\", \"c_d_id\", \"Integer\"}, {\"m_order\", \"o_d_id\", \"Integer\"}}, {{\"m_customer\", \"c_id\", \"Integer\"}, {\"m_order\", \"o_c_id\", \"Integer\"}}};\n        auto join_customer_order = std::make_shared<HashJoin>(select, order, join_attributes);\n    \n        //2. Join customer_order and orderline\n        std::vector<std::pair<Attribute, Attribute>> join_attributes2 = {{{\"m_order\", \"o_w_id\", \"Integer\"}, {\"m_orderline\", \"ol_w_id\", \"Integer\"}}, {{\"m_order\", \"o_d_id\", \"Integer\"}, {\"m_orderline\", \"ol_d_id\", \"Integer\"}}, {{\"m_order\", \"o_id\", \"Integer\"}, {\"m_orderline\", \"ol_o_id\", \"Integer\"}}};\n        auto join_customer_order_orderline = std::make_shared<HashJoin>(join_customer_order, orderline, join_attributes2);\n    \n        std::vector<Attribute> attributes = {{\"m_customer\", \"c_first\", \"Varchar<16>\"}, {\"m_customer\", \"c_last\", \"Varchar<16>\"}, {\"m_order\", \"o_all_local\", \"Numeric<1, 0>\"}, {\"m_orderline\", \"ol_amount\", \"Numeric<6, 2>\"}};\n        auto root = std::make_shared<Print>(join_customer_order_orderline, attributes);\n    \n        root->setParent(nullptr);\n        root->produce(std::shared_ptr<AlgebraOperator>(nullptr));\n    }\n    ";
   
   //end of class DatabaseColumn
   out << "};\n\n";
   
   
   
   
   return out.str();
}

