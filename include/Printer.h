// -----------------------------------------------------------------------------
// 5iar -- Printer.h
// -----------------------------------------------------------------------------
//
// ...
//

#ifndef __FIAR_PRINTER_H__
#define __FIAR_PRINTER_H__

#include <vector>
#include <string>

namespace FIAR
{

class Printer
{
public:
    // constructor
    Printer() {}

    void print_array(const std::vector<std::vector<char>>& array);

private:
    void print_array_top_row(size_t size);
    void print_array_bottom_row(size_t size);
    void print_array_empty_row(size_t size);
    void print_array_filled_row(size_t pos, const std::vector<char>& row);
    void print_array_number_row(size_t size);
};

}

#endif // __FIAR_PRINTER_H__
