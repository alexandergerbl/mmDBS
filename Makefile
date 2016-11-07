all:
	g++ -O3 -std=c++14  ColumnStore.hpp My_Hashes.hpp Types.hpp Types.cpp main.cpp -o mmDBS -lstdc++fs
