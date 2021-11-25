#include <iostream>

#include "PlayerBase.h"
//~ #include "Board.h"

namespace FIAR
{
// Constructor
PlayerBase::PlayerBase(Board* board, const std::string& playerName)
    : m_board{ board }
    , m_name{ playerName }{
    std::cout << "Constructing player " << *this << '\n';
}

// constructor - to be removed? TODO
//~ PlayerBase::PlayerBase( int i ){
//~     std::cout << i << '\n';
//~ }

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

// Incrementing the count of wins
void PlayerBase::incrementWinCount(){
    ++m_winCount;
}

// Printing the player
std::ostream& operator<<(std::ostream& stream, const PlayerBase& player){
    stream << player.name() << " (" << player.winCount() << " wins)";
    return stream;
}

}// End namespace FIAR
