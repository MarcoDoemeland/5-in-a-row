// -----------------------------------------------------------------------------
// 5iar -- Game.h
// -----------------------------------------------------------------------------
//
// ...
//

#ifndef __FIAR_PLAYERBASE_H__
#define __FIAR_PLAYERBASE_H__

// Standard library headers
#include <string>
#include <iostream>
//#include <vector>

// Custom classes and defines
#include "Defines.h"
#include "Board.h"

// uhSchauMichAnIchBinDerMeisterDesCamelCase

namespace FIAR{

// Abstract class, should be inherited
class PlayerBase
{
public:
    // Constructor
    PlayerBase(Board* board, const std::string& playerName, char playerSymbol);

    // constructor - to be removed? TODO
    //~ PlayerBase( int i = 0 );

    // Destructor
    virtual ~PlayerBase();

    // Asking the player to make an action
    // This function is pure virtual, because doing an action should be specific to any type of player
    virtual void doAction() = 0;

    // Accessor to the name and count of wins
    std::string name() const;
    int winCount() const;

    // Accessor to symbol
    char symbol() const;

    // Incrementing the count of wins
    void incrementWinCount();

protected:
    // Pointer to the playboard
    Board* m_board{ nullptr };
    // Name of the player
    std::string m_name{ "" };
    // Symbol to be used when playing
    char m_symbol{ '\0' };
    // Count of wins
    int m_winCount{ 0 };

private:
    // Blocking the standard constructor.
    // A base player will always need a board.
    PlayerBase() = delete;

    // Blocking copy constructor and copy assignement.
    // Who the hell should be allowed to copy a base player after all?
    PlayerBase(const PlayerBase& p) = delete;
    PlayerBase& operator=(const PlayerBase& p) = delete;

        // constructor for square board
        //~ Board( unsigned int const size );

        // destructor
        //~ ~Printer( void );

        // ...
        //~ auto add_symbol(	int const pos_x,
                            //~ int const pos_y,
                            //~ char const symbol
                        //~ ) -> bool;

        // ...
        //~ auto get_array( void ) -> std::vector<std::vector<char>>;

        // ...
        //~ auto find_winning_sequences( void ) -> std::vector< std::vector< int > >;

    private:

        // size of board
        //~ FIAR::Board board;
        //~ unsigned int size_y;
        //~ char default_symbol;

        // actual board representation
        //~ std::vector<std::vector<char>> array;
        //~ std::vector<char> array;
};

// Printing a player
std::ostream& operator<<(std::ostream& stream, const PlayerBase& player);

}// End namespace FIAR

#endif
