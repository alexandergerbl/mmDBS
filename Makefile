CC=g++
HEADER=Types.hpp Row.hpp Warehouse.hpp Customer.hpp District.hpp History.hpp Item.hpp NewOrder.hpp Order.hpp OrderLine.hpp Stock.hpp Database.hpp My_Hashes.hpp
CPP=Types.cpp Warehouse.cpp Customer.cpp District.cpp History.cpp Item.cpp NewOrder.cpp Order.cpp OrderLine.cpp Stock.cpp Database.cpp main.cpp

all:
	$(CC) -g -O2 -Wall -std=c++17 $(CPP) $(HEADER) -o mmDBS -lstdc++fs
