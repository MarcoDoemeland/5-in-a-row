// -----------------------------------------------------------------------------
// 5iar -- Board.h
// -----------------------------------------------------------------------------
//
// ...
//




#ifndef __FIAR_BOARD_H__
#define __FIAR_BOARD_H__


#include <vector>
#include <iostream>


namespace FIAR
{
	class Board
	{
		public:
			// standard constructor
			Board();

			// constructor for square board
			Board( unsigned int _size_x,
				   unsigned int _size_y,
				   char _default_symbol
				 );

			// constructor for square board
			//~ Board( unsigned int const size );

			// destructor
			//~ ~Printer( void );

			// ...
			auto add_symbol(	int pos_x,
								int pos_y,
								char symbol
							) -> bool;

			// ...
			auto get_array( void ) -> std::vector<std::vector<char>>;

			// ...
			auto find_winning_sequences( void ) -> std::vector< std::vector< int > >;

		private:

			// size of board
			unsigned int size_x;
			unsigned int size_y;
			char default_symbol;

			// actual board representation
			std::vector<std::vector<char>> array;
			//~ std::vector<char> array;
	};
}



#endif
