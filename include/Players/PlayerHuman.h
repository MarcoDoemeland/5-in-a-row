// -----------------------------------------------------------------------------
// 5iar -- Game.h
// -----------------------------------------------------------------------------
//
// ...
//




#ifndef __FIAR_PLAYERHUMAN_H__
#define __FIAR_PLAYERHUMAN_H__


//~ #include <vector>

#include "PlayerBase.h"


namespace FIAR
{
	class PlayerHuman : public PlayerBase
	{
		public:
			// standard constructor
			PlayerHuman() {};

			// constructor
			//~ PlayerBase( int i );

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
			//~ FIAR::Board board;
			//~ unsigned int size_y;
			//~ char default_symbol;

			// actual board representation
			//~ std::vector<std::vector<char>> array;
			//~ std::vector<char> array;
	};
}


#endif
