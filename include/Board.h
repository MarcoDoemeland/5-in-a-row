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

namespace FIAR
{
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

        // ...
        auto get_array( void ) -> std::vector<std::vector<char>>;// TODO: Probably pretty expensive

        // ...
        //~ auto find_winning_sequences( void ) -> std::vector< std::vector< int > >;
        auto find_winning_sequences( void ) -> std::vector<WinningSequence>;

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
        std::vector<std::vector<char>> m_array{ };// Maybe the previous one was better?
        //~ std::vector<char> array;
	};
}// End namespace FIAR

#endif
