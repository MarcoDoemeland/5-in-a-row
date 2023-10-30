#include "Game.h"

#include <iostream>
#include <stdexcept>// For std::runtime_error
#include <ctime>// For std::time, to used for seeding the random number generator

#include "Utilities.h"

#include "Position.h"

#include "Players/PlayerDefensive.h"
#include "Players/PlayerHuman.h"
#include "Players/PlayerJoseph.h"
#include "Players/PlayerMarco.h"
#include "Players/PlayerRandom.h"

namespace FIAR
{

// Constructor
Game::Game()
    : m_board(g_boardSizeX, g_boardSizeY){
    definePlayer1();
    definePlayer2();
    try{
        if(!m_board.isValid()) throw std::runtime_error("Board invalid.");
        if(!m_player1) throw std::runtime_error("First player define failed.");
        if(!m_player2) throw std::runtime_error("Second player define failed.");
    }
    catch(...){
        if(m_player1) delete m_player1;
        if(m_player2) delete m_player2;
        throw;
    }
    // Initializing the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::rand();

    std::cout << "Board and players ready.\n\n";
}

// Destructor
Game::~Game(){
    delete m_player1;
    delete m_player2;
}

// Starting a gaming session
void Game::start(){
    // Defining the first player
    defineCurrentPlayer();

    // As long as no winner was found and there are slots left, playing
    std::size_t cycleCount{0};
    while(!winnerFound() && cycleCount < m_board.area()){
        // Telling who is playing
        std::cout << *m_currentPlayer << " is playing: ";
        // Getting the position the player wants to use and placing on the board
        m_board.addPiece(m_currentPlayer->doAction(), m_currentPlayer->piece());
        // Printing the board
        std::cout << m_board << '\n';
        // Swapping the current player
        swapCurrentPlayer();
        // Incrementing the cycle count
        ++cycleCount;
    }
    // No winner, pretty sad
    if(cycleCount >= m_board.area())  std::cout << "Nobody won.\n";

    // Printing the board
    std::cout << m_board << '\n';
}
// A winner was found, returning a pointer
PlayerBase* Game::winnerFound() const{
    // Looking for winning combination and returning the pointer to the player it belongs to
    // If no combination found returning nullptr
    WinningSequence sequence;
    if(m_board.getWinningSequence(sequence)){
        if(sequence.m_piece == Piece::player1){
            m_player1->incrementWinCount();
            return m_player1;
        }
        else if(sequence.m_piece == Piece::player2){
            m_player2->incrementWinCount();
            return m_player2;
        }
    }
    return nullptr;
}

// Defining the players
void Game::definePlayer1(){
    definePlayer("first", m_player1, Piece::player1);
}
void Game::definePlayer2(){
    definePlayer("second", m_player2, Piece::player2);
}
void Game::definePlayer(const std::string& text, PlayerBase*& player, Piece piece){
    // Building a list containing all usable players (only by the first call)
    static const std::vector<int> s_playerIds{
        player_human,
        player_random,
        player_marco,
        player_joseph,
        player_defensive,
        player_joseph2 };

    // Building the string to be displayed
    std::string message = "Choose your " + text + " player (";
    for(std::size_t i{ 0u }; i < s_playerIds.size(); ++i){
        message.append(std::to_string(s_playerIds[i]));
        if(i < s_playerIds.size() - 1) message.append(", ");
    }
    message.append("): ");

    // Firstly, deleting the player
    delete player;
    // Then getting the type of player the user wants to play with
    int playerId{ getValidInputFromUser<int>( message, s_playerIds ) };
    // ... And building the player
    switch(playerId){
    case player_human:
        player = new PlayerHuman(&m_board, getInputFromUser<std::string>("Enter the player's name: "), piece);
        break;
    case player_random:
        player = new PlayerRandom(&m_board, piece);
        break;
    case player_marco:
        player = new PlayerMarco(&m_board, piece);
        break;
    case player_joseph:
        player = new PlayerJosephOG(&m_board, piece);
        break;
    case player_defensive:
        player = new PlayerDefensive(&m_board, piece);
        break;
    case player_joseph2:
        player = new PlayerJosephPlus(&m_board, piece);
        break;
    // Stupid, but allows getting the warning away.
    case player_max:
        player = nullptr;
        break;
    default:
        player = nullptr;
        break;
    }
}

// Telling which player should start the game. To be called only at start.
void Game::defineCurrentPlayer(){
    // Defining which player should play first
    int firstPlayer{ getRandomInt(1, 2) };
    // Distruting the roles
    if(firstPlayer == 1) m_currentPlayer = m_player1;
    else m_currentPlayer = m_player2;
    // Displaying a short message
    if(m_currentPlayer) std::cout << *m_currentPlayer << " is starting the game.\n";
}
// Swapping the current player. To be called in the end of each round.
void Game::swapCurrentPlayer(){
    if(m_currentPlayer == m_player1) m_currentPlayer = m_player2;
    else m_currentPlayer = m_player1;
}

} // End namespace FIAR
