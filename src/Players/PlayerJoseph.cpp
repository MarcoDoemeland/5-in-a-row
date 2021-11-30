#include "Players/PlayerJoseph.h"

namespace FIAR{

// Constructor
PlayerJoseph::PlayerJoseph(Board* board, Piece piece)
    : PlayerBase( board, "JosephBot", piece ){

}

// Destructor
PlayerJoseph::~PlayerJoseph(){

}

// Function to use to make an action
Position PlayerJoseph::doAction(){

    return Position{};
}

}// ENd namespace FIAR
