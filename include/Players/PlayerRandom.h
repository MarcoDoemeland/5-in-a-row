// -----------------------------------------------------------------------------
// 5iar -- Game.h
// -----------------------------------------------------------------------------
//
// ...
//

#ifndef __FIAR_PLAYERRANDOM_H__
#define __FIAR_PLAYERRANDOM_H__

// Custom classes and defines
#include "Defines.h"
#include "PlayerBase.h"
#include "Board.h"

namespace FIAR{

class PlayerRandom : public PlayerBase
{
public:
    // Constructor
    PlayerRandom(const Board* board, Piece piece);

    // Destructor
    virtual ~PlayerRandom();

    // Asking the player to make an action
    // This function has to be overriden
    Position doAction() override;

private:
    // Dimensions of the board
    std::size_t m_boardW{ 0u };
    std::size_t m_boardH{ 0u };

};

}// End namespace FIAR

#endif
