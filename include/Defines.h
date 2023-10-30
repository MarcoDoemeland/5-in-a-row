#ifndef DEFINES_H
#define DEFINES_H

#include <cstddef> // For std::size_t

namespace FIAR
{

// List of player IDs available (1 per usable class), to be used for the input control
enum PlayerId{
    player_human,
    player_random,
    player_marco,
    player_joseph,
    player_defensive,
    player_joseph2,
    // Max count
    player_max
};

enum Direction{
    dir_horizontal,
    dir_diagonal,
    dir_vertical,
    dir_antiDiagonal,
    // Count of available directions
    dir_maxCount
};

// States defining the content of a cell
enum class Piece{
    none,
    player1,
    player2,
    outside,
};

struct WinningSequence{
    std::size_t m_x;
    std::size_t m_y;
    int m_dir;
    Piece m_piece;
};

// avoid object-like macros????
//~ #define BOARD_SIZE_X 8
//~ #define BOARD_SIZE_Y 15
//~ #define DEFAULT_SYMBOL ' '

//constexpr int BOARD_SIZE_X { 8 };
//constexpr int BOARD_SIZE_Y { 15 };
//constexpr char DEFAULT_SYMBOL { ' ' };

extern const std::size_t g_boardSizeX;
extern const std::size_t g_boardSizeY;
extern const char g_defaultSymbol;
extern const char g_outsideSymbol;
extern const char g_player1Symbol;
extern const char g_player2Symbol;

}// End namespace FIAR

#endif // DEFINES_H
