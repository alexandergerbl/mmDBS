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
   out << "#include \"ColumnStore.hpp\"\n#include <cstdlib>\n#include <tuple>\n#include<chrono>\n#include<cmath>\n\n";
   
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
      
      //find
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
        out << "\tstd::size_t size() const\n\t{\n\t\treturn std::get<0>(data).size();\n\t}\n\n";

        
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
        out << ")\n\t{\n\t\tauto tid = this->size();\n\n\t\t//if(this->keys.keys.count(std::make_tuple(";
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
   
   
   //create DatabaseColumn class
   out << "\n\nclass DatabaseColumn\n{\npublic:\n";
   
   for(auto & rel : relations)
   {
       out << "\t" << rel.name << " m_" << rel.name << "{\"./task1/tpcc_" << rel.name << ".tbl\"};\n";
   }
   out << std::endl << "const int32_t warehouses=5;";
   out << std::endl << std::endl;
   //Constructor
   out << "\tDatabaseColumn(){};\n\n";
   
   
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
   
   //end of class DatabaseColumn
   out << "};\n\n";
   
   
    
   /*
    * print test main
    */
   out << "\n\nint main()\n{\n\tDatabaseColumn db;\n\tdb.printTask1Info(1000000);\n\n\treturn 0;\n}";
   
   return out.str();
}

