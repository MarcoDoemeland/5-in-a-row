
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
// convolveTimeSeries: (for Series of Length 9 and window size 5)
//			x_xxxx_xx_x_x__xx_xxx__x__xxx__x_xx___x__xxx
//			--> find position with most x's in a single of window of fixed size
//~ double PlayerMarco::evaluateDirection(const Position& currentPosition, const Position& direction, const Piece& tileFocused)

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
int PlayerMarco::evaluateDirection(const Position& currentPosition, const Position& direction, const Piece& tileFocused)
{
	int retVal{ 0 };
	Piece emptyTile{ Piece::none };


	std::vector<Piece> directionSeries{};

	for (int i{-4}; i<5; ++i) {
		Position i_Position{ currentPosition + i*direction };
		directionSeries.push_back( m_board->getPiece(i_Position) );
	}


	//~ std::cout << '\n';
	//~ std::cout << currentPosition << '\n';
	//~ std::cout << direction << '\n';
	//~ for (auto i: directionSeries){
		//~ std::cout << i << ' ';
	//~ }

	//~ std::cout << '\n';
	//~ std::cout << convolveTimeSeries(directionSeries, tileFocused, {emptyTile})["count"] << '\n';

	std::map<std::string, int> timeSeriesEval { convolveTimeSeries(directionSeries, tileFocused, {emptyTile}) };

	int counts{ timeSeriesEval["count"] };
	int endpoints{ timeSeriesEval["endpoints"] };

	//~ retVal = 10*counts;

	if (counts < 4) retVal = std::max(10*counts - 5*endpoints, 0);
	else retVal = 1000;




	//~ for (int factor{1}; factor < 5; ++factor)
	//~ {
		//~ Position positionToBeChecked{ currentPosition + factor*direction };
		//~ if (! (m_board->tileIsInside(positionToBeChecked)) ) break;

		//~ Piece tileToBeChecked{ m_board->getPiece(positionToBeChecked) };
		//~ if ( tileToBeChecked != tileFocused ) break;

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
	return retVal;
}


