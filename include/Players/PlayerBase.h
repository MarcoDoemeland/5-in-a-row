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
    PlayerBase(const Board* board, const std::string& playerName, Piece piece);

    // Destructor
    virtual ~PlayerBase();

    // Asking the player to make an action
    // This function is pure virtual, because doing an action should be specific to any type of player
    virtual Position doAction() = 0;

    // Accessor to the name and count of wins
    std::string name() const;
    int winCount() const;
    // Accessor to the symbol
    //char symbol() const;
    // Accessor to the piece
    Piece piece() const;

    // Incrementing the count of wins
    void incrementWinCount();

protected:
    // Pointer to the playboard
    const Board* m_board{ nullptr };
    // Name of the player
    std::string m_name{ "" };
    // Symbol of the player
    //char m_symbol{ g_defaultSymbol };
    // Piece of the player
    Piece m_piece{ Piece::none };
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

};

// Printing a player
std::ostream& operator<<(std::ostream& stream, const PlayerBase& player);

}// End namespace FIAR

#endif
