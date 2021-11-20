// -----------------------------------------------------------------------------
// 5iar -- Game.h
// -----------------------------------------------------------------------------
//
// ...
//




#ifndef __FIAR_GAME_H__
#define __FIAR_GAME_H__


#include <vector>

#include "Board.h"
#include "Printer.h"
#include "Players/PlayerBase.h"


namespace FIAR
{



	class Game
	{
		public:
			// standard constructor
			Game();

			// constructor
			Game( FIAR::Board board_,
			      FIAR::PlayerBase player_1_,
			      FIAR::PlayerBase player_2_,
			      FIAR::Printer printer_
			    );

			// constructor for square board
			//~ Board( unsigned int const size );

			// destructor
			//~ ~Printer( void );

			// ...
			//~ auto add_symbol(	int const pos_x,
								//~ int const pos_y,
								//~ char const symbol
							//~ ) -> bool;

			// ...
			//~ auto get_array( void ) -> std::vector<std::vector<char>>;

			// ...
			//~ auto find_winning_sequences( void ) -> std::vector< std::vector< int > >;

		private:

			// size of board
			FIAR::Board board;
			FIAR::PlayerBase player_1;
			FIAR::PlayerBase player_2;
			FIAR::Printer printer;
			//~ unsigned int size_y;
			//~ char default_symbol;

			// actual board representation
			//~ std::vector<std::vector<char>> array;
			//~ std::vector<char> array;
	};
}



#endif
