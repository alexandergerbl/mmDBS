CC=g++
HEADER=Types.hpp Warehouse.hpp 
CPP=Types.cpp neworderrandom.cpp Warehouse.cpp main.cpp

all:
	$(CC) -g -std=c++11 $(CPP) $(HEADER) -o mmDBS
