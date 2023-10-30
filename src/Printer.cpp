#include "Printer.h"

#include <iostream>
#include <stdio.h>

namespace FIAR
{

void Printer::print_array(const std::vector<std::vector<char>>& array)
{
    std::cout << '\n' << std::endl;
    size_t size_x{ array[0].size() };
    print_array_number_row(size_x);
    print_array_top_row(size_x);

    for (size_t i{0}; i < array.size(); ++i)
    {
        print_array_empty_row(size_x);
        print_array_filled_row(i + 1, array[i]);
        print_array_bottom_row(size_x);
    }
    std::cout << '\n' << std::endl;
}

void Printer::print_array_top_row(size_t size)
{
    std::cout << "    ";
    for (size_t i{0}; i < size; ++i)
        std::cout << "____";
    std::cout << '_' << std::endl;
}

void Printer::print_array_bottom_row(size_t size)
{
    std::cout << "    ";
    for (size_t i{0}; i < size; ++i)
        std::cout << "|___";
    std::cout << '|' << std::endl;
}

void Printer::print_array_empty_row(size_t size)
{
    std::cout << "    ";
    for (size_t i{0}; i < size; ++i)
        std::cout << "|   ";
    std::cout << '|' << std::endl;
}

void Printer::print_array_filled_row(size_t pos, const std::vector<char>& row)
{
    int padlen_left;
    int padlen_right;
    std::string str;

    str = std::to_string(pos);
    padlen_left = (4 + 1 - str.size()) / 2;
    padlen_right = 4 - str.size() - padlen_left;
    printf("%*s%s%*s", padlen_left, "", str.c_str(), padlen_right, "");

    for (auto i_content: row)
        std::cout << "| " << i_content << " ";
    std::cout << '|' << std::endl;
}

void Printer::print_array_number_row(size_t size)
{
    int padlen_left;
    int padlen_right;
    std::string i_str;

    std::cout << R"( x\y)";
    for (size_t i{0}; i < size; ++i)
    {
        i_str = std::to_string(i + 1);
        padlen_left = (3 + 1 - i_str.size()) / 2;
        padlen_right = 3 - i_str.size() - padlen_left;
        printf(" %*s%s%*s", padlen_left, "", i_str.c_str(), padlen_right, "");
    }
    std::cout << ' ' << std::endl;
}

} // End namespace FIAR
