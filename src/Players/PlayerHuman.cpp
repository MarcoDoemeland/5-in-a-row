#include "Players/PlayerHuman.h"

#include "Utilities.h"
#include "Position.h"

namespace FIAR{

// Constructor
PlayerHuman::PlayerHuman(const Board* board, const std::string& playerName, Piece piece)
    : PlayerBase( board, playerName, piece ){

}

// Destructor
PlayerHuman::~PlayerHuman(){

}

// Asking the player to make an action
// This function has to be overriden
Position PlayerHuman::doAction(){
    // This can be called by PlayerHuman to get an object "Position" from the terminal
    Position pos{ getInputFromUser<Position>("\nEnter a position: ") };
    bool posValid;
    while(!m_board->isEmptyAt(pos, &posValid)){
        if(posValid) pos = getInputFromUser<Position>("\nThis cell is already occupied, please enter another position: ");
        else pos = getInputFromUser<Position>("\nThis position is invalid, please enter another position: ");
    }
    // Returning the position
    return pos;
}

}// End namespace FIAR
