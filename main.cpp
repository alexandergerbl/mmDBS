#include "generatedSchema.cpp"
#include "My_Hashes.hpp"
#include <algorithm>

#include <tuple>
#include <utility>
#include <memory>
#include <fstream>

#include "parser/Parser.hpp"
#include "QueryParser.hpp"



int main()
{
	DatabaseColumn db;

    std::ofstream file("./gen_query.cpp");

    
    QueryParser::QueryParser qp;
    
    try {
        auto a = qp.parse("select w_id from warehouse;");
        //std::cout << a << std::endl;
        file << a;
        file.close();
    } catch (QueryParser::QueryParserError& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;   
}
