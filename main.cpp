#include "generatedSchema.cpp"
#include "My_Hashes.hpp"
#include <algorithm>

#include <tuple>
#include <utility>
#include <memory>
#include <fstream>


#include <dlfcn.h>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "parser/Parser.hpp"
#include "QueryParser.hpp"


/*
 * Read Eval Print loop
 * 
 */
void task5(DatabaseColumn& db)
{
    while(1)
    {
        //1. Read query from console
        std::string tmp_query = "select w_id from warehouse;";
        std::cout << "Enter Query:" << std::endl;
        //std::cin.clear();
        //std::cin.sync();
std::getline(std::cin, tmp_query);
std::cout << "tmp_query = " << tmp_query << std::endl;
        //2. analyse query -> report error and abort, 
        //3. generate c++ Code into temporary file "tmp_query.so"
        std::ofstream file("./tmp.cpp");
        
        QueryParser::QueryParser qp;
        
        try {
            auto a = qp.parse(tmp_query);
            //std::cout << a << std::endl;
            //extern "C" int query() {  }
            file << "#include <iostream>\n#include \"generatedSchema.cpp\"\n#include \"Types.hpp\"\n\n\nvoid real_query(DatabaseColumn& db)\n{\n\n";
            file << a;
            file << "}\n\nextern \"C\" void query_wrap(DatabaseColumn& db){ real_query(db); } ";
            file.close();
        } catch (QueryParser::QueryParserError& e) {
            std::cerr << e.what() << std::endl;
        }    
        //4. compile *.so-file
        system("g++ -O3 -fPIC -std=c++14 -g tmp.cpp -shared -o tmp.so -lstdc++fs");
        //load query 
        void* handle=dlopen("./tmp.so",RTLD_NOW);
        if (!handle) {
            std::cerr << "error: " << dlerror() << std::endl;
            exit(1);
        }

        auto fn=reinterpret_cast<void (*)(DatabaseColumn&)>(dlsym(handle, "query_wrap"));
        if (!fn) {
            std::cerr << "error: " << dlerror() << std::endl;
            exit(1);
        }
        //execute query
    //CALL QUERY
        fn(db);

        if (dlclose(handle)) {
            std::cerr << "error: " << dlerror() << std::endl;
            exit(1);
        }
        
        
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
