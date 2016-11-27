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
#include <chrono>

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
        std::string tmp_query;// = "select c_last, o_id, i_id, ol_dist_info from customer, order, orderline, item where c_id = o_c_id and c_d_id = o_d_id and c_w_id = o_w_id and o_id = ol_o_id and ol_d_id = o_d_id and o_w_id = ol_w_id and ol_number = 1 and ol_o_id = 100 and ol_i_id = i_id;";
        std::cout << "Enter Query:" << std::endl;
        
        std::getline(std::cin, tmp_query);
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
            return;
        } catch (QueryParser::MissingJoinAttributeException& e) {
            std::cerr << e.what() << std::endl;
            return;
        } 
        
        auto start = std::chrono::system_clock::now();
        //4. compile *.so-file
        int status = system("g++ -O3 -fPIC -std=c++14 -g tmp.cpp -shared -o tmp.so -lstdc++fs");
        
        
        auto end = std::chrono::system_clock::now();
        auto compileTime = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
                
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
        
        
        start = std::chrono::system_clock::now();
        
        //execute query
        fn(db);
        
        end = std::chrono::system_clock::now();
        
        //5. measure compilation-time and execution time
        std::cout << "\tcompilation-time:\t" << compileTime << " µs" << std::endl;
        std::cout << "\texecution-time:\t" << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << " µs\n" << std::endl;

        if (dlclose(handle)) {
            std::cerr << "error: " << dlerror() << std::endl;
            exit(1);
        }
        //system("g++ -O3 -std=c++11 -g main.cpp -lrt -ldl -o run");
        
        
    }
}

int main()
{
	DatabaseColumn db;
    
    
std::unordered_multimap<std::tuple<Integer, Integer>, std::tuple< Varchar<10>>, IntIntHash> join_district_order_HT;
for(auto tid = 0; tid < db.m_district.size(); tid++)
{
join_district_order_HT.emplace(std::make_tuple(db.m_district.d_w_id()[tid], db.m_district.d_id()[tid]), std::make_tuple(db.m_district.d_name()[tid]));
}

for(auto tid = 0; tid < db.m_order.size(); tid++)
{
	if(db.m_order.o_id()[tid] == 7)
	{
auto range = join_district_order_HT.equal_range(std::make_tuple(db.m_order.o_w_id()[tid], db.m_order.o_d_id()[tid]));
for(auto it = range.first; it != range.second; ++it)
{

		std::cout << std::get<0>(it->second) << " " << std::endl;
}


	}
}


    task5(db);
    
    return 0;   
}
