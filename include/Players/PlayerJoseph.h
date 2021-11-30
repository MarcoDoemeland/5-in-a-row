#ifndef PLAYERJOSEPH_H
#define PLAYERJOSEPH_H

// Custom classes and defines
#include "Defines.h"
#include "PlayerBase.h"
#include "Board.h"

namespace FIAR{

// Dumm, duemmer, am duemmsten
class PlayerJoseph final : public PlayerBase{
public:
    // Constructor
    PlayerJoseph(Board* board, Piece piece);
    // Destructor
    virtual ~PlayerJoseph();

    // Function to use to make an action
    Position doAction() override;

private:


};

}// ENd namespace FIAR

#endif // PLAYERJOSEPH_H
