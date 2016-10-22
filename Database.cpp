#include "Database.hpp"

#include <chrono>
#include <cmath>
 
  //Transactions
  
void Database::printInfo() const
{
    std::cout << "Database - RowStore\n" << std::endl;
    std::cout << "Customer has " << c.rows.size() << " rows!" << std::endl;
    std::cout << "District has " << d.rows.size() << " rows!" << std::endl;
    std::cout << "History has " << h.rows.size() << " rows!" << std::endl;
    std::cout << "Item has " << i.rows.size() << " rows!" << std::endl;
    std::cout << "NewOrder has " << no.rows.size() << " rows!" << std::endl;
    std::cout << "Order has " << o.rows.size() << " rows!" << std::endl;
    std::cout << "OrderLine has " << ol.rows.size() << " rows!" << std::endl;
    std::cout << "Stock has " << s.rows.size() << " rows!" << std::endl;
    std::cout << "Warehouse has " << w.rows.size() << " rows!" << std::endl;
}
  

  
void Database::newOrder(Integer w_id, Integer d_id, Integer c_id, int32_t items, int32_t* supware, int32_t* itemid, int32_t* qty, Timestamp datetime)
{
    auto w_tax = this->w.w_id(w_id).w_tax();
    auto c_discount = this->c.getByPrimaryKey(std::make_tuple(w_id, d_id, c_id)).c_discount();
    Row_District& tmp_district = this->d.getByPrimaryKey(std::make_pair(w_id, d_id));
    auto o_id = tmp_district.d_next_o_id();
    auto d_tax = tmp_district.d_tax();
    //update
    tmp_district.d_next_o_id() = tmp_district.d_next_o_id() + 1;

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
        auto i_price = i.getByPrimaryKey(itemid[index]).i_price();
        
        Row_Stock& tmp_s = s.getByPrimaryKey(std::make_pair(supware[index], itemid[index]));
        Char<24> s_dist;
        
        switch(d_id.value)
        {
        case(1):
                s_dist = tmp_s.s_dist_01();
                break;
        case(2):
                s_dist = tmp_s.s_dist_02();
                break;
        case(3):
                s_dist = tmp_s.s_dist_03();
                break;
        case(4):
                s_dist = tmp_s.s_dist_04();
                break;
        case(5):
                s_dist = tmp_s.s_dist_05();
                break;
        case(6):
                s_dist = tmp_s.s_dist_06();
                break;
        case(7):
                s_dist = tmp_s.s_dist_07();
                break;
        case(8):
                s_dist = tmp_s.s_dist_08();
                break;
        case(9):
                s_dist = tmp_s.s_dist_09();
                break;
        case(10):
                s_dist = tmp_s.s_dist_10();
                break;
            default:
                break;
        }
        
        if(tmp_s.s_quantity() > qty[index])
        {
            tmp_s.s_quantity() = tmp_s.s_quantity() - qty[index];
        }
        else
        {
            tmp_s.s_quantity() = tmp_s.s_quantity() + 91 - qty[index];
            
            if(supware[index] != w_id.value)
            {
                tmp_s.s_remote_cnt() = tmp_s.s_remote_cnt() + 1;
            }else
            {
                tmp_s.s_remote_cnt() = tmp_s.s_order_cnt() + 1;
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


 

int32_t Database::urand(int32_t min,int32_t max) {
    return (random()%(max-min+1))+min;
}

int32_t Database::urandexcept(int32_t min,int32_t max,int32_t v) {
    if (max<=min)
	return min;
    int32_t r=(random()%(max-min))+min;
    if (r>=v)
	return r+1; else
	return r;
}

int32_t Database::nurand(int32_t A,int32_t x,int32_t y) {
    return ((((random()%A)|(random()%(y-x+1)+x))+42)%(y-x+1))+x;
}

void Database::newOrderRandom(Timestamp now,int32_t w_id) {
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

void Database::printTask1Info(int numRepeat)
{
    std::cout << "NewOrder has " << no.rows.size() << " rows!" << std::endl;
    std::cout << "Order has " << o.rows.size() << " rows!" << std::endl;
    std::cout << "OrderLine has " << ol.rows.size() << " rows!\n" << std::endl;
      
    std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  
  for(auto i = 0; i < numRepeat; i++)
  {
      newOrderRandom(Timestamp{static_cast<uint64_t>( 40+i)}, i%5);
  }
  end = std::chrono::system_clock::now();

  std::cout << ((double) numRepeat*1000000) / std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << " newOrderRandom/s\n" << std::endl;
  
    std::cout << "NewOrder has " << no.rows.size() << " rows!" << std::endl;
    std::cout << "Order has " << o.rows.size() << " rows!" << std::endl;
    std::cout << "OrderLine has " << ol.rows.size() << " rows!" << std::endl;
  
  
}
