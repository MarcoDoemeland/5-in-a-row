// -----------------------------------------------------------------------------
// 5iar -- PlayerDefensive.h
// -----------------------------------------------------------------------------
//
// ...
//

#ifndef __FIAR_PLAYERDEFENSIVE_H__
#define __FIAR_PLAYERDEFENSIVE_H__

// Custom classes and defines
#include "Defines.h"
#include "PlayerBase.h"
#include "Board.h"

namespace FIAR{



template <typename T>
using matrix_t = std::vector<std::vector<T>>;



//======================================================================
class PlayerDefensive : public PlayerBase
{
public:
    // Constructor
    PlayerDefensive(const Board* board, Piece symbol);

    // Destructor
    virtual ~PlayerDefensive();

    // Asking the player to make an action
    // This function has to be overriden
    Position doAction() override;

    double evaluateDirection(const Position& currentPosition, const Position& direction);


private:

    // Dimensions of the board
    std::size_t m_boardH{ 0 };
    std::size_t m_boardW{ 0 };

    //~ boardArray_t<char> boardSymbols;
    //~ boardArray_t<double> boardPolicy;



    //~ matrix_t<double> computePolicy(matrix_t<char> boardSymbols);
    matrix_t<double> computePolicy();


};

//======================================================================


template <typename T>
T matrixMax(matrix_t<T> matrix);

template <typename T>
Position matrixMaxPosition(matrix_t<T> matrix);

}// End namespace FIAR

#endif
