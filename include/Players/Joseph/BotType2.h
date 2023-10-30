#ifndef __FIAR_PLAYERS_JOSEPH_BOTTYPE2_H__
#define __FIAR_PLAYERS_JOSEPH_BOTTYPE2_H__

#include <Players/Joseph/BaseBot.h>

namespace FIAR
{

namespace joseph
{

class BotType2 final : public BaseBot
{
public:
    // Constructor
    BotType2(const Board* board, Piece piece);
    // Destructor
    virtual ~BotType2();

private:
    // No copy
    BotType2(const BotType2& player) = delete;
    BotType2& operator=(const BotType2& player) = delete;

    // Playing an algorithm
    void playAlgo(Position& pos) final;

    // Evaluating the board, filling the evaluation vector with data
    void evaluateBoard(Position& pos);
    // Scanning the board calling the appropriate scanning function
    void scanBoard(Position& pos, void (BotType2::*scanFunc)(std::size_t inX, std::size_t inY, Position& pos));
    // Scanning the board in a snail pattern, returning the first free spot
    void scanStar(Position& pos);
    // Evaluating win / lose chances at the given position
    void evaluateWin(std::size_t inX, std::size_t inY, Position& pos);
    void evaluateLose(std::size_t inX, std::size_t inY, Position& pos);
    // Evaluating a line
    double evaluateLine(std::size_t inX, std::size_t inY);

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

    // Tooling for picturing the evaluation in an image
    bool m_enableImg{ false };
    const size_t m_imgZoom { 25u };
    Image m_imgEval;
    Image m_imgChoice;
    void makeEvalImgs(const Position& pos);
    void setPixel(Image& image, size_t x, size_t y, const ColorHSV& c);
    void setPixelFixVal(Image& image, size_t x, size_t y, uint16_t hue, uint8_t saturation);
};

} // End namespace joseph

} // End namespace FIAR

#endif // __FIAR_PLAYERS_JOSEPH_BOTTYPE2_H__
