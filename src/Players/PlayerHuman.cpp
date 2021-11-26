#include "PlayerHuman.h"

#include "Utilities.h"
#include "Position.h"

namespace FIAR{

	//~ PlayerBase::PlayerBase( int i )
	//~ {
		//~ std::cout << i << '\n';
	//~ }

// Constructor
PlayerHuman::PlayerHuman(Board* board, const std::string& playerName, char playerSymbol)
    : PlayerBase( board, playerName, playerSymbol ){

}

// Destructor
PlayerHuman::~PlayerHuman(){

}

// Asking the player to make an action
// This function has to be overriden
void PlayerHuman::doAction(){
    // This can be called by PlayerHuman to get an object "Position" from the terminal
    Position pos{ getInputFromUser<Position>("\nEnter a position: ") };
    while(!m_board->add_symbol(pos, m_symbol)){
        pos = getInputFromUser<Position>("\nThis slot is already occupied, please enter another position: ");
    }
}

}// End namespace FIAR
