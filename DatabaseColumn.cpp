#include "DatabaseColumn.hpp"

#include <chrono>
#include <cmath>
 
  //Transactions
  
void DatabaseColumn::printInfo() const
{
    std::cout << "\tCustomer has " << c.size() << " rows!" << std::endl;
    std::cout << "\tDistrict has " << d.size() << " rows!" << std::endl;
    std::cout << "\tHistory has " << h.size() << " rows!" << std::endl;
    std::cout << "\tItem has " << i.size() << " rows!" << std::endl;
    std::cout << "\tNewOrder has " << no.size() << " rows!" << std::endl;
    std::cout << "\tOrder has " << o.size() << " rows!" << std::endl;
    std::cout << "\tOrderLine has " << ol.size() << " rows!" << std::endl;
    std::cout << "\tStock has " << s.size() << " rows!" << std::endl;
    std::cout << "\tWarehouse has " << w.size() << " rows!" << std::endl;
}
  

  
void DatabaseColumn::newOrder(Integer w_id, Integer d_id, Integer c_id, int32_t items, int32_t* supware, int32_t* itemid, int32_t* qty, Timestamp datetime)
{    
    auto w_tax = w.w_tax()[w.getByPrimaryKey(w_id)];  

    auto c_discount = c.c_discount()[c.getByPrimaryKey( c_id, d_id, w_id)];

    auto district_tid = d.getByPrimaryKey(d_id, w_id);

    auto o_id = d.d_next_o_id()[district_tid];
    auto d_tax = d.d_tax()[district_tid];

    //update
    d.d_tax()[district_tid] = d.d_tax()[district_tid] + 1;
    
    //check if all are local -> std::any_if or SSE?
    int32_t all_local = 1;
    for(int32_t index = 0; index < items; index++)
    {
        if(w_id != supware[index])
        all_local = 0;
    }
    //insert data into Order Table
    o.insert(o_id, d_id, w_id, c_id, Date{(int32_t) datetime.value}, 0, Integer{items}, Integer{all_local});
    //insert data into NewOrder Table
    no.insert(o_id, d_id, w_id);
    
    for(int32_t index = 0; index < items; index++)
    {
            
        auto i_price = i.i_price()[i.getByPrimaryKey(itemid[index])];
            
        auto stock_tid = s.getByPrimaryKey(itemid[index], supware[index]);
            
        Char<24> s_dist;
        
        switch(d_id.value)
        {
        case(1):
                s_dist = s.s_dist_01()[stock_tid];
                break;
        case(2):
                s_dist = s.s_dist_02()[stock_tid];
                break;
        case(3):
                s_dist = s.s_dist_03()[stock_tid];
                break;
        case(4):
                s_dist = s.s_dist_04()[stock_tid];
                break;
        case(5):
                s_dist = s.s_dist_05()[stock_tid];
                break;
        case(6):
                s_dist = s.s_dist_06()[stock_tid];
                break;
        case(7):
                s_dist = s.s_dist_07()[stock_tid];
                break;
        case(8):
                s_dist = s.s_dist_08()[stock_tid];
                break;
        case(9):
                s_dist = s.s_dist_09()[stock_tid];
                break;
        case(10):
                s_dist = s.s_dist_10()[stock_tid];
                break;
            default:
                break;
        }
            
        if(s.s_quantity()[stock_tid] > qty[index])
        {
            s.s_quantity()[stock_tid] = s.s_quantity()[stock_tid] - qty[index];
        }
        else
        {
            s.s_quantity()[stock_tid] = s.s_quantity()[stock_tid] + 91 - qty[index];
            
            if(supware[index] != w_id.value)
            {
                s.s_remote_cnt()[stock_tid] = s.s_remote_cnt()[stock_tid] + 1;
            }else
            {
                s.s_remote_cnt()[stock_tid] = s.s_order_cnt()[stock_tid] + 1;
            }
             
            Numeric<4, 0> one{1};
            Numeric<4, 0> one2{1};
            Numeric<4, 0> one3{1};


            auto a = (Numeric<6, 0>{qty[index]}.castP2() * i_price.castS<6>()).castS<8>();//3457,8
            auto b = (one2.castP2().castP2() + w_tax + d_tax).castS<8>();//1,2141
            auto c = ((one.castP2().castP2()  - c_discount)).castS<8>();

            auto tmp_result = (b * c).castM2<8>().castM2<8>() * a;
            
            
            Numeric<6, 2> ol_amount = tmp_result.castM2<8>().castM2<8>().castM2<6>();
            //due to precision instead of 2001,71 its 2430.14
            //            std::cout << "a * b * c =  2001,72 = " << ol_amount << std::endl;          
            //
            ol.insert(o_id, d_id, w_id, Integer{index+1}, Integer{itemid[index]}, Integer{supware[index]}, Date{0}, Numeric<2, 0>(qty[index]), ol_amount, s_dist);
        }
        
    }    

}


 

int32_t DatabaseColumn::urand(int32_t min,int32_t max) {
    return (random()%(max-min+1))+min;
}

int32_t DatabaseColumn::urandexcept(int32_t min,int32_t max,int32_t v) {
    if (max<=min)
	return min;
    int32_t r=(random()%(max-min))+min;
    if (r>=v)
	return r+1; else
	return r;
}

int32_t DatabaseColumn::nurand(int32_t A,int32_t x,int32_t y) {
    return ((((random()%A)|(random()%(y-x+1)+x))+42)%(y-x+1))+x;
}

void DatabaseColumn::newOrderRandom(Timestamp now,int32_t w_id) {
    int32_t d_id=urand(1,10);
    int32_t c_id=nurand(1023,1,3000);
    int32_t ol_cnt=urand(5,15);

    int32_t supware[15];
    int32_t itemid[15];
    int32_t qty[15];
    for (int32_t i=0; i<ol_cnt; i++) {
	if (urand(1,100)>1)
	  supware[i]=w_id; else
	  supware[i]=urandexcept(1,warehouses,w_id);
	itemid[i]=nurand(8191,1,100000);
	qty[i]=urand(1,10);
    }

    newOrder(w_id,d_id,c_id,ol_cnt,supware,itemid,qty,now);
}

void DatabaseColumn::printTask1Info(int numRepeat)
{
    std::cout << "Database - ColumnStore\n" << std::endl;
    std::cout << "\tNewOrder has " << no.size() << " rows!" << std::endl;
    std::cout << "\tOrder has " << o.size() << " rows!" << std::endl;
    std::cout << "\tOrderLine has " << ol.size() << " rows!\n" << std::endl;
      
    std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  
  for(auto i = 0; i < numRepeat; i++)
  {
      newOrderRandom(Timestamp{static_cast<uint64_t>( 40+i)}, 1+(i%5));
  }
  end = std::chrono::system_clock::now();

  std::cout << "\t" << ((double) numRepeat*1000000) / std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << " newOrderRandom/s\n" << std::endl;
  
    std::cout << "\tNewOrder has " << no.size() << " rows!" << std::endl;
    std::cout << "\tOrder has " << o.size() << " rows!" << std::endl;
    std::cout << "\tOrderLine has " << ol.size() << " rows!\n" << std::endl;
  
  
}
