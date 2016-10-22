#include "WarehouseColumn.hpp"

#include<fstream>
#include<experimental/filesystem>



WarehouseColumn::WarehouseColumn(std::string file) : ColumnStore<1, Integer, Varchar<10>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Numeric<4, 4>, Numeric<12, 2>>(file)
{   
    
}


