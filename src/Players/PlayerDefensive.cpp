
#include <algorithm>
#include <stdio.h>
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

	pos = matrixMaxPosition(policyBoard);

	return pos;

}






//----------------------------------------------------------------------
double PlayerDefensive::evaluateDirection(const Position& currentPosition, const Position& direction)
{
	double retVal{ 1.0 };
	Piece ownTile{ m_piece };
	Piece opponentTile{ };

	if (ownTile == Piece::player1) opponentTile = Piece::player2;
	else opponentTile = Piece::player1;

	for (int factor{1}; factor < 5; ++factor)
	{
		Position positionToBeChecked{ currentPosition + factor*direction };
		if (! (m_board->tileIsInside(positionToBeChecked)) ) break;

		Piece tileToBeChecked{ m_board->getPiece(positionToBeChecked) };
		if ( tileToBeChecked != opponentTile )
		{
			break;
		}

		retVal *= 4.0;
	}

	for (int factor{1}; factor < 5; ++factor)
	{
		Position positionToBeChecked{ currentPosition - factor*direction };
		if (! (m_board->tileIsInside(positionToBeChecked)) ) break;

		Piece tileToBeChecked{ m_board->getPiece(positionToBeChecked) };
		if ( tileToBeChecked != opponentTile ) break;

		retVal *= 4.0;
	}

	retVal -= 1.0;
	return retVal;
}


//----------------------------------------------------------------------
//~ matrix_t<double> PlayerDefensive::computePolicy(matrix_t<char> boardSymbols)
matrix_t<double> PlayerDefensive::computePolicy()
{
	matrix_t<double> boardPolicy (m_boardH, std::vector<double>(m_boardW));

	int i_max {static_cast<int>(m_boardH)-1};
	int j_max {static_cast<int>(m_boardW)-1};

	Piece emptyTile{ Piece::none };
	std::vector<Position> directions{ Position(0,1), Position(1,1), Position(1,0), Position(1,-1) };


	//~ std::cout << '\n';
	for (int i{0}; i <= i_max ; ++i)
	{
		for (int j{0}; j < j_max; ++j)
		{
    		//~ static double s_frac{ 1. / RAND_MAX };
			boardPolicy[i][j] += getRandomDouble(0., 0.5);
			//~ std::cout << getRandomDouble(0., 0.5) << '\n';
			//~ std::cout << rand()*s_frac << '\n';


			Position currentPosition{ Position(i+1,j+1) };
			Piece currentTile{ m_board->getPiece(currentPosition) };

			if (currentTile != emptyTile)
			{
				//~ std::cout << "       ";
				continue;
			}

			for (auto direction : directions)
			{
				boardPolicy[i][j] += evaluateDirection( currentPosition, direction);
			}


			//~ std::cout << boardPolicy[i][j] << ' ';
			//~ printf("%6.1f ", boardPolicy[i][j]);
		}
		//~ std::cout << '\n';
	}


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
Position matrixMaxPosition(matrix_t<T> matrix)
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





