#include "Players/PlayerJoseph.h"
#include "Utilities.h"
#include <algorithm>

namespace FIAR{

// Constructor
PlayerJoseph::PlayerJoseph(const Board* board, Piece piece)
    : PlayerBase( board, "JosephBot", piece )
    , m_boardW{ board->getSizeX() }
    , m_boardH{ board->getSizeY() }
    , m_fields{ m_boardW * m_boardH }
    , m_evalsW( m_fields )
    , m_evalsL( m_fields )
    , m_imgEval{ m_boardW * m_imgZoom, m_boardH * m_imgZoom }
    , m_imgChoice{ m_boardW * m_imgZoom, m_boardH * m_imgZoom }
{

    if(m_enableLog) m_fileStream = std::ofstream("JosephBotLog.txt");

    log("Board size x:", m_boardW);
    log("Board size y:", m_boardH);
}

// Destructor
PlayerJoseph::~PlayerJoseph(){
    log("Rounds:", m_rndCnt);
    log("Errors:", m_errCnt);
    log("Randomized count:", m_ranCnt);
    log("Randomized percentage:", (m_ranCnt * 100) / m_rndCnt);
}

// Function to use to make an action
Position PlayerJoseph::doAction(){
    static TimeEvaluation waitingTime;
    log("Waiting time:", waitingTime);

    // Position where the stone has to be placed
    Position pos;
    ++m_rndCnt;
    log("NewRound:", m_rndCnt);

    // Playing an algorithm
    playNewAlgo(pos);
    log("Evaluation:", pos);
    //playOldAlgo(pos);
    //std::cout << pos << '\n';

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

// Playing an algorithm
void PlayerJoseph::playNewAlgo(Position& pos){
    // Evaluating the board
    TimeEvaluation evalTime;
    evaluateBoard(pos);
    log("Board evaluation time:", evalTime);

    // Drawing the images
    if (!m_enableImg)
        return;
    TimeEvaluation imgTime;
    makeEvalImgs(pos);
    log("Image update time:", imgTime);
}
void PlayerJoseph::playOldAlgo(Position& pos){
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
}

// Looking for a win sequence of the 1st order (****_, ***_*, **_**, *_***, _****)
bool PlayerJoseph::lookForWinSequence1(Position& pos){
    // Size of the sequence
    m_seqSize = 5;
    // Max count of blank section expected
    m_maxCount = 1;
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &PlayerJoseph::pieceIsAdversary;
    // Sequence searching function, sequence with 1 vacancy
    m_trackFunc = &PlayerJoseph::validSequenceFound;
    // Looking for a linear sequence
    log("WinSeq1 tracking starts...");
    return lookForLinearSequence(pos);
}
// Looking for a deadly sequence of the 1st order (****_, ***_*, **_**, *_***, _****)
bool PlayerJoseph::lookForDeadlySequence1(Position& pos){
    // Size of the sequence
    m_seqSize = 5;
    // Max count of blank section expected
    m_maxCount = 1;
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &PlayerJoseph::pieceIsMine;
    // Sequence searching function, sequence with 1 vacancy
    m_trackFunc = &PlayerJoseph::validSequenceFound;
    // Looking for a linear sequence
    log("DeadlySeq1 tracking starts...");
    return lookForLinearSequence(pos);
}
// Looking for a win sequence of the 2nd order
//  type 1: _***__, _**_*_, _*_**_, __***_,
//  type 2: *_*_*_*,
//  type 3: double type 1 blocked at one side
bool PlayerJoseph::lookForWinSequence2(Position& pos){
    // Looking for the type 1:
    // Size of the sequence
    m_seqSize = 6;
    // Max count of blank section expected
    m_maxCount = 1;
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &PlayerJoseph::pieceIsAdversary;
    // Sequence searching function, sequence with 1 vacancy
    m_trackFunc = &PlayerJoseph::freeValidSequenceFound;// TODO, optimize when one end could be blocked
    log("WinSeq2 t1 tracking starts...");
    if(lookForLinearSequence(pos)) return true;

    // Looking for the type 2:
    // Size of the sequence
    m_seqSize = 7;
    // Comparison method, getting a reference to the corresponding function
    m_targetFunc = &PlayerJoseph::pieceIsMine;
    // Sequence searching function, alterning chain
    m_trackFunc = &PlayerJoseph::alternateSeqFound;
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
bool PlayerJoseph::lookForDeadlySequence2(Position& pos){
    // Looking for the type 1:
    // Size of the sequence
    m_seqSize = 6;
    // Max count of blank section expected
    m_maxCount = 1;
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &PlayerJoseph::pieceIsMine;
    // Sequence searching function, sequence with 1 vacancy
    m_trackFunc = &PlayerJoseph::freeValidSequenceFound;
    log("DeadlySeq2 t1 tracking starts...");
    if(lookForLinearSequence(pos)) return true;

    // Looking for the type 2:
    // Size of the sequence
    m_seqSize = 7;
    // Comparison method, getting a reference to the corresponding function
    m_targetFunc = &PlayerJoseph::pieceIsAdversary;
    // Sequence searching function, alterning chain
    m_trackFunc = &PlayerJoseph::alternateSeqFound;
    log("DeadlySeq2 t2 tracking starts...");
    if(lookForLinearSequence(pos)) return true;

    // Looking for the type 3:

    // TODO

    return false;
}
// Looking for a win sequence of the 3rd order
bool PlayerJoseph::lookForWinSequence3(Position& pos){
    // Size of the sequence
    m_seqSize = 5;
    // Max count of blank section expected
    m_maxCount = 1;
    // Reference line status code
    m_refStatus = line_winIn3;
    // Comparison methods, getting a reference to the corresponding function
    m_targetFunc = &PlayerJoseph::pieceIsMine;
    m_inversFunc = &PlayerJoseph::pieceIsAdversary;
    // Sequence searching function, sequence with 1 vacancy
    //m_trackFunc = &PlayerJoseph::freeValidSequenceFound;// does not fit perfectly (too dramatic)
    //m_trackFunc = &PlayerJoseph::sequenceType3Found_old;// Half blind
    m_trackFunc = &PlayerJoseph::sequenceType3Found;
    // Looking for a linear sequence
    log("WinSeq3 tracking starts...");
    return lookForLinearSequence(pos);
}
// Looking for a deadly sequence of the 3rd order
bool PlayerJoseph::lookForDeadlySequence3(Position& pos){
    // Size of the sequence
    m_seqSize = 5;
    // Max count of blank section expected
    m_maxCount = 1;
    // Reference line status code
    m_refStatus = line_loseIn3;
    // Comparison methods, getting a reference to the corresponding function
    m_targetFunc = &PlayerJoseph::pieceIsAdversary;
    m_inversFunc = &PlayerJoseph::pieceIsMine;
    // Sequence searching function, sequence with 1 vacancy
    //m_trackFunc = &PlayerJoseph::freeValidSequenceFound;// does not fit perfectly (too dramatic)
    //m_trackFunc = &PlayerJoseph::sequenceType3Found_old;// Half blind
    m_trackFunc = &PlayerJoseph::sequenceType3Found;
    log("DeadlySeq3 tracking starts...");
    return lookForLinearSequence(pos);
}
// Looking for a win sequence of the 4th order (V or L-shape)
bool PlayerJoseph::lookForWinSequence4(Position& pos){
    // Size of the sequence
    m_seqSize = 4;
    //m_seqSize = 5;
    // Max count of blank section expected
    //m_maxCount = 1;
    // Reference line status code
    //m_refStatus = line_winIn4;
    // Comparison method, getting a reference to the corresponding function
    m_targetFunc = &PlayerJoseph::pieceIsMine;
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &PlayerJoseph::pieceIsAdversary;
    // Sequence searching function, sequence with 1 vacancy
    //m_trackFunc = &PlayerJoseph::sequenceType3Found;
    //return lookForLinearSequence(pos);

    // Looking for a sequence:
    // Sequence searching function, free area with a V-shape
    m_trackFunc = &PlayerJoseph::sequenceTypeVFound;
    log("WinSeq4 type V tracking starts...");
    if(lookFor2DSequence(pos)) return true;
    // Sequence searching function, free area with a L-shape
    m_trackFunc = &PlayerJoseph::sequenceTypeLFound;
    log("WinSeq4 type L tracking starts...");
    if(lookFor2DSequence(pos)) return true;

    // No sequence found
    return false;
}
// Looking for a deadly sequence of the 4th order (V or L-shape)
bool PlayerJoseph::lookForDeadlySequence4(Position& pos){
    // Size of the sequence
    m_seqSize = 4;
    // Max count of inverse sections expected
    //m_maxCount = 1;
    // Comparison method, getting a reference to the corresponding function
    m_targetFunc = &PlayerJoseph::pieceIsAdversary;
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &PlayerJoseph::pieceIsMine;

    // Looking for a sequence:
    // Sequence searching function, free area with a V-shape
    m_trackFunc = &PlayerJoseph::sequenceTypeVFound;
    log("DeadlySeq4 type V tracking starts...");
    if(lookFor2DSequence(pos)) return true;
    // Sequence searching function, free area with a L-shape
    m_trackFunc = &PlayerJoseph::sequenceTypeLFound;
    log("DeadlySeq4 type L tracking starts...");
    if(lookFor2DSequence(pos)) return true;

    // No sequence found
    return false;
}
// Looking for a spot where a win sequence of the 4th order (v-shape) can be placed
bool PlayerJoseph::lookForWinSequence5(Position& pos){

    // Dummy
    pos = Position(1, 1);

    // TODO

    return false;
}
// Looking for a spot where a single stone is placed and giving him a friend
bool PlayerJoseph::lookForBuildPair(Position& pos){
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
    m_dir = jodir_nodir;
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
bool PlayerJoseph::lookForLinearSequence(Position& pos){
    // test for sequences along Y - case 0
    m_dir = jodir_y;
    m_xIncFunc = &PlayerJoseph::incrementSame;
    m_yIncFunc = &PlayerJoseph::incrementPlus;
    for(std::size_t i{1}; i <= m_boardW; ++i){
        for(std::size_t j{1}; j <= m_boardH - m_seqSize + 1; ++j){
            if((this->*m_trackFunc)(i, j, pos)){
                log("Sequence found:", pos);
                return true;
            }
        }
    }
    // test for diagonal sequences - case 1
    m_dir = jodir_d;
    m_xIncFunc = &PlayerJoseph::incrementPlus;
    m_yIncFunc = &PlayerJoseph::incrementPlus;
    for(std::size_t i{1}; i <= m_boardW - m_seqSize + 1; ++i){
        for(std::size_t j{1}; j <= m_boardH - m_seqSize + 1; ++j){
            if((this->*m_trackFunc)(i, j, pos)){
                log("Sequence found:", pos);
                return true;
            }
        }
    }
    // test for sequences along X - case 2
    m_dir = jodir_x;
    m_xIncFunc = &PlayerJoseph::incrementPlus;
    m_yIncFunc = &PlayerJoseph::incrementSame;
    for(std::size_t i{1}; i <= m_boardW - m_seqSize + 1; ++i){
        for(std::size_t j{1}; j <= m_boardH; ++j){
            if((this->*m_trackFunc)(i, j, pos)){
                log("Sequence found:", pos);
                return true;
            }
        }
    }
    // test for anti diagonal sequences - case 3
    m_dir = jodir_a;
    m_xIncFunc = &PlayerJoseph::incrementPlus;
    m_yIncFunc = &PlayerJoseph::incrementMinus;
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
bool PlayerJoseph::lookFor2DSequence(Position& pos){
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
bool PlayerJoseph::validSequenceFound(std::size_t inX, std::size_t inY, Position& pos){
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
bool PlayerJoseph::freeValidSequenceFound(std::size_t inX, std::size_t inY, Position& pos){
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
bool PlayerJoseph::alternateSeqFound(std::size_t inX, std::size_t inY, Position& pos){
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
bool PlayerJoseph::sequenceType3Found_old(std::size_t inX, std::size_t inY, Position& pos){
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
bool PlayerJoseph::sequenceType3Found(std::size_t inX, std::size_t inY, Position& pos){
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
bool PlayerJoseph::sequenceTypeVFound(std::size_t inX, std::size_t inY, Position& pos){
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
bool PlayerJoseph::sequenceTypeLFound(std::size_t inX, std::size_t inY, Position& pos){

    // Dummy TODO
    inX = 0;
    inY = 0;
    if(inX && inY) pos = Position(inX, inY);


    return false;
}

// Looking around a given position
void PlayerJoseph::lookAround(PlayerJoseph::PosData& posData){
    // Filling with information
    if(m_dir != jodir_x){
        set2DIncrementHor();
        lookAlong(posData.m_x, posData.m_y, posData.m_codeX);
    }
    if(m_dir != jodir_y){
        set2DIncrementVer();
        lookAlong(posData.m_x, posData.m_y, posData.m_codeY);
    }
    if(m_dir != jodir_d){
        set2DIncrementDia();
        lookAlong(posData.m_x, posData.m_y, posData.m_codeD);
    }
    if(m_dir != jodir_a){
        set2DIncrementAdia();
        lookAlong(posData.m_x, posData.m_y, posData.m_codeA);
    }
}
// Looking along a given line (up to 5 cells away in both directions)
void PlayerJoseph::lookAlong(std::size_t x, std::size_t y, LineStatus& code){
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
void PlayerJoseph::lookAlong(std::size_t x, std::size_t y, std::size_t size, LineData& lineData){
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
Position PlayerJoseph::evaluatePositions(const std::vector<PosData>& list){
    // Position to return
    Position pos;

    // List is bullshit, stopping here
    if(list.size() == 0) return pos;

    // List of codes and comparison values
    static LineStatus s_stati[jodir_count];
    static int s_refCode;
    static int s_curCode;
    static int s_mult;

    // Building the reference code
    s_mult = 1;
    s_refCode = 0;
    //for(int i{0}; i < jodir_count; ++i) s_stati[i] = line_unknown;
    for(int i{0}; i < jodir_count; ++i){
        s_refCode += line_unknown * s_mult;
        s_mult *= line_maxCount;
    }

    // Scanning the list
    for(const PosData& posData : list){
        // Getting values to evaluate
        s_stati[jodir_x] = posData.m_codeX;
        s_stati[jodir_d] = posData.m_codeD;
        s_stati[jodir_y] = posData.m_codeY;
        s_stati[jodir_a] = posData.m_codeA;
        // Sorting
        std::sort(s_stati, s_stati + jodir_count/*, [](LineStatus a, LineStatus b){return a > b;}*/);
        // Building the reference code
        s_mult = 1;
        s_curCode = 0;
        for(int i{0}; i < jodir_count; ++i){
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

// Evaluating the board, filling the evaluation vector with data
void PlayerJoseph::evaluateBoard(Position& pos){
    // Resetting the evaluation value
    m_evalRslt = 0.;
    std::fill(m_evalsW.begin(), m_evalsW.end(), 0.);
    std::fill(m_evalsL.begin(), m_evalsL.end(), 0.);

    // Scanning the board: win
    m_targetFunc = &PlayerJoseph::pieceIsMine;
    scanBoard(pos, &PlayerJoseph::evaluateWin);

    // Scanning the board: lose
    m_targetFunc = &PlayerJoseph::pieceIsAdversary;
    scanBoard(pos, &PlayerJoseph::evaluateLose);

    // Checking whether the eval is still 0., case yes snail algo
    if(m_evalRslt == 0.) scanStar(pos);
}
// Scanning the board calling the appropriate tracking function
void PlayerJoseph::scanBoard(Position& pos, void (PlayerJoseph::*scanFunc)(std::size_t inX, std::size_t inY, Position& pos)){
    // Checking whether the method pointer is ok
    if(!scanFunc)
        return;
    for (size_t i = 0; i < m_fields; ++i){
        const size_t x = xFromIndex(i);
        const size_t y = yFromIndex(i);
        m_in = m_board->getPiece(x, y);
        (this->*scanFunc)(x, y, pos);
    }
}
// Scanning the board in a star pattern, returning the first free spot
void PlayerJoseph::scanStar(Position& pos){
    // Start positions
    static std::size_t s_minX;
    static std::size_t s_maxX;
    static std::size_t s_minY;
    static std::size_t s_maxY;
    // X definition
    s_minX = m_boardW / 2;
    if(m_boardW % 2 == 0) s_maxX = s_minX + 1;
    else s_maxX = s_minX;
    // Y definition
    s_minY = m_boardH / 2;
    if(m_boardH % 2 == 0) s_maxY = s_minY + 1;
    else s_maxY = s_minY;
    // Max increment definition
    static std::size_t s_maxInc;
    s_maxInc = ((m_boardW > m_boardH ? m_boardW : m_boardW) - 1) / 2;

    // Scanning the center
    for(std::size_t x{ s_minX }; x <= s_maxX; ++x){
        for(std::size_t y{ s_minY }; y <= s_maxY; ++y){
            if(pieceIsNone(m_board->getPiece(x, y))){
                pos = Position(x, y);
                return;
            }
        }
    }
    // Scanning the rest
    for(std::size_t i{ 1 }; i <= s_maxInc; ++i){
        if(s_minY - i >= 1){
            for(std::size_t x{ s_minX - i }; x < s_maxX + i; ++x){
                if(pieceIsNone(m_board->getPiece(x, s_minY - i))){
                    pos = Position(x, s_minY - i);
                    return;
                }
            }
        }
        if(s_maxX + i <= m_boardW){
            for(std::size_t y{ s_minY - i }; y < s_maxY + i; ++y){
                if(pieceIsNone(m_board->getPiece(s_maxX + i, y))){
                    pos = Position(s_maxX + i, y);
                    return;
                }
            }
        }
        if(s_maxY + i <= m_boardH){
            for(std::size_t x{ s_maxX + i }; x > s_minX - i; --x){
                if(pieceIsNone(m_board->getPiece(x, s_maxY + i))){
                    pos = Position(x, s_maxY + i);
                    return;
                }
            }
        }
        if(s_minX - i >= 1){
            for(std::size_t y{ s_maxY + i }; y > s_minY - i; --y){
                if(pieceIsNone(m_board->getPiece(s_minX - i, y))){
                    pos = Position(s_minX - i, y);
                    return;
                }
            }
        }
    }
}
// Evaluating win / lose chances at the given position
void PlayerJoseph::evaluateWin(std::size_t inX, std::size_t inY, Position& pos){
    // Slot occupied
    if(!pieceIsNone(m_in))
        return;

    // Slot is free, processing!
    static double s_locEval;
    s_locEval = 0.;
    set2DIncrementHor();
    s_locEval += evaluateLine(inX, inY);
    set2DIncrementVer();
    s_locEval += evaluateLine(inX, inY);
    set2DIncrementDia();
    s_locEval += evaluateLine(inX, inY);
    set2DIncrementAdia();
    s_locEval += evaluateLine(inX, inY);
    // Checking whether the evaluation is higher than the best one
    if(s_locEval > m_evalRslt){
        pos = Position(inX, inY);
        m_evalRslt = s_locEval;
    }
    // Setting evaluation value in the vector
    m_evalsW[indexFromPos(inX, inY)] = s_locEval;
}
void PlayerJoseph::evaluateLose(std::size_t inX, std::size_t inY, Position& pos){
    // Slot occupied
    if(!pieceIsNone(m_in))
        return;

    // Slot is free, processing!
    static double s_locEval;
    s_locEval = 0.;
    set2DIncrementHor();
    s_locEval += evaluateLine(inX, inY);
    set2DIncrementVer();
    s_locEval += evaluateLine(inX, inY);
    set2DIncrementDia();
    s_locEval += evaluateLine(inX, inY);
    set2DIncrementAdia();
    s_locEval += evaluateLine(inX, inY);
    // Checking whether the evaluation is higher than the best one
    if(s_locEval > m_evalRslt){
        pos = Position(inX, inY);
        m_evalRslt = s_locEval;
    }
    // Setting evaluation value in the vector
    m_evalsL[indexFromPos(inX, inY)] = s_locEval;
}
// Evaluating a line
double PlayerJoseph::evaluateLine(std::size_t x, std::size_t y){
    // Result of the line evaluation
    static double s_rslt;
    s_rslt = 0.;
    // Occupation of a line
    enum PieceId : int8_t {
        Blocked = -1,
        Empty = 0,
        Target = 1
    };
    std::vector<PieceId> slots(1, PieceId::Empty);
    slots.reserve(m_evalSize);
    // Evaluation function
    auto checkPiece = [this](const size_t x, const size_t y)
    {
        // Coordinates out of board's range?
        if(x < 1u || y < 1u || x > m_boardW || y > m_boardH)
            return PieceId::Blocked;
        // Loading the piece
        m_in = m_board->getPiece(x, y);
        // Evaluation
        if((this->*m_targetFunc)(m_in))
            return PieceId::Target;
        else if(pieceIsNone(m_in))
            return PieceId::Empty;
        return PieceId::Blocked;
    };

    // Scanning the line decrementally
    for(size_t i{ 1 }; i < m_winLineSize; ++i){
        const PieceId pieceId = checkPiece(x + (this->*m_x2DDecFunc)(i), y + (this->*m_y2DDecFunc)(i));
        if (pieceId == PieceId::Blocked)
            break;
        slots.insert(slots.begin(), pieceId);
    }
    // Scanning the line incrementally
    for(size_t i{ 1 }; i < m_winLineSize; ++i){
        const PieceId pieceId = checkPiece(x + (this->*m_x2DIncFunc)(i), y + (this->*m_y2DIncFunc)(i));
        if (pieceId == PieceId::Blocked)
            break;
        slots.insert(slots.end(), pieceId);
    }
    // Not enough slots to win
    if(slots.size() < m_winLineSize)
        return 0.;

    // Analyzing the slots
    for(auto it = slots.begin(); slots.end() - it >= static_cast<int>(m_winLineSize); ++it){
        const size_t targets = std::count(it, it + m_winLineSize - 1, PieceId::Target);
        if (targets > 0u)
            s_rslt += std::pow(m_evalBase, targets);
    }
    // Result
    return s_rslt;
}

// Telling whether a piece is adversary
inline bool PlayerJoseph::pieceIsAdversary(Piece piece) const{
    return (piece != m_piece && piece != Piece::none);
}
// Telling whether a piece is adversary or none
inline bool PlayerJoseph::pieceIsHostile(Piece piece) const{
    return (piece != m_piece);
}
// Telling whether a there is a piece
inline bool PlayerJoseph::pieceIsNone(Piece piece) const{
    return (piece == Piece::none);
}
// Telling whether the piece is mine
inline bool PlayerJoseph::pieceIsMine(Piece piece) const{
    return (piece == m_piece);
}

// Increment functions
int PlayerJoseph::incrementSame(int /*inc*/) const{
    return 0;
}
int PlayerJoseph::incrementPlus(int inc) const{
    return inc;
}
int PlayerJoseph::incrementMinus(int inc) const{
    return -inc;
}

// Setting the increment / decrement functions according to the direction
void PlayerJoseph::set2DIncrementHor(){
    m_x2DIncFunc = &PlayerJoseph::incrementPlus;
    m_y2DIncFunc = &PlayerJoseph::incrementSame;
    m_x2DDecFunc = &PlayerJoseph::incrementMinus;
    m_y2DDecFunc = &PlayerJoseph::incrementSame;
}
void PlayerJoseph::set2DIncrementVer(){
    m_x2DIncFunc = &PlayerJoseph::incrementSame;
    m_y2DIncFunc = &PlayerJoseph::incrementPlus;
    m_x2DDecFunc = &PlayerJoseph::incrementSame;
    m_y2DDecFunc = &PlayerJoseph::incrementMinus;
}
void PlayerJoseph::set2DIncrementDia(){
    m_x2DIncFunc = &PlayerJoseph::incrementPlus;
    m_y2DIncFunc = &PlayerJoseph::incrementPlus;
    m_x2DDecFunc = &PlayerJoseph::incrementMinus;
    m_y2DDecFunc = &PlayerJoseph::incrementMinus;
}
void PlayerJoseph::set2DIncrementAdia(){
    m_x2DIncFunc = &PlayerJoseph::incrementPlus;
    m_y2DIncFunc = &PlayerJoseph::incrementMinus;
    m_x2DDecFunc = &PlayerJoseph::incrementMinus;
    m_y2DDecFunc = &PlayerJoseph::incrementPlus;
}

// Getting an empty posData object with the given position
inline PlayerJoseph::PosData PlayerJoseph::getEmptyPosDataAt(const Position& pos) const{
    return getEmptyPosDataAt(pos.x(), pos.y());
}
inline PlayerJoseph::PosData PlayerJoseph::getEmptyPosDataAt(std::size_t x, std::size_t y) const{
    return {x, y,//0, 0, 0, 0};
            LineStatus::line_unknown,
            LineStatus::line_unknown,
            LineStatus::line_unknown,
            LineStatus::line_unknown};
}

// Resetting a LineData object
inline void PlayerJoseph::resetLineData(LineData& lineData) const{
    lineData.m_countInv = 0;
    lineData.m_countTar = 0;
    lineData.m_begBlocked = false;
    lineData.m_endBlocked = false;
    lineData.m_innBlocked = false;
}
// Telling the line is dead
inline bool PlayerJoseph::lineIsDead(const LineData& lineData) const{
    return (lineData.m_innBlocked || (lineData.m_begBlocked && lineData.m_endBlocked));
}

// Operator overload
bool operator==(const PlayerJoseph::PosData& a, const PlayerJoseph::PosData& b){
    return (a.m_x == b.m_x && a.m_y == b.m_y);
}

void PlayerJoseph::makeEvalImgs(const Position& pos)
{
    double maxVal = std::max(*std::max_element(m_evalsW.begin(), m_evalsW.end()),
                             *std::max_element(m_evalsL.begin(), m_evalsL.end()));

    for (size_t i = 0u; i < m_fields; ++i){
        const size_t x = xFromIndex(i) - 1;
        const size_t y = yFromIndex(i) - 1;
        const Piece p = m_board->getPiece(x + 1, y + 1);

        if (pieceIsMine(p)){
            setPixel(m_imgEval, x, y, ColorHSV{0u, 0u, 0u});
            setPixel(m_imgChoice, x, y, ColorHSV{0u, 0u, 0u});
            continue;
        }

        if (pieceIsAdversary(p)){
            setPixel(m_imgEval, x, y, ColorHSV{0u, 0u, 127u});
            setPixel(m_imgChoice, x, y, ColorHSV{0u, 0u, 127u});
            continue;
        }

        const double& evalL = m_evalsL.at(i);
        const double& evalW = m_evalsW.at(i);
        const uint16_t h = static_cast<uint16_t>(120. * evalW / (evalW + evalL));
        const uint8_t v = static_cast<uint8_t>(255. * std::max(evalL, evalW) / maxVal);

        setPixelFixVal(m_imgEval, x, y, h, v);
        setPixelFixVal(m_imgChoice, x, y, h, v);
    }

    setPixelFixVal(m_imgChoice, pos.x() - 1, pos.y() - 1, 240u, 255u);

    m_imgEval.save("Pics/" + std::to_string(m_rndCnt) + "_Eval");
    m_imgChoice.save("Pics/" + std::to_string(m_rndCnt) + "_Play");
}

inline void PlayerJoseph::setPixel(JLImage& image, std::size_t x, std::size_t y, const ColorHSV& c){
    for (std::size_t imgX = x * m_imgZoom; imgX < m_imgZoom * (x + 1); ++imgX)
        for (std::size_t imgY = y * m_imgZoom; imgY < m_imgZoom * (y + 1); ++imgY)
            image.setPixel(imgX, imgY, ColorRGB::fromHSV(c));
}

inline void PlayerJoseph::setPixelFixVal(JLImage& image, std::size_t x, std::size_t y, uint16_t hue, uint8_t saturation)
{
    setPixel(image, x, y, ColorHSV{hue, saturation, 255u});
}

}// End namespace FIAR
