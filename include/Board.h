// -----------------------------------------------------------------------------
// 5iar -- Board.h
// -----------------------------------------------------------------------------
//
// ...
//

#ifndef __FIAR_BOARD_H__
#define __FIAR_BOARD_H__


#include <vector>
#include <iostream>

#include "Defines.h"
#include "Position.h"

namespace FIAR{

using board_t = std::vector<std::vector<char>>;
class Board
{
public:
    // standard constructor
    Board() = default;

    // constructor for square board
    Board(std::size_t sizeX, std::size_t sizeY, char defaultSymbol = g_defaultSymbol);

    // No copy constructor
    Board(const Board& board) = delete;
    // No assignement operator
    Board& operator=(const Board& b) = delete;


    // constructor for square board
    //~ Board( unsigned int const size );

    // destructor
    //~ ~Printer( void );

    // ...
    auto add_symbol(std::size_t posX, std::size_t posY, char symbol) -> bool;
    bool add_symbol(const Position& pos, char symbol);

    // ...
    auto get_array( void ) -> std::vector<std::vector<char>>;// TODO: Probably pretty expensive

    // Get functions
    std::size_t get_size_x() const;
    std::size_t get_size_y() const;
    char get_symbol(std::size_t posX, std::size_t posY, bool* ok = nullptr) const;
    char get_symbol(const Position& pos, bool* ok = nullptr) const;

    // ...
    //~ auto find_winning_sequences( void ) -> std::vector< std::vector< int > >;
    auto find_winning_sequences( void ) const -> std::vector<WinningSequence>;
    bool getWinningSequence(WinningSequence& seq) const;

    // Telling whether the board is valid
    bool isValid() const;
    // Returning the number of slots (the area) of the board
    std::size_t area() const;

    // Overloading the << insertion operator
    friend std::ostream& operator<<(std::ostream& stream, const Board& board);

protected:
    // Building the board, taking care of the dimensions
    void buildBoard();

private:
    // size of board
    std::size_t m_sizeX{ g_boardSizeX };
    std::size_t m_sizeY{ g_boardSizeY };
    // Char to be displayed at each position by default
    char m_defaultSymbol{ g_defaultSymbol };// The global variable should be enough, right?

    // actual board representation
    board_t m_array{ };// Maybe the previous one was better?
    //~ std::vector<char> array;
};

// Printing the board (Marco format)
namespace MarcoPrint{
void marcoPrint(std::ostream& stream, std::size_t sizeY, const board_t& array);
void print_array_top_row(std::ostream& stream, std::size_t size);
void print_array_bottom_row(std::ostream& stream, std::size_t size);
void print_array_empty_row(std::ostream& stream, std::size_t size);
void print_array_filled_row(std::ostream& stream, std::size_t pos, const std::vector<char>& row);
void print_array_number_row(std::ostream& stream, std::size_t size);
}// End namespace MarcoPrint

// Printing the board (Joseph format)
namespace JosephPrint{
void josephPrint(std::ostream& stream, std::size_t sizeY, const board_t& array);
void printHorizontalHead(std::ostream& stream, std::size_t size);
void printHorizontalLine(std::ostream& stream, std::size_t size);
void printHorizontalData(std::ostream& stream, std::size_t pos, const std::vector<char>& row);
std::string getFormatedNumber(int number);
}// End namespace JosephPrint

}// End namespace FIAR

#endif
