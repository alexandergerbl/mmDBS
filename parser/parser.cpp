#include <iostream>
#include <memory>
#include "Parser.hpp"
#include "Schema.hpp"

int main(int argc, char* argv[]) {
   if (argc != 2) {
      std::cerr << "usage: " << argv[0] << " <schema file>" << std::endl;
      return -1;
   }

   SchemaParser::Parser p(argv[1]);
   try {
      std::unique_ptr<SQL::Schema> schema = p.parse();
      std::cout << schema->toCPP() << std::endl;
   } catch (SchemaParser::ParserError& e) {
      std::cerr << e.what() << std::endl;
   }
   return 0;
}
