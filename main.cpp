#include "generatedSchema.cpp"
#include "My_Hashes.hpp"
#include <algorithm>

#include <tuple>
#include <utility>


Numeric<12, 2> hashJoin(DatabaseColumn& db)
{       
    
    std::unordered_map<std::tuple<Integer, Integer, Integer>, std::pair<Tid, Tid>, IntIntIntHash> customer_HT;
    
    for(auto tid = 0; tid < db.m_customer.size(); tid++)
    {
        auto c_last_tmp = db.m_customer.c_last()[tid];
        
        if(c_last_tmp.value[0] == 'B')
        {
            auto c_id = db.m_customer.c_id()[tid];
            auto c_d_id = db.m_customer.c_d_id()[tid];
            auto c_w_id = db.m_customer.c_w_id()[tid];
            
            customer_HT.emplace(std::make_tuple(c_d_id, c_w_id, c_id), std::make_pair(tid, 0));
        }
    }
    
    
        //std::pair<tid of customer, tid of order>
    std::unordered_map<std::tuple<Integer, Integer, Integer>, std::pair<Tid, Tid>, IntIntIntHash> resultJoin1;    
    
    for(auto tid = 0; tid < db.m_order.size(); tid++)
    {
        auto o_c_id = db.m_order.o_c_id()[tid];
        auto o_d_id = db.m_order.o_d_id()[tid];
        auto o_w_id = db.m_order.o_w_id()[tid];
        auto o_id = db.m_order.o_id()[tid];
        
        auto tid_customer_it = customer_HT.find(std::make_tuple(o_d_id, o_w_id, o_c_id));
        
        if(tid_customer_it != customer_HT.end())
        {
            resultJoin1.emplace(std::make_tuple(o_id, o_d_id, o_w_id), std::make_pair(tid_customer_it->second.first, tid)); 
        }
    }
    
    //second join
    Numeric<12, 2> sum{0};
    
    for(auto ol_tid = 0; ol_tid < db.m_orderline.size(); ol_tid++)
    {
        //get range of possible fitting keys
        auto o_id = db.m_orderline.ol_o_id()[ol_tid];
        auto o_d_id = db.m_orderline.ol_d_id()[ol_tid];
        auto o_w_id = db.m_orderline.ol_w_id()[ol_tid];
        
        auto search = resultJoin1.find(std::make_tuple(o_id, o_d_id, o_w_id));
        
        if(resultJoin1.end() != search)
        {
                // sum += ol_quality * ol_amount - c_balance*o_ol_cnt
                auto a = db.m_orderline.ol_quantity()[ol_tid].castP2().castS<12>() * db.m_orderline.ol_amount()[ol_tid].castS<12>();
                auto b = db.m_customer.c_balance()[search->second.first] * db.m_order.o_ol_cnt()[search->second.second].castS<12>().castP2();
             
                sum = sum + a.castM2<12>() - b.castM2<12>();
            
        }
     
    }

    
    return sum;


}



int main()
{
 
	DatabaseColumn db;
    
    int numRepeat = 10;
    
    //auto result = hashJoin(db);
    
    std::cout << "Database - ColumnStore - task 3\n" << std::endl;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    
    for(auto i = 0; i < numRepeat; i++)
    {
        start = std::chrono::system_clock::now();
    
        auto result = hashJoin(db);
        
        end = std::chrono::system_clock::now();
        std::cout << i << "\t" << result << "\t" << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << " µs/hashjoin\n" << std::endl;
    }
    

    


    return 0;   
}
