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

// Alias for the actual board (2D array containing pieces)
using board_t = std::vector<std::vector<Piece>>;

class Board
{
public:
    // standard constructor
    Board() = default;

    // constructor for square board
    Board(std::size_t sizeX, std::size_t sizeY);
    // Storing the default char in this class doesn't make sense
    //~ Board(std::size_t sizeX, std::size_t sizeY, char defaultSymbol = g_defaultSymbol);

    // No copy constructor
    Board(const Board& board) = delete;
    // No assignement operator
    Board& operator=(const Board& b) = delete;

    // check whether Position is in or outside the board
    bool tileIsInside(int posX, int posY) const;
    bool tileIsInside(const Position& pos) const;

    // check whether Tile is empty (returns true of empty)
    bool checkTile(int posX, int posY) const;
    bool checkTile(const Position& pos) const;

    // Telling whether or not a cell is containing Piece::none
    // Similar to checkTile but with enhanced error management
    bool isEmptyAt(int posX, int posY, bool* ok = nullptr) const;
    bool isEmptyAt(const Position& pos, bool* ok = nullptr) const;

    // Adding a piece
    bool addPiece(int posX, int posY, Piece piece);
    bool addPiece(const Position& pos, Piece piece);

    // Returning the piece contained at the given position
    Piece getPiece(int posX, int posY, bool* ok = nullptr) const;
    Piece getPiece(const Position& pos, bool* ok = nullptr) const;

    // Dimensions of the board
    std::size_t getSizeX() const;
    std::size_t getSizeY() const;

    // Telling whether the board is valid
    bool isValid() const;
    // Returning the number of slots (the area) of the board
    std::size_t area() const;

    // ...
    //~ auto get_array( void ) -> std::vector<std::vector<char>>;// TODO: Probably pretty expensive

    // ...
    //~ auto find_winning_sequences( void ) -> std::vector< std::vector< int > >;
    auto find_winning_sequences( void ) const -> std::vector<WinningSequence>;
    bool getWinningSequence(WinningSequence& seq) const;

    // Overloading the << insertion operator
    friend std::ostream& operator<<(std::ostream& stream, const Board& board);

protected:
    // Building the board, taking care of the dimensions
    void buildBoard();

private:
    // Dimensions of the board
    std::size_t m_sizeX{ g_boardSizeX };
    std::size_t m_sizeY{ g_boardSizeY };
    // Char to be displayed at each position by default
    //~ char m_defaultSymbol{ g_defaultSymbol };// The global variable should be enough, right?

    // Actual board representation
    board_t m_array{ };
};

// Overloading the << operator to print the pieces
std::ostream& operator<<(std::ostream& stream, Piece piece);

// Printing the board (Marco format)
namespace MarcoPrint{
void marcoPrint(std::ostream& stream, std::size_t sizeY, const board_t& array);
void print_array_top_row(std::ostream& stream, std::size_t size);
void print_array_bottom_row(std::ostream& stream, std::size_t size);
void print_array_empty_row(std::ostream& stream, std::size_t size);
void print_array_filled_row(std::ostream& stream, std::size_t pos, const std::vector<Piece>& row);
void print_array_number_row(std::ostream& stream, std::size_t size);
}// End namespace MarcoPrint

// Printing the board (Joseph format)
namespace JosephPrint{
void josephPrint(std::ostream& stream, std::size_t sizeY, const board_t& array);
void printHorizontalHead(std::ostream& stream, std::size_t size);
void printHorizontalLine(std::ostream& stream, std::size_t size);
void printHorizontalData(std::ostream& stream, std::size_t pos, const std::vector<Piece>& row);
std::string getFormatedNumber(int number);
}// End namespace JosephPrint

}// End namespace FIAR

#endif
