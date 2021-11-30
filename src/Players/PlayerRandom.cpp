#include "Players/PlayerRandom.h"
#include "Utilities.h"

namespace FIAR{

// Constructor
PlayerRandom::PlayerRandom(const Board* board, Piece piece)
    : PlayerBase( board, "RandomBot", piece )
    , m_boardW{ board->getSizeX() }
    , m_boardH{ board->getSizeY() }{

}

// Destructor
PlayerRandom::~PlayerRandom(){

}

// Asking the player to make an action
Position PlayerRandom::doAction(){
    // Position to be returned
    Position pos;

	// repeat shuffling random positions until non occupied tile has been found
    do{
        pos = Position(static_cast<std::size_t>(getRandomInt(1u, m_boardW)), static_cast<std::size_t>(getRandomInt(1u, m_boardH)));
    }while(!m_board->checkTile(pos));

    // Returning the position
    return pos;
}

}// End namespace FIAR
