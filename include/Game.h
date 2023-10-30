// -----------------------------------------------------------------------------
// 5iar -- Game.h
// -----------------------------------------------------------------------------
//
// ...
//

#ifndef __FIAR_GAME_H__
#define __FIAR_GAME_H__

#include <vector>

#include "Defines.h"
#include "Board.h"
#include "Players/PlayerBase.h"
#include "Printer.h"

namespace FIAR
{

class Game
{
public:
    // Standard constructor
    Game();
    // Destructor
    ~Game();

    // Starting a gaming session
    void start();

    // A winner was found, returning a pointer
    PlayerBase* winnerFound() const;

protected:
    // Defining the players
    void definePlayer1();
    void definePlayer2();
    void definePlayer(const std::string& text, PlayerBase*& player, Piece piece);

    // Telling which player should start the game. To be called only at start.
    void defineCurrentPlayer();
    // Swapping the current player. To be called in the end of each round.
    void swapCurrentPlayer();

private:
    // Test members for the alternative strategy
    // Board where the game is going to be played
    Board m_board;
    // Using pointers for the players, so we avoid slicing
    PlayerBase* m_player1{ nullptr };
    PlayerBase* m_player2{ nullptr };
    // Player currently playing and his piece code
    PlayerBase* m_currentPlayer{ nullptr };
};

}// End namespace FIAR

#endif
