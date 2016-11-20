all:
	g++ -O3 -std=c++14  ColumnStore.hpp My_Hashes.hpp Types.hpp Types.cpp ./parser/Parser.hpp ./parser/Parser.cpp ./parser/Schema.hpp ./parser/Schema.cpp QueryParser.hpp QueryParser.cpp AlgebraOperator.hpp AlgebraOperator.cpp main.cpp -o mmDBS -lstdc++fs
