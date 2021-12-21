#ifndef PLAYERJOSEPH_H
#define PLAYERJOSEPH_H

// Custom classes and defines
#include "Defines.h"
#include "Players/PlayerBase.h"
#include "Board.h"

#include <fstream>

namespace FIAR{

// Dumm, duemmer, am duemmsten
class PlayerJoseph final : public PlayerBase{
public:
    // Constructor
    PlayerJoseph(const Board* board, Piece piece);
    // Destructor
    virtual ~PlayerJoseph();

private:
    // Directions
    enum JoDir{
        jodir_x,// Along x
        jodir_d,// Diagonal
        jodir_y,// Along y
        jodir_a,// Antidiagonal
        jodir_count,
        jodir_nodir// Placed after count, since it is not a direction
    };

    // Status of a line: the order here is very important
    enum LineStatus{
        line_winIn1,
        line_loseIn1,
        line_winIn2,
        line_loseIn2,
        line_winIn3,
        line_loseIn3,
        line_winIn4,
        line_loseIn4,
        line_empty,
        line_dead,
        line_unknown,
        // Count
        line_maxCount
    };

    // Other definition of a line, more statistic
    struct LineData{
        bool m_begBlocked{ false };
        bool m_endBlocked{ false };
        bool m_innBlocked{ false };
        int m_countTar{ 0 };
        int m_countInv{ 0 };
    };

    // Characteristics of a position
    struct PosData{
        std::size_t m_x;
        std::size_t m_y;
        LineStatus m_codeX;
        LineStatus m_codeY;
        LineStatus m_codeD;
        LineStatus m_codeA;
    };

public:
    // Operator overload
    friend bool operator==(const PosData& a, const PosData& b);

private:
    // No copy
    PlayerJoseph(const PlayerJoseph& player) = delete;
    PlayerJoseph& operator=(const PlayerJoseph& player) = delete;

    // Function to use to make an action
    Position doAction() override;

    // Looking for a win sequence of the 1st order
    // (****_, ***_*, **_**, *_***, _****)
    bool lookForWinSequence1(Position& pos);
    // Looking for a deadly sequence of the 1st order
    // (****_, ***_*, **_**, *_***, _****)
    bool lookForDeadlySequence1(Position& pos);

    // Looking for a win sequence of the 2nd order
    //  type 1: _***__, _**_*_, _*_**_, __***_,
    //  type 2: *_*_*_*,
    //  type 3: double type 1 blocked at one side
    bool lookForWinSequence2(Position& pos);
    // Looking for a win sequence of the 2nd order
    //  type 1: _***__, _**_*_, _*_**_, __***_,
    //  type 2: *_*_*_*,
    //  type 3: double type 1 blocked at one side
    bool lookForDeadlySequence2(Position& pos);

    // Looking for a win sequence of the 3rd order
    // (free _**, *_* or **_ combined with free _**, *_* or **_)
    bool lookForWinSequence3(Position& pos);
    // Looking for a deadly sequence of the 3rd order
    // (free _**, *_* or **_ combined with free _**, *_* or **_)
    bool lookForDeadlySequence3(Position& pos);

    // Looking for a spot where a win sequence of the 3rd order can be placed
    bool lookForWinSequence4(Position& pos);
    // Looking for a deadly sequence of the 4th order (v-shape)
    bool lookForDeadlySequence4(Position& pos);
    // Looking for a spot where a win sequence of the 4th (v-shape) order can be placed
    bool lookForWinSequence5(Position& pos);
    // Looking for a spot where a single stone is placed and giving him a friend
    bool lookForBuildPair(Position& pos);

    // S-form TODO!!!
    // _x_x
    // x_x_

    // Looking for a linear sequence
    bool lookForLinearSequence(Position& pos);
    // Scanning the board
    bool lookFor2DSequence(Position& pos);
    // Reading a sequence (limits must not be empty)
    bool validSequenceFound(std::size_t inX, std::size_t inY, Position& pos);
    // Reading a sequence (limits must be free)
    bool freeValidSequenceFound(std::size_t inX, std::size_t inY, Position& pos);
    // Reading a sequence (alternating ref and void)
    bool alternateSeqFound(std::size_t inX, std::size_t inY, Position& pos);
    // Reading a sequence (limits must be free, evaluation must return symetrical data)
    bool sequenceType3Found_old(std::size_t inX, std::size_t inY, Position& pos);
    // Reading a sequence (limits must be free or occupied by the target, evaluation shall be <= reference)4
    bool sequenceType3Found(std::size_t inX, std::size_t inY, Position& pos);
    // Reading a sequence (limits must be free, evaluation returns single point)
    bool sequenceType4Found(std::size_t inX, std::size_t inY, Position& pos);
    // Reading a sequence (a V-shape with free spots around)
    bool sequenceTypeVFound(std::size_t inX, std::size_t inY, Position& pos);
    // Reading a sequence (a L-shape with free spots around)
    bool sequenceTypeLFound(std::size_t inX, std::size_t inY, Position& pos);

