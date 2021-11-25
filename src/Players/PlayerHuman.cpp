#include "PlayerHuman.h"

namespace FIAR{

	//~ PlayerBase::PlayerBase( int i )
	//~ {
		//~ std::cout << i << '\n';
	//~ }

// Constructor
PlayerHuman::PlayerHuman(Board* board, const std::string& playerName)
    : PlayerBase( board, playerName ){

}

// Destructor
PlayerHuman::~PlayerHuman(){

}

// Asking the player to make an action
// This function has to be overriden
void PlayerHuman::doAction(){

}

}// End namespace FIAR
