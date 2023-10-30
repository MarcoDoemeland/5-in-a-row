#include <Players/Joseph/BotType1.h>

#include "Utilities.h"
#include <algorithm>

namespace FIAR
{

namespace joseph
{

BotType1::BotType1(const Board* board, Piece piece)
    : BaseBot( board, piece, "JosephBotOG" )
{
}

BotType1::~BotType1()
{
}

void BotType1::playAlgo(Position& pos)
{
    TimeEvaluation evalTime;

    // Looking for a win sequence of the 1st order
    if(lookForWinSequence1(pos)) return;
    // Looking for a deadly sequence of the 1st order
    else if(lookForDeadlySequence1(pos)) return;

    // Looking for a win sequence of the 2nd order
    else if(lookForWinSequence2(pos)) return;
    // Looking for a deadly sequence of the 2nd order
    else if(lookForDeadlySequence2(pos)) return;

    // Looking for a win sequence of the 3rd order
    else if(lookForWinSequence3(pos)) return;
    // Looking for a deadly sequence of the 3rd order
    else if(lookForDeadlySequence3(pos)) return;

    // Looking for a spot where a win sequence of the 3rd order can be placed
    else if(lookForWinSequence4(pos)) return;
    // Looking for a deadly sequence of the 4th order
    else if(lookForDeadlySequence4(pos)) return;

    // Looking for a spot where a single stone is placed and giving him a friend
    else if(lookForBuildPair(pos)) return;

    // Doing random shit
    else{
        do{
            pos = Position(static_cast<std::size_t>(getRandomInt(1, m_boardW)), static_cast<std::size_t>(getRandomInt(1, m_boardH)));
        }while(!m_board->isEmptyAt(pos));
        ++m_ranCnt;
    }

    log("Board evaluation time:", evalTime);
}

// Looking for a win sequence of the 1st order (****_, ***_*, **_**, *_***, _****)
bool BotType1::lookForWinSequence1(Position& pos)
{
    // Size of the sequence
    m_seqSize = 5;
    // Max count of blank section expected
    m_maxCount = 1;
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &BotType1::pieceIsAdversary;
    // Sequence searching function, sequence with 1 vacancy
    m_trackFunc = &BotType1::validSequenceFound;
    // Looking for a linear sequence
    log("WinSeq1 tracking starts...");
    return lookForLinearSequence(pos);
}
// Looking for a deadly sequence of the 1st order (****_, ***_*, **_**, *_***, _****)
bool BotType1::lookForDeadlySequence1(Position& pos)
{
    // Size of the sequence
    m_seqSize = 5;
    // Max count of blank section expected
    m_maxCount = 1;
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &BotType1::pieceIsMine;
    // Sequence searching function, sequence with 1 vacancy
    m_trackFunc = &BotType1::validSequenceFound;
    // Looking for a linear sequence
    log("DeadlySeq1 tracking starts...");
    return lookForLinearSequence(pos);
}
// Looking for a win sequence of the 2nd order
//  type 1: _***__, _**_*_, _*_**_, __***_,
//  type 2: *_*_*_*,
//  type 3: double type 1 blocked at one side
bool BotType1::lookForWinSequence2(Position& pos)
{
    // Looking for the type 1:
    // Size of the sequence
    m_seqSize = 6;
    // Max count of blank section expected
    m_maxCount = 1;
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &BotType1::pieceIsAdversary;
    // Sequence searching function, sequence with 1 vacancy
    m_trackFunc = &BotType1::freeValidSequenceFound;// TODO, optimize when one end could be blocked
    log("WinSeq2 t1 tracking starts...");
    if(lookForLinearSequence(pos)) return true;

    // Looking for the type 2:
    // Size of the sequence
    m_seqSize = 7;
    // Comparison method, getting a reference to the corresponding function
    m_targetFunc = &BotType1::pieceIsMine;
    // Sequence searching function, alterning chain
    m_trackFunc = &BotType1::alternateSeqFound;
    log("WinSeq2 t2 tracking starts...");
    if(lookForLinearSequence(pos)) return true;

    // Looking for the type 3:

    // TODO

    return false;
}
// Looking for a win sequence of the 2nd order
//  type 1: _***__, _**_*_, _*_**_, __***_,
//  type 2: *_*_*_*,
//  type 3: double type 1 blocked at one side
bool BotType1::lookForDeadlySequence2(Position& pos)
{
    // Looking for the type 1:
    // Size of the sequence
    m_seqSize = 6;
    // Max count of blank section expected
    m_maxCount = 1;
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &BotType1::pieceIsMine;
    // Sequence searching function, sequence with 1 vacancy
    m_trackFunc = &BotType1::freeValidSequenceFound;
    log("DeadlySeq2 t1 tracking starts...");
    if(lookForLinearSequence(pos)) return true;

    // Looking for the type 2:
    // Size of the sequence
    m_seqSize = 7;
    // Comparison method, getting a reference to the corresponding function
    m_targetFunc = &BotType1::pieceIsAdversary;
    // Sequence searching function, alterning chain
    m_trackFunc = &BotType1::alternateSeqFound;
    log("DeadlySeq2 t2 tracking starts...");
    if(lookForLinearSequence(pos)) return true;

    // Looking for the type 3:

    // TODO

    return false;
}
// Looking for a win sequence of the 3rd order
bool BotType1::lookForWinSequence3(Position& pos)
{
    // Size of the sequence
    m_seqSize = 5;
    // Max count of blank section expected
    m_maxCount = 1;
    // Reference line status code
    m_refStatus = line_winIn3;
    // Comparison methods, getting a reference to the corresponding function
    m_targetFunc = &BotType1::pieceIsMine;
    m_inversFunc = &BotType1::pieceIsAdversary;
    // Sequence searching function, sequence with 1 vacancy
    //m_trackFunc = &BotType1::freeValidSequenceFound;// does not fit perfectly (too dramatic)
    //m_trackFunc = &BotType1::sequenceType3Found_old;// Half blind
    m_trackFunc = &BotType1::sequenceType3Found;
    // Looking for a linear sequence
    log("WinSeq3 tracking starts...");
    return lookForLinearSequence(pos);
}
// Looking for a deadly sequence of the 3rd order
bool BotType1::lookForDeadlySequence3(Position& pos)
{
    // Size of the sequence
    m_seqSize = 5;
    // Max count of blank section expected
    m_maxCount = 1;
    // Reference line status code
    m_refStatus = line_loseIn3;
    // Comparison methods, getting a reference to the corresponding function
    m_targetFunc = &BotType1::pieceIsAdversary;
    m_inversFunc = &BotType1::pieceIsMine;
    // Sequence searching function, sequence with 1 vacancy
    //m_trackFunc = &BotType1::freeValidSequenceFound;// does not fit perfectly (too dramatic)
    //m_trackFunc = &BotType1::sequenceType3Found_old;// Half blind
    m_trackFunc = &BotType1::sequenceType3Found;
    log("DeadlySeq3 tracking starts...");
    return lookForLinearSequence(pos);
}
// Looking for a win sequence of the 4th order (V or L-shape)
bool BotType1::lookForWinSequence4(Position& pos)
{
    // Size of the sequence
    m_seqSize = 4;
    //m_seqSize = 5;
    // Max count of blank section expected
    //m_maxCount = 1;
    // Reference line status code
    //m_refStatus = line_winIn4;
    // Comparison method, getting a reference to the corresponding function
    m_targetFunc = &BotType1::pieceIsMine;
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &BotType1::pieceIsAdversary;
    // Sequence searching function, sequence with 1 vacancy
    //m_trackFunc = &BotType1::sequenceType3Found;
    //return lookForLinearSequence(pos);

    // Looking for a sequence:
    // Sequence searching function, free area with a V-shape
    m_trackFunc = &BotType1::sequenceTypeVFound;
    log("WinSeq4 type V tracking starts...");
    if(lookFor2DSequence(pos)) return true;
    // Sequence searching function, free area with a L-shape
    m_trackFunc = &BotType1::sequenceTypeLFound;
    log("WinSeq4 type L tracking starts...");
    if(lookFor2DSequence(pos)) return true;

    // No sequence found
    return false;
}
// Looking for a deadly sequence of the 4th order (V or L-shape)
bool BotType1::lookForDeadlySequence4(Position& pos)
{
    // Size of the sequence
    m_seqSize = 4;
    // Max count of inverse sections expected
    //m_maxCount = 1;
    // Comparison method, getting a reference to the corresponding function
    m_targetFunc = &BotType1::pieceIsAdversary;
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &BotType1::pieceIsMine;

    // Looking for a sequence:
    // Sequence searching function, free area with a V-shape
    m_trackFunc = &BotType1::sequenceTypeVFound;
    log("DeadlySeq4 type V tracking starts...");
    if(lookFor2DSequence(pos)) return true;
    // Sequence searching function, free area with a L-shape
    m_trackFunc = &BotType1::sequenceTypeLFound;
    log("DeadlySeq4 type L tracking starts...");
    if(lookFor2DSequence(pos)) return true;

    // No sequence found
    return false;
}
// Looking for a spot where a win sequence of the 4th order (v-shape) can be placed
bool BotType1::lookForWinSequence5(Position& pos)
{

    // Dummy
    pos = Position(1, 1);

    // TODO

    return false;
}
// Looking for a spot where a single stone is placed and giving him a friend
bool BotType1::lookForBuildPair(Position& pos)
{
    log("BuildPair tracking starts...");

    // Vector containing positions to analyze
    std::vector<PosData> posDataList;
    static bool isOk;
    static PosData s_posData;
    // Scanning the board
    for (size_t i = 0; i < m_boardW * m_boardH; ++i)
    {
        m_in = m_board->getPiece(xFromIndex(i), yFromIndex(i));
        if(!pieceIsMine(m_in))
            continue;
        // Scanning around
        for(int k{-1}; k <= 1; ++k){
            for(int l{-1}; l <= 1; ++l){
                if((k || l) && m_board->isEmptyAt(pos.x() - 1 - k, pos.y() - 1 - l, &isOk)){
                    if(!isOk)
                        continue;
                    s_posData = getEmptyPosDataAt(pos.x() - 1 - k, pos.y() - 1 - l);
                    if(std::count(posDataList.begin(), posDataList.end(), s_posData) <= 0)
                        posDataList.push_back(s_posData);
                }
            }
        }
    }

    // If we came to this point, the ideal place for the stone has to be defined
    m_dir = dir_nodir;
    static int s_emptyLineCount;
    // 3 empty lines
    for(PosData& posData : posDataList){
        lookAround(posData);// Not the ideal function TODO
        s_emptyLineCount = 0;
        if(posData.m_codeX == line_empty) ++s_emptyLineCount;
        if(posData.m_codeY == line_empty) ++s_emptyLineCount;
        if(posData.m_codeD == line_empty) ++s_emptyLineCount;
        if(posData.m_codeA == line_empty) ++s_emptyLineCount;
        if(s_emptyLineCount >= 3){
            pos = Position(posData.m_x, posData.m_y);
            return true;
        }
    }
    // 2 empty lines
    for(PosData& posData : posDataList){
        lookAround(posData);// Not the ideal function TODO
        s_emptyLineCount = 0;
        if(posData.m_codeX == line_empty) ++s_emptyLineCount;
        if(posData.m_codeY == line_empty) ++s_emptyLineCount;
        if(posData.m_codeD == line_empty) ++s_emptyLineCount;
        if(posData.m_codeA == line_empty) ++s_emptyLineCount;
        if(s_emptyLineCount >= 2){
            pos = Position(posData.m_x, posData.m_y);
            return true;
        }
    }
    // 1 empty line
    for(PosData& posData : posDataList){
        lookAround(posData);// Not the ideal function TODO
        s_emptyLineCount = 0;
        if(posData.m_codeX == line_empty) ++s_emptyLineCount;
        if(posData.m_codeY == line_empty) ++s_emptyLineCount;
        if(posData.m_codeD == line_empty) ++s_emptyLineCount;
        if(posData.m_codeA == line_empty) ++s_emptyLineCount;
        if(s_emptyLineCount >= 1){
            pos = Position(posData.m_x, posData.m_y);
            return true;
        }
    }

    return false;
}

// Looking for a linear sequence
bool BotType1::lookForLinearSequence(Position& pos)
{
    // test for sequences along Y - case 0
    m_dir = dir_y;
    m_xIncFunc = &BotType1::incrementSame;
    m_yIncFunc = &BotType1::incrementPlus;
    for(std::size_t i{1}; i <= m_boardW; ++i){
        for(std::size_t j{1}; j <= m_boardH - m_seqSize + 1; ++j){
            if((this->*m_trackFunc)(i, j, pos)){
                log("Sequence found:", pos);
                return true;
            }
        }
    }
    // test for diagonal sequences - case 1
    m_dir = dir_d;
    m_xIncFunc = &BotType1::incrementPlus;
    m_yIncFunc = &BotType1::incrementPlus;
    for(std::size_t i{1}; i <= m_boardW - m_seqSize + 1; ++i){
        for(std::size_t j{1}; j <= m_boardH - m_seqSize + 1; ++j){
            if((this->*m_trackFunc)(i, j, pos)){
                log("Sequence found:", pos);
                return true;
            }
        }
    }
    // test for sequences along X - case 2
    m_dir = dir_x;
    m_xIncFunc = &BotType1::incrementPlus;
    m_yIncFunc = &BotType1::incrementSame;
    for(std::size_t i{1}; i <= m_boardW - m_seqSize + 1; ++i){
        for(std::size_t j{1}; j <= m_boardH; ++j){
            if((this->*m_trackFunc)(i, j, pos)){
                log("Sequence found:", pos);
                return true;
            }
        }
    }
    // test for anti diagonal sequences - case 3
    m_dir = dir_a;
    m_xIncFunc = &BotType1::incrementPlus;
    m_yIncFunc = &BotType1::incrementMinus;
    for(std::size_t i{1}; i <= m_boardW - m_seqSize + 1; ++i){
        for(std::size_t j{m_seqSize}; j <= m_boardH; ++j){
            if((this->*m_trackFunc)(i, j, pos)){
                log("Sequence found:", pos);
                return true;
            }
        }
    }

    log("Look for linear sequence: no sequence found.");
    // No sequence found, leaving
    return false;
}
// Scanning the board calling the appropriate tracking function
bool BotType1::lookFor2DSequence(Position& pos)
{
    for(std::size_t i{ m_seqSize }; i <= m_boardW - m_seqSize + 1; ++i){
        for(std::size_t j{ m_seqSize }; j <= m_boardH - m_seqSize + 1; ++j){
            if((this->*m_trackFunc)(i, j, pos)){
                log("Sequence found:", pos);
                return true;
            }
        }
    }

    log("Look for 2D sequence: no sequence found.");
    // No sequence found, leaving
    return false;
}
// Reading a sequence
bool BotType1::validSequenceFound(std::size_t inX, std::size_t inY, Position& pos)
{
    static std::size_t s_count;
    s_count = 0;
    for(std::size_t k{0}; k < m_seqSize; ++k){
        m_in = m_board->getPiece(inX + (this->*m_xIncFunc)(k), inY + (this->*m_yIncFunc)(k));
        //if(m_enableLog) m_fileStream << "Point check: " << Position(inX + (this->*m_xIncFunc)(k), inY + (this->*m_yIncFunc)(k)) << '\n';
        // Sequence is broken, no need to go further
        if((this->*m_inversFunc)(m_in)) return false;
        // Hole in the sequence, decreasing the count, stopping if necessary
        else if(pieceIsNone(m_in)){
            if(++s_count > m_maxCount) return false;
            else pos = Position(inX + (this->*m_xIncFunc)(k), inY + (this->*m_yIncFunc)(k));
        }
    }
    return true;
}
// Reading a sequence (limits must be free)
bool BotType1::freeValidSequenceFound(std::size_t inX, std::size_t inY, Position& pos)
{
    static std::size_t s_count;
    s_count = 0;
    // Vector containing positions to analyze
    std::vector<PosData> posDataList;
    // Checking first position
    m_in = m_board->getPiece(inX, inY);
    if(!pieceIsNone(m_in)) return false;
    posDataList.push_back(getEmptyPosDataAt(inX, inY));
    // Checking last position
    m_in = m_board->getPiece(inX + (this->*m_xIncFunc)(m_seqSize - 1), inY + (this->*m_yIncFunc)(m_seqSize - 1));
    if(!pieceIsNone(m_in)) return false;
    posDataList.push_back(getEmptyPosDataAt(inX + (this->*m_xIncFunc)(m_seqSize - 1), inY + (this->*m_yIncFunc)(m_seqSize - 1)));
    // Checking the inside of the sequence
    for(std::size_t k{1}; k < m_seqSize - 1; ++k){
        m_in = m_board->getPiece(inX + (this->*m_xIncFunc)(k), inY + (this->*m_yIncFunc)(k));
        // Sequence is broken, no need to go further
        if((this->*m_inversFunc)(m_in)) return false;
        // Hole in the sequence, decreasing the count, stopping if necessary
        else if(pieceIsNone(m_in)){
            if(++s_count > m_maxCount) return false;
            else{
                pos = Position(inX + (this->*m_xIncFunc)(k), inY + (this->*m_yIncFunc)(k));
                posDataList.push_back(getEmptyPosDataAt(pos));
            }
        }
    }

    // If we came to this point, the ideal place for the stone has to be defined
    for(PosData& posData : posDataList) lookAround(posData);
    // Analyzing positions
    pos = evaluatePositions(posDataList);

    // Returning the result
    return true;
}
bool BotType1::alternateSeqFound(std::size_t inX, std::size_t inY, Position& pos)
{
    static bool s_target;
    // The first piece must be filled
    m_in = m_board->getPiece(inX, inY);
    // Sequence not initialized, no need to go further
    if(!(this->*m_targetFunc)(m_in)) return false;
    s_target = true;

    // Scanning
    for(std::size_t k{1}; k < m_seqSize; ++k){
        m_in = m_board->getPiece(inX + (this->*m_xIncFunc)(k), inY + (this->*m_yIncFunc)(k));
        // Sequence is broken, no need to go further
        if((this->*m_inversFunc)(m_in)) return false;
        // Checking the slot, toggling the target boolean
        if(s_target && pieceIsNone(m_in)) s_target = false;
        else if(!s_target && (this->*m_targetFunc)(m_in))  s_target = false;
        else return false;
    }

    // Defining the position. Same for odd and even since we start with the target piece
    pos = Position(inX + (this->*m_xIncFunc)(m_seqSize / 2), inY + (this->*m_yIncFunc)(m_seqSize / 2));

    return true;
}
// Reading a sequence (limits must be free, evaluation must return symetrical data)
bool BotType1::sequenceType3Found_old(std::size_t inX, std::size_t inY, Position& pos)
{
    static std::size_t s_count;
    s_count = 0;
    // Vector containing positions to analyze
    std::vector<PosData> posDataList;
    // Checking first position
    m_in = m_board->getPiece(inX, inY);
    if(!pieceIsNone(m_in)) return false;
    posDataList.push_back(getEmptyPosDataAt(inX, inY));
    // Checking last position
    m_in = m_board->getPiece(inX + (this->*m_xIncFunc)(m_seqSize - 1), inY + (this->*m_yIncFunc)(m_seqSize - 1));
    if(!pieceIsNone(m_in)) return false;
    posDataList.push_back(getEmptyPosDataAt(inX + (this->*m_xIncFunc)(m_seqSize - 1), inY + (this->*m_yIncFunc)(m_seqSize - 1)));
    // Checking the inside of the sequence
    for(std::size_t k{1}; k < m_seqSize - 1; ++k){
        m_in = m_board->getPiece(inX + (this->*m_xIncFunc)(k), inY + (this->*m_yIncFunc)(k));
        // Sequence is broken, no need to go further
        if((this->*m_inversFunc)(m_in)) return false;
        // Hole in the sequence, decreasing the count, stopping if necessary
        else if(pieceIsNone(m_in)){
            if(++s_count > m_maxCount) return false;
            else{
                pos = Position(inX + (this->*m_xIncFunc)(k), inY + (this->*m_yIncFunc)(k));
                posDataList.push_back(getEmptyPosDataAt(pos));
            }
        }
    }

    // If we came to this point, the ideal place for the stone has to be defined
    for(PosData& posData : posDataList){
        lookAround(posData);
        if(posData.m_codeX <= m_refStatus
                || posData.m_codeY <= m_refStatus
                || posData.m_codeD <= m_refStatus
                || posData.m_codeA <= m_refStatus){
            pos = Position(posData.m_x, posData.m_y);
            return true;
        }
    }

    // Returning the result
    return false;
}
// Reading a sequence (limits must be not inverse, evaluation shall be <= reference)
bool BotType1::sequenceType3Found(std::size_t inX, std::size_t inY, Position& pos)
{
    static std::size_t s_count;
    s_count = 0;
    // Vector containing positions to analyze
    std::vector<PosData> posDataList;

    // Checking first position
    m_in = m_board->getPiece(inX, inY);
    if((this->*m_inversFunc)(m_in)) return false;
    else if(pieceIsNone(m_in))
        posDataList.push_back(getEmptyPosDataAt(inX, inY));

    // Checking last position
    m_in = m_board->getPiece(inX + (this->*m_xIncFunc)(m_seqSize - 1), inY + (this->*m_yIncFunc)(m_seqSize - 1));
    if((this->*m_inversFunc)(m_in)) return false;
    else if(pieceIsNone(m_in))
        posDataList.push_back(getEmptyPosDataAt(inX + (this->*m_xIncFunc)(m_seqSize - 1), inY + (this->*m_yIncFunc)(m_seqSize - 1)));

    // Checking the inside of the sequence
    for(std::size_t k{1}; k < m_seqSize - 1; ++k){
        m_in = m_board->getPiece(inX + (this->*m_xIncFunc)(k), inY + (this->*m_yIncFunc)(k));
        // Sequence is broken, no need to go further
        if((this->*m_inversFunc)(m_in)) return false;
        // Hole in the sequence, decreasing the count, stopping if necessary
        else if(pieceIsNone(m_in)){
            if(++s_count > m_maxCount) return false;
            else{
                pos = Position(inX + (this->*m_xIncFunc)(k), inY + (this->*m_yIncFunc)(k));
                posDataList.push_back(getEmptyPosDataAt(pos));
            }
        }
    }

    // If we came to this point, the ideal place for the stone has to be defined
    for(PosData& posData : posDataList){
        lookAround(posData);
        if(posData.m_codeX <= m_refStatus
                || posData.m_codeY <= m_refStatus
                || posData.m_codeD <= m_refStatus
                || posData.m_codeA <= m_refStatus){
            pos = Position(posData.m_x, posData.m_y);
            return true;
        }
    }

    // Returning the result
    return false;
}
// Reading a sequence (a V-shape with free spots around)
bool BotType1::sequenceTypeVFound(std::size_t inX, std::size_t inY, Position& pos)
{
    // Pieces at each direct diagonal
    static Piece s_tl;
    static Piece s_tr;
    static Piece s_bl;
    static Piece s_br;
    // Line check structure
    static LineData s_lineData1;
    static LineData s_lineData2;
    // Checking whether the central position is a target piece
    m_in = m_board->getPiece(inX, inY);
    if(!(this->*m_targetFunc)(m_in)) return false;
    // Checking the pieces at the direct corners:
    // Getting the pieces
    s_tl = m_board->getPiece(inX - 1, inY + 1);
    s_tr = m_board->getPiece(inX + 1, inY + 1);
    s_bl = m_board->getPiece(inX - 1, inY - 1);
    s_br = m_board->getPiece(inX + 1, inY - 1);
    // Checking diagonal
    if(!((this->*m_targetFunc)(s_tr) && pieceIsNone(s_bl)) &&
            !((this->*m_targetFunc)(s_bl) && pieceIsNone(s_tr)))
        return false;
    // Checking antidiagonal
    if(!((this->*m_targetFunc)(s_tl) && pieceIsNone(s_br)) &&
            !((this->*m_targetFunc)(s_br) && pieceIsNone(s_tl)))
        return false;

    // If we got to this point, a v was found with free spots around.
    // Checking whether it can be exploitable, first diagonal:
    set2DIncrementDia();
    lookAlong(inX, inY, m_seqSize, s_lineData1);
    if(lineIsDead(s_lineData1)) return false;
    // Checking whether it can be exploitable, second diagonal:
    set2DIncrementAdia();
    lookAlong(inX, inY, m_seqSize, s_lineData2);
    if(lineIsDead(s_lineData2)) return false;

    // Still there, dangerous v found.
    // Defining the two positions to work with and the direction of the lines
    static Position s_pos1;
    static Position s_pos2;
    if(((this->*m_targetFunc)(s_tl) && (this->*m_targetFunc)(s_tr)) ||
            ((this->*m_targetFunc)(s_bl) && (this->*m_targetFunc)(s_br))){
        // Position
        s_pos1.setX(inX - 1);
        s_pos1.setY(inY);
        s_pos2.setX(inX + 1);
        s_pos2.setY(inY);
        // Direction
        set2DIncrementVer();
    }
    else if(((this->*m_targetFunc)(s_tr) && (this->*m_targetFunc)(s_br)) ||
            ((this->*m_targetFunc)(s_tl) && (this->*m_targetFunc)(s_bl))){
        // Position
        s_pos1.setX(inX);
        s_pos1.setY(inY - 1);
        s_pos2.setX(inX);
        s_pos2.setY(inY + 1);
        // Direction
        set2DIncrementHor();
    }

    // Checking the points
    if(!pieceIsNone(m_board->getPiece(s_pos1)) && !pieceIsNone(m_board->getPiece(s_pos2))) return false;
    // Then checking the lines
    if(pieceIsNone(m_board->getPiece(s_pos1))) lookAlong(s_pos1.x(), s_pos1.y(), m_seqSize, s_lineData1);
    else s_lineData1.m_innBlocked = true;
    if(pieceIsNone(m_board->getPiece(s_pos2))) lookAlong(s_pos2.x(), s_pos2.y(), m_seqSize, s_lineData2);
    else s_lineData2.m_innBlocked = true;
    // Consequences:
    if(lineIsDead(s_lineData1) && lineIsDead(s_lineData2)) return false;
    else if(lineIsDead(s_lineData1)) pos = s_pos2;
    else if(lineIsDead(s_lineData2)) pos = s_pos1;
    else if(s_lineData1.m_countTar < s_lineData2.m_countTar) pos = s_pos2;
    else if(s_lineData1.m_countTar > s_lineData2.m_countTar) pos = s_pos1;
    else if(getRandomInt(1, 2) == 1) pos = s_pos1;
    else pos = s_pos2;

    // We did it!
    return true;
}
// Reading a sequence (a L-shape with free spots around)
bool BotType1::sequenceTypeLFound(std::size_t inX, std::size_t inY, Position& pos)
{

    // Dummy TODO
    inX = 0;
    inY = 0;
    if(inX && inY) pos = Position(inX, inY);


    return false;
}

// Looking around a given position
void BotType1::lookAround(BotType1::PosData& posData){
    // Filling with information
    if(m_dir != dir_x){
        set2DIncrementHor();
        lookAlong(posData.m_x, posData.m_y, posData.m_codeX);
    }
    if(m_dir != dir_y){
        set2DIncrementVer();
        lookAlong(posData.m_x, posData.m_y, posData.m_codeY);
    }
    if(m_dir != dir_d){
        set2DIncrementDia();
        lookAlong(posData.m_x, posData.m_y, posData.m_codeD);
    }
    if(m_dir != dir_a){
        set2DIncrementAdia();
        lookAlong(posData.m_x, posData.m_y, posData.m_codeA);
    }
}
// Looking along a given line (up to 5 cells away in both directions)
void BotType1::lookAlong(std::size_t x, std::size_t y, LineStatus& code){
    static const int s_range{ 5 };
    static int sX;
    static int sY;
    static int eX;
    static int eY;
    static int countAd;
    static int countMe;
    static LineStatus s_code;
    static bool s_ok;
    s_code = line_unknown;
    code = line_unknown;
    // Scanning each sequence possibility
    for(int i{0}; i < s_range; ++i){
        s_ok = true;
        // Checking if the start coordinates are valid
        sX = x + (this->*m_x2DDecFunc)(i);
        sY = y + (this->*m_y2DDecFunc)(i);
        if(sX < 1 || sY < 1) s_ok = false;
        else if(static_cast<std::size_t>(sX) > m_boardW || static_cast<std::size_t>(sY) > m_boardH) s_ok = false;

        // Checking if the end coordinates are valid
        eX = x + (this->*m_x2DIncFunc)(s_range - i);
        eY = y + (this->*m_y2DIncFunc)(s_range - i);
        if(eX < 1 || eY < 1) s_ok = false;
        else if(static_cast<std::size_t>(eX) > m_boardW || static_cast<std::size_t>(eY) > m_boardH) s_ok = false;

        // Scanning the line
        if(s_ok){
            countAd = 0;
            countMe = 0;
            for(int j{0}; j < s_range; ++j){
                m_in = m_board->getPiece(sX + (this->*m_x2DIncFunc)(j), sY + (this->*m_y2DIncFunc)(j));
                if(pieceIsAdversary(m_in)) ++countAd;
                else if(pieceIsMine(m_in)) ++countMe;
            }
            // Extracting data
            if(countAd && countMe) s_code = line_dead;
            else if(countAd){
                switch(countAd){
                case s_range - 1:
                    s_code = line_loseIn1;
                    break;
                case s_range - 2:
                    s_code = line_loseIn2;
                    break;
                case s_range - 3:
                    s_code = line_loseIn3;
                    break;
                case s_range - 4:
                    s_code = line_loseIn4;
                    break;
                default:
                    s_code = line_unknown;
                }
            }
            else if(countMe){
                switch(countMe){
                case s_range - 1:
                    s_code = line_winIn1;
                    break;
                case s_range - 2:
                    s_code = line_winIn2;
                    break;
                case s_range - 3:
                    s_code = line_winIn3;
                    break;
                case s_range - 4:
                    s_code = line_winIn4;
                    break;
                default:
                    s_code = line_unknown;
                }
            }
            else s_code = line_empty;
            // Saving the result
            if(s_code < code) code = s_code;
        }
    }
}
// Looking along a given line centered on the input point (variable size)
void BotType1::lookAlong(std::size_t x, std::size_t y, std::size_t size, LineData& lineData){
    // Initializing line data
    resetLineData(lineData);

    // Scanning the line
    for(std::size_t i{ 1 }; i < size; ++i){
        // Checking the decremental side
        m_in = m_board->getPiece(x + (this->*m_x2DDecFunc)(i), y + (this->*m_x2DDecFunc)(i));
        if((this->*m_targetFunc)(m_in)) ++(lineData.m_countTar);
        else if((this->*m_inversFunc)(m_in)){
            ++(lineData.m_countInv);
            if(i == size - 1) lineData.m_begBlocked = true;
            else lineData.m_innBlocked = true;
        }
        // Checking the incremental side
        m_in = m_board->getPiece(x + (this->*m_x2DIncFunc)(i), y + (this->*m_x2DIncFunc)(i));
        if((this->*m_targetFunc)(m_in)) ++(lineData.m_countTar);
        else if((this->*m_inversFunc)(m_in)){
            ++(lineData.m_countInv);
            if(i == size - 1) lineData.m_endBlocked = true;
            else lineData.m_innBlocked = true;
        }
    }
}

// Evaluating a list of position data
Position BotType1::evaluatePositions(const std::vector<PosData>& list){
    // Position to return
    Position pos;

    // List is bullshit, stopping here
    if(list.size() == 0) return pos;

    // List of codes and comparison values
    static LineStatus s_stati[dir_count];
    static int s_refCode;
    static int s_curCode;
    static int s_mult;

    // Building the reference code
    s_mult = 1;
    s_refCode = 0;
    //for(int i{0}; i < dir_count; ++i) s_stati[i] = line_unknown;
    for(int i{0}; i < dir_count; ++i){
        s_refCode += line_unknown * s_mult;
        s_mult *= line_maxCount;
    }

    // Scanning the list
    for(const PosData& posData : list){
        // Getting values to evaluate
        s_stati[dir_x] = posData.m_codeX;
        s_stati[dir_d] = posData.m_codeD;
        s_stati[dir_y] = posData.m_codeY;
        s_stati[dir_a] = posData.m_codeA;
        // Sorting
        std::sort(s_stati, s_stati + dir_count);
        // Building the reference code
        s_mult = 1;
        s_curCode = 0;
        for(int i{0}; i < dir_count; ++i){
            s_refCode += s_stati[i] * s_mult;
            s_mult *= line_maxCount;
        }
        // Comparing
        if(s_curCode < s_refCode){
            s_curCode = s_refCode;
            pos = Position(posData.m_x, posData.m_y);
        }
    }
    // Returning the result
    return pos;
}

// Getting an empty posData object with the given position
inline BotType1::PosData BotType1::getEmptyPosDataAt(const Position& pos) const{
    return getEmptyPosDataAt(pos.x(), pos.y());
}
inline BotType1::PosData BotType1::getEmptyPosDataAt(std::size_t x, std::size_t y) const{
    return {x, y,//0, 0, 0, 0};
            LineStatus::line_unknown,
            LineStatus::line_unknown,
            LineStatus::line_unknown,
            LineStatus::line_unknown};
}

// Resetting a LineData object
inline void BotType1::resetLineData(LineData& lineData) const{
    lineData.m_countInv = 0;
    lineData.m_countTar = 0;
    lineData.m_begBlocked = false;
    lineData.m_endBlocked = false;
    lineData.m_innBlocked = false;
}
// Telling the line is dead
inline bool BotType1::lineIsDead(const LineData& lineData) const{
    return (lineData.m_innBlocked || (lineData.m_begBlocked && lineData.m_endBlocked));
}

// Operator overload
bool operator==(const BotType1::PosData& a, const BotType1::PosData& b){
    return (a.m_x == b.m_x && a.m_y == b.m_y);
}

} // End namespace joseph

} // End namespace FIAR
