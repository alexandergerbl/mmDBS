CC=g++
HEADER=Types.hpp Row.hpp Warehouse.hpp WarehouseColumn.hpp Customer.hpp CustomerColumn.hpp District.hpp DistrictColumn.hpp History.hpp HistoryColumn.cpp Item.hpp ItemColumn.hpp NewOrder.hpp NewOrderColumn.hpp Order.hpp OrderColumn.hpp OrderLine.hpp OrderLineColumn.hpp Stock.hpp Database.hpp My_Hashes.hpp ColumnStore.hpp WarehouseColumn.hpp StockColumn.hpp
CPP=Types.cpp Warehouse.cpp WarehouseColumn.cpp Customer.cpp CustomerColumn.cpp District.cpp DistrictColumn.cpp History.cpp HistoryColumn.cpp Item.cpp ItemColumn.cpp NewOrder.cpp NewOrderColumn.cpp Order.cpp OrderColumn.cpp OrderLine.cpp OrderLineColumn.cpp Stock.cpp Database.cpp DatabaseColumn.cpp WarehouseColumn.cpp StockColumn.cpp main.cpp

all:
	$(CC) -g -O3 -Wall -std=c++17 $(CPP) -o mmDBS -lstdc++fs
