// -----------------------------------------------------------------------------
// 5iar -- Game.h
// -----------------------------------------------------------------------------
//
// ...
//

#ifndef __FIAR_GAME_H__
#define __FIAR_GAME_H__


#include <vector>

#include "Defines.h"
#include "Board.h"
#include "Players/PlayerBase.h"
#include "Printer.h"


namespace FIAR{

class Game
{
public:
    // standard constructor
    Game();

    // constructor
    //~ Game( FIAR::Board board_,
    //~       FIAR::PlayerBase player_1_,
    //~       FIAR::PlayerBase player_2_,
    //~       FIAR::Printer printer_
    //~     );
    // Using pointers here, Game is not the owner of those objects.
    // Using objects called by value would create copies
    // No need to use scope resolution FIAR::, since we actually are in FIAR
    //Game(Board* board, PlayerBase* player1, PlayerBase* player2, Printer* printer);

    // Destructor
    ~Game();

    // Starting a gaming session
    void start();

    // A winner was found, returning a pointer
    PlayerBase* winnerFound() const;

			// constructor for square board
			//~ Board( unsigned int const size );

			// destructor
			//~ ~Printer( void );

			// ...
			//~ auto add_symbol(	int const pos_x,
								//~ int const pos_y,
								//~ char const symbol
							//~ ) -> bool;

			// ...
			//~ auto get_array( void ) -> std::vector<std::vector<char>>;

			// ...
			//~ auto find_winning_sequences( void ) -> std::vector< std::vector< int > >;


		private:

			// size of board
            // Using pointers here, Game is not the owner of those objects
            // Joseph: Shouldn't Game be the owner of both players and board?
            // It would make more sense to me because then you instanciate Game and Printer
            // in main, and then game is taking care of the whole game mechanic.
            // In the other hand I'm not sure about the printer class, I think it's better if
            // it stays external like you did and can be asked to print with the help of a
            // function calling the board by const ref. Probably more secure and flexible.
            // A Printer instance could also be built only when needed. Or even more brutal,
            // kick the printer away and write the corresponding function in a << overload
            // taking const Board& as argument. Oh boy, so many options!!
            Board* m_boardPtr{ nullptr };
            PlayerBase* m_player1Ptr{ nullptr };
            PlayerBase* m_player2Ptr{ nullptr };
            Printer* m_printerPtr{ nullptr };
			//~ unsigned int size_y;
			//~ char default_symbol;

			// actual board representation
			//~ std::vector<std::vector<char>> array;
            //~ std::vector<char> array;
protected:
    // Defining the players
    void definePlayer1();
    void definePlayer2();
    void definePlayer(const std::string& text, PlayerBase*& player);

    // Telling which player should start the game. To be called only at start.
    void defineCurrentPlayer();
    // Swapping the current player. To be called in the end of each round.
    void swapCurrentPlayer();

private:
    // Test members for the alternative strategy
    // Board where the game is going to be played
    Board m_board;
    // Using pointers for the players, so we avoid slicing
    PlayerBase* m_player1{ nullptr };
    PlayerBase* m_player2{ nullptr };
    // Player currently playing
    PlayerBase* m_currentPlayer{ nullptr };
};

}// End namespace FIAR

#endif