//----------------------------------------------------------------------
double PlayerMarco::computePolicyOfPosition(const Position& currentPosition)
{
	double retVal{ 0. };
	double tendency{ 0.4 }; // fraction to go offensively (1) or defensively (0)
	Piece ownTile{ m_piece };
	Piece opponentTile{ };


	std::vector<Position> directions{ Position(0,1), Position(1,1), Position(1,0), Position(1,-1) };

	if (ownTile == Piece::player1) opponentTile = Piece::player2;
	else opponentTile = Piece::player1;



	retVal += getRandomDouble(0., 0.5);	// add some unpredictability

	// offensive value
	std::vector<int> offVals{};
	for (auto direction : directions) {
		offVals.push_back( evaluateDirection( currentPosition, direction, ownTile) );
	}
	std::sort(offVals.begin(), offVals.end());

	int offensiveVal{ offVals[3] + offVals[2] };
	//~ offensiveVal = *max_element(offVals.begin(), offVals.end());

	// offensive value
	std::vector<int> defVals{};
	for (auto direction : directions) {
		defVals.push_back( evaluateDirection( currentPosition, direction, opponentTile) );
	}
	std::sort(defVals.begin(), defVals.end());

	int defensiveVal{ defVals[3] + defVals[2] };
	//~ defensiveVal = *max_element(defVals.begin(), defVals.end());


	//~ if ( (offVals[3] == 40 && ) || () )

	retVal += tendency * offensiveVal*offensiveVal + (1.-tendency) * defensiveVal*defensiveVal;

	//~ for (auto direction : directions)
	//~ {
		//~ // defensice policy
		//~ int defensiveVal { evaluateDirection( currentPosition, direction, opponentTile) };
		//~ // retVal += evaluateDirection( currentPosition, direction, opponentTile);

		//~ // offensive policy
		//~ int offensiveVal { evaluateDirection( currentPosition, direction, ownTile) };
		//~ // retVal += 1.01*evaluateDirection( currentPosition, direction, ownTile);

		//~ retVal += tendency * offensiveVal + (1.-tendency) * defensiveVal;

	//~ }

	return retVal;
	//~ return std::max(defensiveVal, offensiveVal);
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
	for (int i{0}; i <= i_max ; ++i) {
		for (int j{0}; j <= j_max; ++j) {
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
			//~ printf("%6.2f ", boardPolicy[i][j]);
		}
		//~ std::cout << '\n';
	}



	//~ std::cout << '\n';
	//~ for (int i{0}; i <= i_max ; ++i) {
		//~ for (int j{0}; j <= j_max; ++j) {
			//~ printf("%6.2f ", boardPolicy[i][j]);
		//~ }
		//~ std::cout << '\n';
	//~ }

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


//----------------------------------------------------------------------
// comapct way to check whether item appears inside a list and return true if so
template <typename T>
bool itemInList (const T item, const std::vector<T>& list)
{
	return (std::find(list.begin(), list.end(), item) != list.end());
}

//----------------------------------------------------------------------
// evaluate a time series (of types T) for max appearances of a T type item in a window of given length
// return map with keys:
//		"count"     : maximum of counts over all windows
//		"pos"       : position/start of max window
//		"endpoints" : number of points left and right of window are blocked
//~ template <typename T>
//~ std::map<std::string, int> convolveTimeSeries (
										//~ const std::vector<T>& series,
										//~ const T               countSymbol,
										//~ const std::vector<T>& neutralSymbols,
										//~ const int             windowSize
									//~ )
{
	std::map<std::string, int> resultsDict { {"pos", 0}, {"count", 0} , {"endpoints", 2} };

	int seriesLength { static_cast<int>(series.size()) };
	std::vector<double> valArray (seriesLength );

	for (int i {0}; i < seriesLength; ++i) {
		if (series[i] != countSymbol) continue;

		for (int j {std::max(i-windowSize+1,0)}; j <= i; ++j) {
			++valArray[j];
		}
	}

	for (int i {0}; i < seriesLength; ++i) {
		if (itemInList(series[i], neutralSymbols) || series[i] == countSymbol) continue;

		for (int j {std::max(i-windowSize+1,0)}; j <= i; ++j) {
			valArray[j] = 0.;
		}

		if (i >= windowSize) valArray[i-windowSize] -= 0.1;
		if (i < seriesLength-1) valArray[i+1] -= 0.1;
	}

	valArray[0] -= 0.1;
	valArray[seriesLength-windowSize-1] -= 0.1;

	auto maxIt = std::max_element(valArray.begin(), valArray.end());

	int maxPos { static_cast<int>(std::distance(valArray.begin(), maxIt)) };
	int maxVal { static_cast<int>(valArray[maxPos]+0.5) };
	int maxEP { static_cast<int>(10*(maxVal-valArray[maxPos])+0.5) };

	resultsDict["pos"] = maxPos;
	resultsDict["count"] = maxVal;
	resultsDict["endpoints"] = maxEP;

	return resultsDict;
}
//~ {
	//~ std::map<std::string, int> resultsDict { {"pos", 0}, {"count", 0} , {"endpoints", 2} };

	//~ int maxIndex{ std::max(static_cast<int>(series.size()) - windowSize, 0) };

	//~ int currentBestPos {0};
	//~ int currentBestCount {0};
	//~ int currentBestEndpoints {2};

	//~ for (int i {0}; i <= maxIndex ; ++i){
		//~ int i_count{ 0 };
		//~ int i_numEndpoints{ 0 };

		//~ const std::vector<T>& i_window{ std::vector<T>(series.begin() + i, series.begin() + i + windowSize) };

		//~ for (auto j : i_window) {
			//~ if (itemInList(j, neutralSymbols)) {
				//~ continue;
			//~ }
			//~ else if (j == countSymbol) {
				//~ ++i_count;
			//~ }
			//~ else {
				//~ i_count = 0;
				//~ break;
			//~ }
		//~ }


		//~ // check left edge of window
		//~ if ( i == 0 ) ++i_numEndpoints;
		//~ else if ( series[i-1] != countSymbol && !itemInList(series[i-1], neutralSymbols) ) ++i_numEndpoints;

		//~ // check right edge of window
		//~ if ( i == maxIndex ) ++i_numEndpoints;
		//~ else if ( series[i+windowSize] != countSymbol && !itemInList(series[i+windowSize], neutralSymbols) ) ++i_numEndpoints;

		//~ if (i_count > currentBestCount || (i_count == currentBestCount && i_numEndpoints < currentBestEndpoints) ) {
			//~ currentBestPos = i;
			//~ currentBestCount = i_count;
			//~ currentBestEndpoints = i_numEndpoints;
		//~ }


	//~ }

	//~ resultsDict["pos"] = currentBestPos;
	//~ resultsDict["count"] = currentBestCount;
	//~ resultsDict["endpoints"] = currentBestEndpoints;

	//~ return resultsDict;
//~ }












}// End namespace FIAR





