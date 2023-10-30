#include <Players/Joseph/BaseBot.h>

#include <algorithm>

#include <Utilities.h>

namespace FIAR
{

namespace joseph
{

// Constructor
BaseBot::BaseBot(const Board* board, Piece piece, const std::string& name)
    : PlayerBase( board, name, piece )
    , Logger( "JosephBotLog.txt" )
    , m_boardW{ board->getSizeX() }
    , m_boardH{ board->getSizeY() }
    , m_fields{ m_boardW * m_boardH }
{
}

// Destructor
BaseBot::~BaseBot()
{
}

// Function to use to make an action
Position BaseBot::doAction()
{
    static TimeEvaluation waitingTime;
    log("Waiting time:", waitingTime);

    // Position where the stone has to be placed
    Position pos;
    ++m_rndCnt;
    log("NewRound:", m_rndCnt);

    // Playing an algorithm
    playAlgo(pos);
    log("Evaluation:", pos);

    // Logging the position and possible errors
    log("Playing", pos);
    if(!m_board->isEmptyAt(pos)){
        log("Fatal error!!! Position already occupied");
        ++m_errCnt;
    }
    // End of round, jumpline to make the log clearer
    log("");

    // Saving the time to evaluate the waiting time
    waitingTime = TimeEvaluation();

    // Returning the position
    return pos;
}

// Setting the increment / decrement functions according to the direction
void BaseBot::set2DIncrementHor(){
    m_x2DIncFunc = &BaseBot::incrementPlus;
    m_y2DIncFunc = &BaseBot::incrementSame;
    m_x2DDecFunc = &BaseBot::incrementMinus;
    m_y2DDecFunc = &BaseBot::incrementSame;
}
void BaseBot::set2DIncrementVer(){
    m_x2DIncFunc = &BaseBot::incrementSame;
    m_y2DIncFunc = &BaseBot::incrementPlus;
    m_x2DDecFunc = &BaseBot::incrementSame;
    m_y2DDecFunc = &BaseBot::incrementMinus;
}
void BaseBot::set2DIncrementDia(){
    m_x2DIncFunc = &BaseBot::incrementPlus;
    m_y2DIncFunc = &BaseBot::incrementPlus;
    m_x2DDecFunc = &BaseBot::incrementMinus;
    m_y2DDecFunc = &BaseBot::incrementMinus;
}
void BaseBot::set2DIncrementAdia(){
    m_x2DIncFunc = &BaseBot::incrementPlus;
    m_y2DIncFunc = &BaseBot::incrementMinus;
    m_x2DDecFunc = &BaseBot::incrementMinus;
    m_y2DDecFunc = &BaseBot::incrementPlus;
}

} // End namespace joseph

} // End namespace FIAR
