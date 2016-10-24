#ifndef H_Schema_hpp
#define H_Schema_hpp

#include <vector>
#include <string>
#include "Types.hpp"

struct Schema {
   struct Relation {
      struct Attribute {
         std::string name;
         Types::Tag type;
         unsigned len;
         unsigned len1;
         unsigned len2;
         bool notNull;
         Attribute() : len(~0), notNull(true) {}
      };
      std::string name;
      std::vector<Schema::Relation::Attribute> attributes;
      std::vector<unsigned> primaryKey;
      std::vector<unsigned> nonPrimaryKey;
      std::string relationName_tmp;
      Relation(const std::string& name) : name(name) {}
   };
   //nonPrimaryKey tmp buffer
   std::string relationName;
   std::vector<unsigned> nonprimaryKey;
   //end nonprimaryKey
   std::vector<Schema::Relation> relations;
   std::string toString() const;
};
#endif