    // Looking around a given position
    void lookAround(PlayerJoseph::PosData& posData);
    // Looking along a given line (up to 5 cells away in both directions)
    void lookAlong(std::size_t x, std::size_t y, LineStatus& code);
    // Looking along a given line centered on the input point (variable size)
    void lookAlong(std::size_t x, std::size_t y, std::size_t size, LineData& lineData);

    // Evaluating a list of position data
    Position evaluatePositions(const std::vector<PosData>& list);

    // Looking for sequences
    bool findSequence(std::size_t size) const;

    // Telling whether a piece is adversary
    inline bool pieceIsAdversary(Piece piece) const;
    // Telling whether a piece is adversary or none
    inline bool pieceIsHostile(Piece piece) const;
    // Telling whether a there is a piece
    inline bool pieceIsNone(Piece piece) const;
    // Telling whether the piece is mine
    inline bool pieceIsMine(Piece piece) const;

    // Increment functions
    int incrementSame(int inc) const;
    int incrementPlus(int inc) const;
    int incrementMinus(int inc) const;

    // Setting the increment / decrement functions according to the direction
    void set2DIncrementHor();
    void set2DIncrementVer();
    void set2DIncrementDia();
    void set2DIncrementAdia();

    // Getting an empty posData object with the given position
    inline PosData getEmptyPosDataAt(const Position& pos) const;
    inline PosData getEmptyPosDataAt(std::size_t x, std::size_t y) const;

    // Resetting a LineData object
    inline void resetLineData(LineData& lineData) const;
    // Telling the line is dead
    inline bool lineIsDead(const LineData& lineData) const;

    // Logging stuff
    inline void log(const std::string& str);
    // Logging text + template
    template<typename T>
    inline void log(const std::string& str, const T& var){
        if(m_enableLog) m_fileStream << str << ' ' << var << '\n';
    }

    // Rounds counter
    int m_rndCnt{ 0 };
    // Random counter
    int m_ranCnt{ 0 };
    // Error counter
    int m_errCnt{ 0 };

    // Dimensions of the board
    std::size_t m_boardW{ 0 };
    std::size_t m_boardH{ 0 };

    // Char used to read (more performant)
    Piece m_in;
    // Current direction inspected during the linear sequence search
    JoDir m_dir;
    // Size of the sequence
    std::size_t m_seqSize;
    // Count of default chars allowed in the sequence
    std::size_t m_maxCount;

    // Reference line status code
    LineStatus m_refStatus;

    // Comparison function
    bool (PlayerJoseph::*m_inversFunc)(Piece) const{ nullptr };
    bool (PlayerJoseph::*m_targetFunc)(Piece) const{ nullptr };
    // LINEAR SEQUENCE SEARCH
    // Increment functions
    int (PlayerJoseph::*m_xIncFunc)(int) const{ nullptr };
    int (PlayerJoseph::*m_yIncFunc)(int) const{ nullptr };
    // Decrement functions
    int (PlayerJoseph::*m_xDecFunc)(int) const{ nullptr };
    int (PlayerJoseph::*m_yDecFunc)(int) const{ nullptr };
    // 2D SEQUENCE SEARCH
    // Increment functions
    int (PlayerJoseph::*m_x2DIncFunc)(int) const{ nullptr };
    int (PlayerJoseph::*m_y2DIncFunc)(int) const{ nullptr };
    // Decrement functions
    int (PlayerJoseph::*m_x2DDecFunc)(int) const{ nullptr };
    int (PlayerJoseph::*m_y2DDecFunc)(int) const{ nullptr };
    // Sequence searching function
    bool (PlayerJoseph::*m_trackFunc)(std::size_t inX, std::size_t inY, Position& pos){ nullptr };

    // Logging
    bool m_enableLog{true};
    //bool m_enableLog{false};
    std::ofstream m_fileStream;
};

}// End namespace FIAR

#endif // PLAYERJOSEPH_H
