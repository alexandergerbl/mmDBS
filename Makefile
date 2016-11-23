all:
	g++ -rdynamic -O3 -std=c++14 -L. ColumnStore.hpp My_Hashes.hpp Types.hpp Types.cpp ./parser/Parser.hpp ./parser/Parser.cpp ./parser/Schema.hpp ./parser/Schema.cpp QueryParser.hpp QueryParser.cpp AlgebraOperator.hpp AlgebraOperator.cpp main.cpp -o mmDBS -lrt -ldl  -lstdc++fs
