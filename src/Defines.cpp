#include "Defines.h"

namespace FIAR
{

// avoid object-like macros????
//~ #define BOARD_SIZE_X 8
//~ #define BOARD_SIZE_Y 15
//~ #define DEFAULT_SYMBOL ' '
//~ extern const int BOARD_SIZE_X{ 8 };
//~ extern const int BOARD_SIZE_Y{ 15 };
//~ extern const char DEFAULT_SYMBOL{ ' ' };

// Using g_ prefix for global variables
// Using std::size_t for a better compatibility with std::vector indexes and sizes
extern const std::size_t g_boardSizeX{ 22u };
extern const std::size_t g_boardSizeY{ 22u };
extern const char g_defaultSymbol{ ' ' };
extern const char g_outsideSymbol{ '#' };
extern const char g_player1Symbol{ 'x' };
extern const char g_player2Symbol{ 'o' };

}// End namespace FIAR
