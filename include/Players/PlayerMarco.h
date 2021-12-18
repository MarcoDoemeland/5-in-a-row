// -----------------------------------------------------------------------------
// 5iar -- PlayerMarco.h
// -----------------------------------------------------------------------------
//
// ...
//

#ifndef __FIAR_PLAYERMARCO_H__
#define __FIAR_PLAYERMARCO_H__

// Custom classes and defines
#include "Defines.h"
#include "PlayerBase.h"
#include "Board.h"

namespace FIAR{



template <typename T>
using matrix_t = std::vector<std::vector<T>>;



//======================================================================
class PlayerMarco : public PlayerBase
{
public:
    // Constructor
    PlayerMarco(const Board* board, Piece symbol);

    // Destructor
    virtual ~PlayerMarco();

    // Asking the player to make an action
    // This function has to be overriden
    Position doAction() override;



private:

    // Dimensions of the board
    std::size_t m_boardH{ 0 };
    std::size_t m_boardW{ 0 };

    //~ boardArray_t<char> boardSymbols;
    //~ boardArray_t<double> boardPolicy;



    //~ matrix_t<double> computePolicy(matrix_t<char> boardSymbols);
    double evaluateDirection ( const Position& currentPosition, const Position& direction, const Piece& tileFocused );
	double computePolicyOfPosition ( const Position& currentPosition );
    matrix_t<double> computePolicy ();


};

//======================================================================


template <typename T>
T matrixMax(matrix_t<T> matrix);

template <typename T>
Position matrixMaxPosition(matrix_t<T> matrix);

}// End namespace FIAR

#endif
