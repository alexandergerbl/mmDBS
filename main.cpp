#include "generatedSchema.cpp"
#include "My_Hashes.hpp"
#include <algorithm>

#include <tuple>
#include <utility>
#include <memory>

#include "parser/Parser.hpp"
#include "QueryParser.hpp"

int main()
{
	DatabaseColumn db;

    QueryParser::QueryParser qp;
    
    try {
        qp.parse("select w_id from warehouse;");
    } catch (QueryParser::QueryParserError& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;   
}
