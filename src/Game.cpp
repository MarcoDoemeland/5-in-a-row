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

	Game::Game( FIAR::Board board_,
		        FIAR::PlayerBase player_1_,
		        FIAR::PlayerBase player_2_,
		        FIAR::Printer printer_
		      )
		: board (board_),
		  player_1 (player_1_),
		  player_2 (player_2_),
		  printer (printer_)
	{
		std::cout << "huhuh" << '\n';
	}
}

