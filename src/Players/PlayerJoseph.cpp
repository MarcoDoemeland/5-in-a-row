#include "Players/PlayerJoseph.h"
#include "Utilities.h"
#include <algorithm>

namespace FIAR{

// Constructor
PlayerJoseph::PlayerJoseph(const Board* board, Piece piece)
    : PlayerBase( board, "JosephBot", piece )
    , m_boardW{ board->getSizeX() }
    , m_boardH{ board->getSizeY() }{

    if(m_enableLog) m_fileStream = std::ofstream("JosephBotLog.txt");

    //if(m_enableLog) m_fileStream << "Board size x: " << m_boardW << '\n';
    //if(m_enableLog) m_fileStream << "Board size y: " << m_boardH << '\n';
}

// Destructor
PlayerJoseph::~PlayerJoseph(){
    if(m_enableLog) m_fileStream << "Rounds: " << m_rndCnt << '\n';
    if(m_enableLog) m_fileStream << "Randomized count: " << m_ranCnt << '\n';
    if(m_enableLog) m_fileStream << "Randomized percentage: " << (m_ranCnt * 100) / m_rndCnt << '\n';
}

// Function to use to make an action
Position PlayerJoseph::doAction(){
    // Position where the stone has to be placed
    Position pos;
    log("NewRound:");
    ++m_rndCnt;

    // Looking for a win sequence of the 1st order (****_, ***_*, **_**, *_***, _****)
    if(lookForWinSequence1(pos)) std::cout << pos << '\n';
    // Looking for a deadly sequence of the 1st order (****_, ***_*, **_**, *_***, _****)
    else if(lookForDeadlySequence1(pos)) std::cout << pos << '\n';

    // Looking for a win sequence of the 2nd order (type 1: _***__, _**_*_, _*_**_, __***_, type 2: *_*_*_*)
    else if(lookForWinSequence2(pos)) std::cout << pos << '\n';
    // Looking for a deadly sequence of the 2nd order (type 1: _***__, _**_*_, _*_**_, __***_, type 2: *_*_*_*)
    else if(lookForDeadlySequence2(pos)) std::cout << pos << '\n';

    // Looking for a win sequence of the 3rd order
    else if(lookForWinSequence3(pos)) std::cout << pos << '\n';
    // Looking for a deadly sequence of the 3rd order
    else if(lookForDeadlySequence3(pos)) std::cout << pos << '\n';

    // Looking for a spot where a win sequence of the 3rd order can be placed
    else if(lookForWinSequence4(pos)) std::cout << pos << '\n';

    // Looking for a spot where a single stone is placed and giving him a friend
    else if(lookForBuildPair(pos)) std::cout << pos << '\n';

    // Doing random shit
    else{
        do{
            pos = Position(static_cast<std::size_t>(getRandomInt(1, m_boardW)), static_cast<std::size_t>(getRandomInt(1, m_boardH)));
        }while(!m_board->isEmptyAt(pos));
        std::cout << pos << '\n';
        ++m_ranCnt;
    }

    // End of round, jumpline to make the log clearer
    log("");

    // Returning the position
    return pos;
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
// Looking for a win sequence of the 2nd order (type 1: _***__, _**_*_, _*_**_, __***_, type 2: *_*_*_*)
bool PlayerJoseph::lookForWinSequence2(Position& pos){
    // Size of the sequence
    m_seqSize = 6;
    // Max count of blank section expected
    m_maxCount = 1;
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &PlayerJoseph::pieceIsAdversary;
    // Sequence searching function, sequence with 1 vacancy
    m_trackFunc = &PlayerJoseph::freeValidSequenceFound;// TODO, optimize when one end could be blocked
    // Looking for the type 1
    log("WinSeq2 t1 tracking starts...");
    if(lookForLinearSequence(pos)) return true;

    // Size of the sequence
    m_seqSize = 7;
    // Comparison method, getting a reference to the corresponding function
    m_targetFunc = &PlayerJoseph::pieceIsMine;
    // Sequence searching function, alterning chain
    m_trackFunc = &PlayerJoseph::alternateSeqFound;
    // Looking for the type 2
    log("WinSeq2 t2 tracking starts...");
    if(lookForLinearSequence(pos)) return true;
    return false;
}
// Looking for a deadly sequence of the 2nd order (type 1: _***__, _**_*_, _*_**_, __***_, type 2: *_*_*_*)
bool PlayerJoseph::lookForDeadlySequence2(Position& pos){
    // Size of the sequence
    m_seqSize = 6;
    // Max count of blank section expected
    m_maxCount = 1;
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &PlayerJoseph::pieceIsMine;
    // Sequence searching function, sequence with 1 vacancy
    m_trackFunc = &PlayerJoseph::freeValidSequenceFound;
    // Looking for the type 1
    log("DeadlySeq2 t1 tracking starts...");
    if(lookForLinearSequence(pos)) return true;

    // Size of the sequence
    m_seqSize = 7;
    // Comparison method, getting a reference to the corresponding function
    m_targetFunc = &PlayerJoseph::pieceIsAdversary;
    // Sequence searching function, alterning chain
    m_trackFunc = &PlayerJoseph::alternateSeqFound;
    // Looking for the type 2
    log("DeadlySeq2 t2 tracking starts...");
    if(lookForLinearSequence(pos)) return true;
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
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &PlayerJoseph::pieceIsAdversary;
    // Sequence searching function, sequence with 1 vacancy
    //m_trackFunc = &PlayerJoseph::freeValidSequenceFound;// does not fit perfectly (too dramatic)
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
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &PlayerJoseph::pieceIsMine;
    // Sequence searching function, sequence with 1 vacancy
    //m_trackFunc = &PlayerJoseph::freeValidSequenceFound;// does not fit perfectly (too dramatic)
    m_trackFunc = &PlayerJoseph::sequenceType3Found;
    log("DeadlySeq3 tracking starts...");
    return lookForLinearSequence(pos);
}
// Looking for a spot where a win sequence of the 3rd order can be placed
bool PlayerJoseph::lookForWinSequence4(Position& pos){
    // Size of the sequence
    m_seqSize = 5;
    // Max count of blank section expected
    m_maxCount = 1;
    // Reference line status code
    m_refStatus = line_winIn4;
    // Comparison method, getting a reference to the corresponding function
    m_inversFunc = &PlayerJoseph::pieceIsAdversary;
    // Sequence searching function, sequence with 1 vacancy
    m_trackFunc = &PlayerJoseph::sequenceType3Found;
    // Looking for a linear sequence
    log("WinSeq4 tracking starts...");
    return lookForLinearSequence(pos);
}
// Looking for a spot where a single stone is placed and giving him a friend
bool PlayerJoseph::lookForBuildPair(Position& pos){
    log("BuildPair tracking starts...");

    // Vector containing positions to analyze
    std::vector<PosData> posDataList;
    static bool isOk;
    static PosData s_posData;
    // Scanning the board
    for(std::size_t i{1}; i <= m_boardW; ++i){
        for(std::size_t j{1}; j <= m_boardH; ++j){
            m_in = m_board->getPiece(i, j);
            if(pieceIsMine(m_in)){
                // Scanning around
                for(int k{-1}; k <= 1; ++k){
                    for(int l{-1}; l <= 1; ++l){
                        if((k || l) && m_board->isEmptyAt(i - k, j - l, &isOk)){
                            if(isOk){
                                s_posData = getEmptyPosDataAt(i - k, j - l);
                                if(std::count(posDataList.begin(), posDataList.end(), s_posData) <= 0)
                                    posDataList.push_back(s_posData);
                            }
                        }
                    }
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
                if(m_enableLog) m_fileStream << "Sequence found: " << pos << '\n';
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
                if(m_enableLog) m_fileStream << "Sequence found: " << pos << '\n';
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
                if(m_enableLog) m_fileStream << "Sequence found: " << pos << '\n';
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
                if(m_enableLog) m_fileStream << "Sequence found: " << pos << '\n';
                return true;
            }
        }
    }

    log("Look for linear sequence: no sequence found.");
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
bool PlayerJoseph::sequenceType3Found(std::size_t inX, std::size_t inY, Position& pos){
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
        if(posData.m_codeX == m_refStatus
                || posData.m_codeY == m_refStatus
                || posData.m_codeD == m_refStatus
                || posData.m_codeA == m_refStatus){
            pos = Position(posData.m_x, posData.m_y);
            return true;
        }
    }

    // Returning the result
    return false;
}

// Looking around a given position
void PlayerJoseph::lookAround(PlayerJoseph::PosData& posData){
    // Filling with information
    if(m_dir != jodir_x){
        m_x2DIncFunc = &PlayerJoseph::incrementPlus;
        m_y2DIncFunc = &PlayerJoseph::incrementSame;
        m_x2DDecFunc = &PlayerJoseph::incrementMinus;
        m_y2DDecFunc = &PlayerJoseph::incrementSame;
        lookAlong(posData.m_x, posData.m_y, posData.m_codeX);
    }
    if(m_dir != jodir_y){
        m_x2DIncFunc = &PlayerJoseph::incrementSame;
        m_y2DIncFunc = &PlayerJoseph::incrementPlus;
        m_x2DDecFunc = &PlayerJoseph::incrementSame;
        m_y2DDecFunc = &PlayerJoseph::incrementMinus;
        lookAlong(posData.m_x, posData.m_y, posData.m_codeY);
    }
    if(m_dir != jodir_d){
        m_x2DIncFunc = &PlayerJoseph::incrementPlus;
        m_y2DIncFunc = &PlayerJoseph::incrementPlus;
        m_x2DDecFunc = &PlayerJoseph::incrementMinus;
        m_y2DDecFunc = &PlayerJoseph::incrementMinus;
        lookAlong(posData.m_x, posData.m_y, posData.m_codeD);
    }
    if(m_dir != jodir_a){
        m_x2DIncFunc = &PlayerJoseph::incrementPlus;
        m_y2DIncFunc = &PlayerJoseph::incrementMinus;
        m_x2DDecFunc = &PlayerJoseph::incrementMinus;
        m_y2DDecFunc = &PlayerJoseph::incrementPlus;
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
int PlayerJoseph::incrementSame(int inc) const{
    inc = 0;// To remove warning
    return inc;
}
int PlayerJoseph::incrementPlus(int inc) const{
    return inc;
}
int PlayerJoseph::incrementMinus(int inc) const{
    return -inc;
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

// Logging stuff
inline void PlayerJoseph::log(const std::string& str){
    if(m_enableLog) m_fileStream << str << '\n';
}

// Operator overload
bool operator==(const PlayerJoseph::PosData& a, const PlayerJoseph::PosData& b){
    return (a.m_x == b.m_x && a.m_y == b.m_y);
}

}// End namespace FIAR
