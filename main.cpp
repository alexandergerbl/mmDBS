#include "generatedSchema.cpp"
#include "My_Hashes.hpp"
#include <algorithm>

#include <tuple>
#include <utility>
#include <memory>
#include <fstream>

#include <cstdlib>

#include "parser/Parser.hpp"
#include "QueryParser.hpp"


/*
 * Read Eval Print loop
 * 
 */
void task5(DatabaseColumn const& db)
{
    //while(1)
    {
        //1. Read query from console
        std::string tmp_query = "select w_id from warehouse;";
        std::cout << "Enter Query:" << std::endl;
        //std::cin.clear();
        //std::cin.sync();
std::getline(std::cin, tmp_query, '\n');
std::cout << "tmp_query = " << tmp_query << std::endl;
        //2. analyse query -> report error and abort, 
        //3. generate c++ Code into temporary file "tmp_query.so"
        std::ofstream file("./tmp.cpp");
        
        QueryParser::QueryParser qp;
        
        try {
            auto a = qp.parse(tmp_query);
            //std::cout << a << std::endl;
            //extern "C" int query() {  }
            file << "#include \"generatedSchema.cpp\"\n\nextern \"C\" int query(DatabaseColumn const& db)\n{\n\n";
            file << a;
            file << "}\n";
            file.close();
        } catch (QueryParser::QueryParserError& e) {
            std::cerr << e.what() << std::endl;
        }    
        //4. compile *.so-file
        system("g++ -O3 -fPIC -std=c++14 -g tmp.cpp -shared -o tmp.so -lstdc++fs");
        //load query 
        
        //execute query
        
        //system("g++ -O3 -std=c++11 -g main.cpp -lrt -ldl -o run");
        //5. measure compilation-time and execution time
        
    }
}

int main()
{
	DatabaseColumn db;

    task5(db);
    
    return 0;   
}
