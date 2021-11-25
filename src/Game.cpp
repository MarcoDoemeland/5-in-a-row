//~ #include <iostream>

#include "Game.h"
//~ #include "Board.h"
//~ #include "Printer.h"
//~ #include "Players/PlayerBase.h"

namespace FIAR
{
	//~ Game::Game( FIAR::Board _board )
		//~ : board (_board)
	//~ {
		//~ board = _board;
		//~ std::cout << BOARD_SIZE_X << '\n';
	//~ }
Game::Game(Board* board, PlayerBase* player1, PlayerBase* player2, Printer* printer)
    : m_board{ board }
    , m_player1{ player1 }
    , m_player2{ player2 }
    , m_printer{ printer }{
    //~ std::cout << "huhuh" << '\n';

}

}// End namespace FIAR

