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
        qp.parse("select c_id, c_first, c_middle, c_last from customer where c_last = 'BARBARBAR';");
        
        //std::cout << qp.generateOperatorTree() << std::endl;
        
    } catch (QueryParser::QueryParserError& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;   
}
