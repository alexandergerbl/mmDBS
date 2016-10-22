#include "HistoryColumn.hpp"

#include<fstream>
#include<experimental/filesystem>


std::size_t HistoryColumn::size() const
{
    return std::get<0>(data).size();
}
