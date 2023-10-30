#include <Players/Joseph/BotType2.h>

#include "Utilities.h"
#include <algorithm>
#include <math.h>

namespace FIAR
{

namespace joseph
{

BotType2::BotType2(const Board* board, Piece piece)
    : BaseBot( board, piece, "JosephBotPlus" )
    , m_evalsW( m_fields )
    , m_evalsL( m_fields )
    , m_imgEval{ m_boardW * m_imgZoom, m_boardH * m_imgZoom }
    , m_imgChoice{ m_boardW * m_imgZoom, m_boardH * m_imgZoom }
{
}

BotType2::~BotType2()
{
}

void BotType2::playAlgo(Position& pos){
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

// Evaluating the board, filling the evaluation vector with data
void BotType2::evaluateBoard(Position& pos){
    // Resetting the evaluation value
    m_evalRslt = 0.;
    std::fill(m_evalsW.begin(), m_evalsW.end(), 0.);
    std::fill(m_evalsL.begin(), m_evalsL.end(), 0.);

    // Scanning the board: win
    m_targetFunc = &BotType2::pieceIsMine;
    scanBoard(pos, &BotType2::evaluateWin);

    // Scanning the board: lose
    m_targetFunc = &BotType2::pieceIsAdversary;
    scanBoard(pos, &BotType2::evaluateLose);

    // Checking whether the eval is still 0., case yes snail algo
    if(m_evalRslt == 0.) scanStar(pos);
}
// Scanning the board calling the appropriate tracking function
void BotType2::scanBoard(Position& pos, void (BotType2::*scanFunc)(std::size_t inX, std::size_t inY, Position& pos)){
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
void BotType2::scanStar(Position& pos){
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
void BotType2::evaluateWin(std::size_t inX, std::size_t inY, Position& pos){
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
void BotType2::evaluateLose(std::size_t inX, std::size_t inY, Position& pos){
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
double BotType2::evaluateLine(std::size_t x, std::size_t y){
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

void BotType2::makeEvalImgs(const Position& pos)
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

inline void BotType2::setPixel(Image& image, std::size_t x, std::size_t y, const ColorHSV& c){
    for (std::size_t imgX = x * m_imgZoom; imgX < m_imgZoom * (x + 1); ++imgX)
        for (std::size_t imgY = y * m_imgZoom; imgY < m_imgZoom * (y + 1); ++imgY)
            image.setPixel(imgX, imgY, ColorRGB::fromHSV(c));
}

inline void BotType2::setPixelFixVal(Image& image, std::size_t x, std::size_t y, uint16_t hue, uint8_t saturation)
{
    setPixel(image, x, y, ColorHSV{hue, saturation, 255u});
}

} // End namespace joseph

} // End namespace FIAR
