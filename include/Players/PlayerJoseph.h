#ifndef PLAYERJOSEPH_H
#define PLAYERJOSEPH_H

// Custom classes and defines
#include "Defines.h"
#include "Players/PlayerBase.h"
#include "Board.h"

#include <fstream>

namespace FIAR{

class JLImage{
public:
    // Color
    struct ColorHSV{
        uint16_t hue      { 0u };
        uint8_t saturation{ 0u };
        uint8_t value     { 0u };
    };
    struct ColorRGB{
        uint8_t red  { 0u };
        uint8_t green{ 0u };
        uint8_t blue { 0u };

        static ColorRGB fromHSV(const ColorHSV& in);
    };

    // Constructors
    JLImage(std::size_t w, std::size_t h);
    JLImage(const JLImage& ref) = delete;
    JLImage() = delete;

    // Destructor
    ~JLImage();

    // Resetting the image
    void reset(uint8_t r, uint8_t g, uint8_t b);
    void reset(const ColorRGB& color);
    void reset();
    // Setting a pixel value
    void setPixel(std::size_t x, std::size_t y, uint8_t r, uint8_t g, uint8_t b);
    void setPixel(std::size_t x, std::size_t y, const ColorRGB& color);
    // Returning a pixel value
    ColorRGB getPixel(std::size_t x, std::size_t y);
    // Saving the image
    void save(const std::string& name);
    // << operator overload
    friend std::ostream& operator<<(std::ostream& stream, const JLImage& image);

private:
    // Dimensions
    std::size_t m_w{ 0 };
    std::size_t m_h{ 0 };
    // Size of the header
    static const std::int32_t s_gHeaderBytes{ 14 };
    // Size of the image header
    static const std::int32_t s_iHeaderBytes{ 40 };
    // Size of a row
    std::int32_t m_rowBytes{ 0 };
    // Size of the data
    std::int32_t m_dataBytes{ 0 };
    // Size of the file
    std::int32_t m_fileBytes{ 0 };

    // Actual image
    std::int8_t* m_image{ nullptr };

    // General header:
    // 0x0000	2 octets	le nombre magique correspondant à l'utilisation du fichier BMP
    std::int16_t* m_magicNumber{ nullptr };
    // BM - Windows 3.1x, 95, NT, etc.
    // BA - OS/2 Bitmap Array
    // CI - OS/2 Icône Couleur (Color Icon)
    // CP - OS/2 Pointeur Couleur (Color Pointer)
    // IC - OS/2 Icône (Icon)
    // PT - OS/2 Pointeur (Pointer)
    // 0x0002	4 octets	la taille du fichier BMP en octets
    std::int32_t* m_fileSize{ nullptr };
    // 0x0006	2 octets	réservé pour l'identifiant de l'application qui a créé le fichier
    std::int16_t* m_id1{ nullptr };
    // 0x0008	2 octets	réservé pour l'identifiant de l'application qui a créé le fichier
    std::int16_t* m_id2{ nullptr };
    // 0x000A	4 octets	l'offset (l'adresse de départ) du contenu du BMP
    std::int32_t* m_imageOffset{ nullptr };

    // Image header:
    std::int32_t* m_iHeaderSize{ nullptr };
    std::int32_t* m_imageW{ nullptr };
    std::int32_t* m_imageH{ nullptr };
    std::int16_t* m_colorPlanes{ nullptr };
    std::int16_t* m_bitsPerPx{ nullptr };
    std::int32_t* m_comprMethod{ nullptr };
    std::int32_t* m_imageSize{ nullptr };
    std::int32_t* m_horiReso{ nullptr };
    std::int32_t* m_vertReso{ nullptr };
    std::int32_t* m_colorCnt{ nullptr };
    std::int32_t* m_impColorCnt{ nullptr };

    // Data
    std::uint8_t* m_data{ nullptr };

    // Pointer for reading / writing pixels
    std::uint8_t* m_pxRW{ nullptr };
};

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

    // Playing an algorithm
    void playNewAlgo(Position& pos);
    void playOldAlgo(Position& pos);

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

    // Evaluating the board, filling the evaluation vector with data
    void evaluateBoard(Position& pos);
    // Scanning the board calling the appropriate scanning function
    void scanBoard(Position& pos, void (PlayerJoseph::*scanFunc)(std::size_t inX, std::size_t inY, Position& pos));
    // Scanning the board in a snail pattern, returning the first free spot
    void scanStar(Position& pos);
    // Evaluating win / lose chances at the given position
    void evaluateWin(std::size_t inX, std::size_t inY, Position& pos);
    void evaluateLose(std::size_t inX, std::size_t inY, Position& pos);
    // Evaluating a line
    double evaluateLine(std::size_t inX, std::size_t inY);

    // Telling whether a piece is adversary
    inline bool pieceIsAdversary(Piece piece) const;
    // Telling whether a piece is adversary or none
    inline bool pieceIsHostile(Piece piece) const;
    // Telling whether a there is a piece
    inline bool pieceIsNone(Piece piece) const;
    // Telling whether the piece is mine
    inline bool pieceIsMine(Piece piece) const;

    // Increment functions
    int incrementSame(int /*inc*/) const;
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

    // Getting x and y out of an index
    inline size_t xFromIndex(size_t index) { return index % m_boardW + 1; }
    inline size_t yFromIndex(size_t index) { return index / m_boardW + 1; }
    // Getting an index out of x and y
    inline size_t indexFromPos(size_t x, size_t y) { return x - 1 + (y - 1) * m_boardW; }

    // Resetting a LineData object
    inline void resetLineData(LineData& lineData) const;
    // Telling the line is dead
    inline bool lineIsDead(const LineData& lineData) const;

    // Logging stuff
    inline void log(const std::string& str){
        if(!m_enableLog)
            return;
        m_fileStream << str << '\n';
        std::cout << str << std::endl;
    }
    // Logging text + template
    template<typename T>
    inline void log(const std::string& str, const T& var){
        if(!m_enableLog)
            return;
        m_fileStream << str << ' ' << var << '\n';
        std::cout << str << ' ' << var << std::endl;
    }

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

    // Size of a line required to win
    const size_t m_winLineSize{ 5 };
    // Base of the evaluation code (count of possible directions)
    const size_t m_evalBase{ 4 };
    // Size of a complete line (a sequence can start or end at the point we're looking at)
    const size_t m_evalSize{ 2 * m_winLineSize - 1 };

    // Evaluation value
    double m_evalRslt{ 0. };
    double m_evalMax { 4. };
    std::vector<double> m_evalsW;
    std::vector<double> m_evalsL;

    const size_t m_imgZoom { 25u };
    JLImage m_imgEval;
    JLImage m_imgChoice;
    void makeEvalImgs(const Position& pos);
    void setPixel(JLImage& image, std::size_t x, std::size_t y, const JLImage::ColorHSV& c);
    void setPixelFixVal(JLImage& image, std::size_t x, std::size_t y, uint16_t hue, uint8_t saturation);

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
    bool m_enableImg{false};
    bool m_enableLog{false};
    std::ofstream m_fileStream;
};

}// End namespace FIAR

#endif // PLAYERJOSEPH_H
