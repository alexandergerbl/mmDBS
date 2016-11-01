all:
	g++ -O3 -std=c++14  generatedSchema.cpp ColumnStore.hpp My_Hashes.hpp Types.hpp Types.cpp -o mmDBS -lstdc++fs
