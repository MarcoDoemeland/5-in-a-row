#include <iostream>

#include "Players/PlayerBase.h"
//~ #include "Board.h"

namespace FIAR
{
// Constructor
PlayerBase::PlayerBase(const Board* board, const std::string& playerName, Piece piece)
    : m_board{ board }
    , m_name{ playerName }
    , m_piece{ piece }{
    std::cout << "Constructing player " << *this << '\n';
}

// Destructor
PlayerBase::~PlayerBase(){
    std::cout << "Destroying player " << *this << '\n';
}

// Accessor to the name and count of wins
std::string PlayerBase::name() const{
    return m_name;
}
int PlayerBase::winCount() const{
    return m_winCount;
}
// Accessor to the symbol
//char PlayerBase::symbol() const{
//    return m_symbol;
//}
// Accessor to the piece
Piece PlayerBase::piece() const{
    return m_piece;
}

// Incrementing the count of wins
void PlayerBase::incrementWinCount(){
    ++m_winCount;
}

// Printing the player
std::ostream& operator<<(std::ostream& stream, const PlayerBase& player){
    stream << player.name() << " (symbol " << player.piece() << ", " << player.winCount() << " wins)";
    return stream;
}

}// End namespace FIAR
