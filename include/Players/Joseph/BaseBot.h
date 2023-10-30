#ifndef __FIAR_PLAYERS_JOSEPH_BASEBOT_H_
#define __FIAR_PLAYERS_JOSEPH_BASEBOT_H_

#include <Board.h>
#include <Defines.h>
#include <Players/PlayerBase.h>
#include <Players/Joseph/Utils.h>

namespace FIAR
{

namespace joseph
{

// Dumm, duemmer, am duemmsten
class BaseBot : public PlayerBase, public Logger
{
public:
    // Constructor
    BaseBot(const Board* board, Piece piece, const std::string& name);
    // Destructor
    virtual ~BaseBot();

private:
    // Function to use to make an action
    Position doAction() final;

protected:
    // Playing an algorithm
    virtual void playAlgo(Position& /*pos*/) { }

    // Getting x and y out of an index
    inline size_t xFromIndex(size_t index) { return index % m_boardW + 1; }
    inline size_t yFromIndex(size_t index) { return index / m_boardW + 1; }
    // Getting an index out of x and y
    inline size_t indexFromPos(size_t x, size_t y) { return x - 1 + (y - 1) * m_boardW; }

    // Telling whether a piece is adversary
    inline bool pieceIsAdversary(Piece piece) const
    { return pieceIsHostile(piece) && !pieceIsNone(piece); }
    // Telling whether a piece is adversary or none
    inline bool pieceIsHostile(Piece piece) const
    { return piece != m_piece; }
    // Telling whether a there is a piece
    inline bool pieceIsNone(Piece piece) const
    { return piece == Piece::none; }
    // Telling whether the piece is mine
    inline bool pieceIsMine(Piece piece) const
    { return piece == m_piece; }

    // Increment functions
    int incrementSame(int /*inc*/) const { return 0; }
    int incrementPlus(int inc) const { return inc; }
    int incrementMinus(int inc) const{ return -inc; }

    // Setting the increment / decrement functions according to the direction
    void set2DIncrementHor();
    void set2DIncrementVer();
    void set2DIncrementDia();
    void set2DIncrementAdia();

    // Comparison function
    bool (BaseBot::*m_inversFunc)(Piece) const{ nullptr };
    bool (BaseBot::*m_targetFunc)(Piece) const{ nullptr };
    // LINEAR SEQUENCE SEARCH
    // Increment functions
    int (BaseBot::*m_xIncFunc)(int) const{ nullptr };
    int (BaseBot::*m_yIncFunc)(int) const{ nullptr };
    // Decrement functions
    int (BaseBot::*m_xDecFunc)(int) const{ nullptr };
    int (BaseBot::*m_yDecFunc)(int) const{ nullptr };
    // 2D SEQUENCE SEARCH
    // Increment functions
    int (BaseBot::*m_x2DIncFunc)(int) const{ nullptr };
    int (BaseBot::*m_y2DIncFunc)(int) const{ nullptr };
    // Decrement functions
    int (BaseBot::*m_x2DDecFunc)(int) const{ nullptr };
    int (BaseBot::*m_y2DDecFunc)(int) const{ nullptr };

    // Rounds counter
    int m_rndCnt{ 0 };
    // Random counter
    int m_ranCnt{ 0 };
    // Error counter
    int m_errCnt{ 0 };

    // Dimensions of the board
    const size_t m_boardW;
    const size_t m_boardH;
    const size_t m_fields;

    // Char used to read (more performant)
    Piece m_in;
};

} // End namespace joseph

} // End namespace FIAR

#endif // __FIAR_PLAYERS_JOSEPH_BASEBOT_H_
