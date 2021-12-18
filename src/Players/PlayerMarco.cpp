
#include <algorithm>
#include <stdio.h>
#include <vector>

#include "Defines.h"
#include "Utilities.h"
#include "Players/PlayerMarco.h"

namespace FIAR{


//----------------------------------------------------------------------
// Constructor
PlayerMarco::PlayerMarco(const Board* board, Piece symbol)
    : PlayerBase( board, "MarcoBot", symbol )
    , m_boardH{ board->getSizeX() }
    , m_boardW{ board->getSizeY() }
{

}

//----------------------------------------------------------------------
// Destructor
PlayerMarco::~PlayerMarco()
{

}

//----------------------------------------------------------------------
// Asking the player to make an action
Position PlayerMarco::doAction(){

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

	//~ for (int i{0}; i < static_cast<int>(m_boardH); ++i)
	//~ {
		//~ for (int j{0}; j < static_cast<int>(m_boardW); ++j)
		//~ {
			//~ Position currentPosition{ Position(i+1,j+1) };
			//~ char ij_content{ array[i][j] };
			//~ std::cout << i << ' ' << j << " : " << array[i][j] << '\n';
			//~ std::cout << i << ' ' << j << " : " << policyBoard[i][j] << '\n';
			//~ std::cout << typeid(j).name() << '\n';
		//~ }
	//~ }

	pos = matrixMaxPosition(policyBoard);

	return pos;

}






//----------------------------------------------------------------------
// tileFocused : Piece for which policy will be computed
//				 --> used opponents piece for defensive strategy
//				 --> used own piece for offensice strategy
//~ double PlayerMarco::evaluateDirection(const Position& currentPosition, const Position& direction, const Piece& tileFocused)
//~ {
	//~ double retVal{ 1.0 };

	//~ for (int factor{1}; factor < 5; ++factor)
	//~ {
		//~ Position positionToBeChecked{ currentPosition + factor*direction };
		//~ if (! (m_board->tileIsInside(positionToBeChecked)) ) break;

		//~ Piece tileToBeChecked{ m_board->getPiece(positionToBeChecked) };
		//~ if ( tileToBeChecked != tileFocused )
		//~ {
			//~ break;
		//~ }

		//~ retVal *= 4.0;
	//~ }

	//~ for (int factor{1}; factor < 5; ++factor)
	//~ {
		//~ Position positionToBeChecked{ currentPosition - factor*direction };
		//~ if (! (m_board->tileIsInside(positionToBeChecked)) ) break;

		//~ Piece tileToBeChecked{ m_board->getPiece(positionToBeChecked) };
		//~ if ( tileToBeChecked != tileFocused ) break;

		//~ retVal *= 4.0;
	//~ }

	//~ retVal -= 1.0;
	//~ return retVal;
//~ }
double PlayerMarco::evaluateDirection(const Position& currentPosition, const Position& direction, const Piece& tileFocused)
{
	double retVal{ 1.0 };
	//~ Piece emptyTile{ Piece::none };
	//~ Piece blockedTile{ Piece::none };

	//~ int maxReach {1};
	//~ int numOppInReach {0};
	//~ for (; maxReach < 5; ++maxReach)
	for (int factor{1}; factor < 5; ++factor)
	{
		Position positionToBeChecked{ currentPosition + factor*direction };
		if (! (m_board->tileIsInside(positionToBeChecked)) ) break;

		Piece tileToBeChecked{ m_board->getPiece(positionToBeChecked) };
		if ( tileToBeChecked != tileFocused )
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
		if ( tileToBeChecked != tileFocused ) break;

		retVal *= 4.0;
	}

	retVal -= 1.0;
	return retVal;
}


//----------------------------------------------------------------------
double PlayerMarco::computePolicyOfPosition(const Position& currentPosition)
{
	double retVal{ 0. };
	double tendency{ 0.51 }; // fraction to go offensively (1) or defensively (0)
	//~ double defensiveVal{ 0. };
	//~ double offensiceVal{ 0. };
	Piece ownTile{ m_piece };
	Piece opponentTile{ };


	std::vector<Position> directions{ Position(0,1), Position(1,1), Position(1,0), Position(1,-1) };

	if (ownTile == Piece::player1) opponentTile = Piece::player2;
	else opponentTile = Piece::player1;



	retVal += getRandomDouble(0., 1.e-5);	// add some unpredictability

	for (auto direction : directions)
	{
		// defensice policy
		double defensiveVal { evaluateDirection( currentPosition, direction, opponentTile) };
		//~ retVal += evaluateDirection( currentPosition, direction, opponentTile);

		// offensive policy
		double offensiceVal { evaluateDirection( currentPosition, direction, ownTile) };
		//~ retVal += 1.01*evaluateDirection( currentPosition, direction, ownTile);

		retVal += tendency * offensiceVal + (1.-tendency) * defensiveVal;

	}

	return retVal;
	//~ return std::max(defensiveVal, offensiceVal);
}


//----------------------------------------------------------------------
//~ matrix_t<double> PlayerMarco::computePolicy(matrix_t<char> boardSymbols)
matrix_t<double> PlayerMarco::computePolicy()
{
	matrix_t<double> boardPolicy (m_boardH, std::vector<double>(m_boardW));

	int i_max {static_cast<int>(m_boardH)-1};
	int j_max {static_cast<int>(m_boardW)-1};

	Piece emptyTile{ Piece::none };
	//~ std::vector<Position> directions{ Position(0,1), Position(1,1), Position(1,0), Position(1,-1) };


	//~ std::cout << '\n';
	for (int i{0}; i <= i_max ; ++i)
	{
		for (int j{0}; j <= j_max; ++j)
		{
    		//~ static double s_frac{ 1. / RAND_MAX };
			//~ boardPolicy[i][j] += getRandomDouble(0., 0.1);
			//~ std::cout << getRandomDouble(0., 0.5) << '\n';
			//~ std::cout << rand()*s_frac << '\n';


			Position currentPosition{ Position(i+1,j+1) };
			Piece currentTile{ m_board->getPiece(currentPosition) };

			//~ std::cout << static_cast<int>(currentTile) << ' ';
			//~ std::cout << currentTile << ' ';

			if (currentTile != emptyTile)
			{
				//~ std::cout << "       ";
				continue;
			}

			boardPolicy[i][j] += computePolicyOfPosition( currentPosition );
			//~ for (auto direction : directions)
			//~ {
				//~ boardPolicy[i][j] += evaluateDirection( currentPosition, direction);
			//~ }


			//~ std::cout << currentTile << ' ';
			//~ printf("%s ", currentTile);
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





