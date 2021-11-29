#include "Players/PlayerRandom.h"
#include "Utilities.h"

namespace FIAR{


// Constructor
PlayerRandom::PlayerRandom(Board* board, char symbol)
    : PlayerBase( board, "RandomBot", symbol )
    , m_boardW{ board->get_size_x() }
    , m_boardH{ board->get_size_y() }
{

}

// Destructor
PlayerRandom::~PlayerRandom()
{

}

// Asking the player to make an action
void PlayerRandom::doAction(){

    Position pos;

	// repeat shuffling random positions until non occupied tile has been found
    do
    {
		pos = Position(static_cast<std::size_t>(getRandomInt(1, m_boardW)), static_cast<std::size_t>(getRandomInt(1, m_boardH)));
	}
	while (!m_board->checkTile(pos));


	m_board->add_symbol(pos, m_symbol);
}

}// End namespace FIAR
