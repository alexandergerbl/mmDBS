#ifndef H_Schema_hpp
#define H_Schema_hpp

#include <vector>
#include <string>
#include "Types.hpp"

namespace SQL{

    struct Schema {
    struct Relation {
        struct Attribute {
            std::string name;
            SchemaParser::Types::Tag type;
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
        std::string nonPrimaryKeyName;
        Relation(const std::string& name) : name(name) {}
    };
    //nonPrimaryKey tmp buffer
    std::string relationName;
    std::vector<unsigned> nonprimaryKey;
    //end nonprimaryKey
    std::vector<Schema::Relation> relations;
    std::string toString() const;
    
    std::string toTestCase() const;
    
    std::string toCPP() const;
    
    bool isAttribute(std::string attr) const;
    bool isTableName(std::string attr) const;
    std::string getTableName(std::string attribute) const;
    };

}

#endif
