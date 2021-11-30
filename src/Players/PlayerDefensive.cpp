
#include <algorithm>
#include <vector>

#include "Defines.h"
#include "Utilities.h"
#include "Players/PlayerDefensive.h"

namespace FIAR{


//----------------------------------------------------------------------
// Constructor
PlayerDefensive::PlayerDefensive(const Board* board, Piece symbol)
    : PlayerBase( board, "DefensiveBot", symbol )
    , m_boardH{ board->getSizeX() }
    , m_boardW{ board->getSizeY() }
{

}

//----------------------------------------------------------------------
// Destructor
PlayerDefensive::~PlayerDefensive()
{

}

//----------------------------------------------------------------------
// Asking the player to make an action
Position PlayerDefensive::doAction(){

    Position pos(1,1);

	// repeat shuffling random positions until non occupied tile has been found
    //~ do
    //~ {
		//~ pos = Position(static_cast<std::size_t>(getRandomInt(1, m_boardW)), static_cast<std::size_t>(getRandomInt(1, m_boardH)));
    //~ }
	//~ while (!m_board->checkTile(pos));


	//~ m_board->add_symbol(pos, m_symbol);


	//~ auto array{ m_board->get_array() };
	//~ auto array{ m_board->get_array() };

	matrix_t<double> policyBoard { computePolicy() };
	//~ matrix_t<double> policyBoard { {1,2,3},{4,5,6} };

	//~ std::cout << policyBoard.size() << '\n';
	//~ std::cout << matrixMax(policyBoard) << '\n';
	//~ std::cout << matrixMaxPos(policyBoard) << '\n';

	for (int i{0}; i < static_cast<int>(m_boardH); ++i)
	{
		for (int j{0}; j < static_cast<int>(m_boardW); ++j)
		{
			//~ char ij_content{ array[i][j] };
			//~ std::cout << i << ' ' << j << " : " << array[i][j] << '\n';
			//~ std::cout << i << ' ' << j << " : " << policyBoard[i][j] << '\n';
			//~ std::cout << typeid(j).name() << '\n';
		}
	}

	pos = matrixMaxPos(policyBoard);

	return pos;

}

//----------------------------------------------------------------------
//~ matrix_t<double> PlayerDefensive::computePolicy(matrix_t<char> boardSymbols)
matrix_t<double> PlayerDefensive::computePolicy()
{
	//~ auto boardPolicy ( (boardSymbols[0].size(), std::vector<double>()) );
	//~ matrix_t<double> boardPolicy [boardSymbols.size()][boardSymbols[0].size()];
	//~ matrix_t<double> boardPolicy [m_boardH][m_boardW];
	matrix_t<int> boardArray (m_boardH, std::vector<int>(m_boardW));
	matrix_t<double> boardPolicy (m_boardH, std::vector<double>(m_boardW));
	//~ std::vector<std::vector<double>> boardPolicy (5, std::vector<double>(10));

	Piece tempTileContent{ };

	for (std::size_t i{0}; i < m_boardH; ++i)
	{
		for (std::size_t j{0}; j < m_boardW; ++j)
		{
			tempTileContent = m_board->getPiece(i+1,j+1);

			if (tempTileContent == m_piece)
			{
				boardArray[i][j] = 1;
			} else if (tempTileContent == Piece::none)
			{
				boardArray[i][j] = 0;
			} else
			{
				boardArray[i][j] = -1;
			}


			//~ if (tempTileContent == Piece::none):
				//~ boardArray[i][j] = 0;

			//~ switch( static_cast<int>(tempTileContent) )
			//~ {
				//~ case( static_cast<int>(Piece::none) ):
					//~ boardArray[i][j] = 0;
					//~ break;
				//~ case( static_cast<int>(m_piece) ):
					//~ boardArray[i][j] = 1;
					//~ break;
				 //~ default:
					//~ boardArray[i][j] = -1;
					//~ break;
			//~ }

			//~ boardArray[i][j] = static_cast<double>(m_board->getPiece(i+1,j+1));
			//~ boardPolicy[i][j] = static_cast<double>(boardSymbols[i][j]);
			//~ boardPolicy[i].push_back(static_cast<double>(boardSymbols[i][j]));

			//~ std::cout << boardArray[i][j] << ' ';
		}

		//~ std::cout << '\n';
	}

	int i_max {static_cast<int>(m_boardH)-1};
	int j_max {static_cast<int>(m_boardW)-1};

	for (int i{0}; i <= i_max ; ++i)
	{
		for (int j{0}; j < j_max; ++j)
		{
			if (boardArray[i][j] != 0) continue;

			if ( i >= 1 && boardArray[i-1][j] == -1 ) boardPolicy[i][j] += 1;
			if ( i <= i_max-1 && boardArray[i+1][j] == -1 ) boardPolicy[i][j] += 1;
			if ( j >= 1 && boardArray[i][j-1] == -1 ) boardPolicy[i][j] += 1;
			if ( j <= j_max-1 && boardArray[i][j+1] == -1 ) boardPolicy[i][j] += 1;
			//~ if ( i > 1 && boardArray[i-2][j] == -1 ) boardPolicy[i][j] += 1;
			//~ if ( i > 1 && boardArray[i-2][j] == -1 ) boardPolicy[i][j] += 1;
		}
	}

	//~ std::cout << "bla" << '\n';

	return boardPolicy;
}





//----------------------------------------------------------------------
template <typename T>
T matrixMax(matrix_t<T> matrix)
{
	T maximum{ matrix[0][0]};

	for (auto& i_row : matrix)
		for (auto& ij_entry : i_row)
			maximum = std::max(ij_entry, maximum);

	//~ for(int x=0; x<width; ++x)
	  //~ for(int y=0; y<height; ++y)
		//~ maximum = std::max(matrix[x][y], maximum);

	return maximum;
}

//----------------------------------------------------------------------
template <typename T>
Position matrixMaxPos(matrix_t<T> matrix)
{
	T maximum{ matrix[0][0]};
	Position maxpos(1,1);


	for (int i{0}; i < static_cast<int>(matrix.size()); ++i)
	{
		for (int j{0}; j < static_cast<int>(matrix[0].size()); ++j)
		{
			if ( matrix[i][j] <= maximum ) continue;

			maximum = matrix[i][j];
			maxpos = Position(i+1, j+1);
		}
	}

	return maxpos;


}


}// End namespace FIAR





