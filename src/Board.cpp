#include "Board.h"

// HALLO, ICH BIN EIN KOMMENTAR!!

namespace FIAR
{
	Board::Board(	unsigned int _size_x,
					unsigned int _size_y,
					char _default_symbol	)
		: size_x (_size_x)
		, size_y (_size_y)
		, default_symbol (_default_symbol)
	{
		//~ array = std::vector<char>(size_y, 'a');
		array = std::vector<std::vector<char>>(size_x, std::vector<char>(size_y, default_symbol));
	}


	auto Board::add_symbol(	int pos_x,
							int pos_y,
							char symbol
						) -> bool
	{
		if (pos_x < 1 || pos_x > (int) size_x) return false;
		if (pos_y < 1 || pos_y > (int) size_y) return false;
		if (array[pos_x-1][pos_y-1] != default_symbol) return false;

		array[pos_x-1][pos_y-1] = symbol;
		return true;
	}


	auto Board::get_array( void ) -> std::vector<std::vector<char>>
	{
		return array;
	}


	auto Board::find_winning_sequences( void ) -> std::vector< std::vector< int > >
	{
		std::vector< std::vector< int > > found_sequences {};
		std::vector< int > found_solution {};

		// test for horizontal sequences
		for (int i {0}; i < (int)size_x; ++i)
		{
			for (int j {0}; j < (int)size_y-4; ++j)
			{
				if (	   array[i][j] != default_symbol
						&& array[i][j] == array[i][j+1]
						&& array[i][j] == array[i][j+2]
						&& array[i][j] == array[i][j+3]
						&& array[i][j] == array[i][j+4]
					)
				{
					found_sequences.push_back( {i+1,j+1,0} );
				}
			}
		}

		// test for diagonal sequences
		for (int i {0}; i < (int)size_x-4; ++i)
		{
			for (int j {0}; j < (int)size_y-4; ++j)
			{
				if (	   array[i][j] != default_symbol
						&& array[i][j] == array[i+1][j+1]
						&& array[i][j] == array[i+2][j+2]
						&& array[i][j] == array[i+3][j+3]
						&& array[i][j] == array[i+4][j+4]
					)
				{
					found_sequences.push_back( {i+1,j+1,1} );
				}
			}
		}

		// test for vertical sequences
		for (int i {0}; i < (int)size_x-4; ++i)
		{
			for (int j {0}; j < (int)size_y; ++j)
			{
				if (	   array[i][j] != default_symbol
						&& array[i][j] == array[i+1][j]
						&& array[i][j] == array[i+2][j]
						&& array[i][j] == array[i+3][j]
						&& array[i][j] == array[i+4][j]
					)
				{
					found_sequences.push_back( {i+1,j+1,2} );
				}
			}
		}

		// test for anti diagonal sequences
		for (int i {0}; i < (int)size_x-4; ++i)
		{
			for (int j {4}; j < (int)size_y; ++j)
			{
				if (	   array[i][j] != default_symbol
						&& array[i][j] == array[i+1][j-1]
						&& array[i][j] == array[i+2][j-2]
						&& array[i][j] == array[i+3][j-3]
						&& array[i][j] == array[i+4][j-4]
					)
				{
					found_sequences.push_back( {i+1,j+1,3} );
				}
			}
		}

		return found_sequences;
	}


}


