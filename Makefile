CC=g++
HEADER=Types.hpp Warehouse.hpp 
CPP=Types.cpp neworderrandom.cpp Warehouse.cpp main.cpp

all:
	$(CC) -g -std=c++17 $(CPP) $(HEADER) -o mmDBS -lstdc++fs
