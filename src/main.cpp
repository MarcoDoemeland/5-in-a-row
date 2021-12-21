#include <cassert>	// for assertions
#include <cstdlib>	// for std::exit
#include <iostream>
#include <stdexcept>
#include <unistd.h>	// for getopt

#include "Game.h"
#include "Printer.h"
#include "Defines.h"



int main(int argc, char* argv[]){
    // Displaying the arguments
    //~ for(int i{ 0 }; i < argc; ++i) std::cout << argv[i] << '\n';



	std::string firstPlayerName {};
	std::string secondPlayerName {};
	int numGames { 1 };
	int interactiveMode { 0 };	// 0 : simulate to end; 1 : halt after each game; 2 : halt after each round


    //~ for (int i{ 0 }; i < argc; ++i) {

	//~ }
	int opt;
	while ((opt = getopt(argc, argv, "iIst:p:")) != -1) {
		//~ std::cout << opt << '\n';
		switch (opt) {
			case 'i':
				//~ std::cout << 'i' << '\n';
				interactiveMode = 1;	// halt after each game
				continue;

			case 'I':
				//~ std::cout << 'I' << '\n';
				interactiveMode = 2;	// halt after each round
				continue;

			case 's':
				//~ std::cout << 's' << '\n';
				//~ if (optarg) {
				numGames = 1;
				//~ }
				continue;

			case 't':
				//~ std::cout << 't' << '\n';
				//~ std::cout << (int) optarg << '\n';
				//~ std::cout << atoi(optarg) << '\n';
				//~ if (optarg) {
					//~ numGames = static_cast<int>(*optarg);
				numGames = atoi(optarg);
				//~ }
				continue;

			case 'p':
				//~ std::cout << 'p' << '\n';
				//~ std::cout << (int) optarg << '\n';
				//~ std::cout << atoi(optarg) << '\n';
				if (firstPlayerName.empty())  {
					//~ numGames = static_cast<int>(*optarg);
					firstPlayerName = static_cast<std::string>(optarg);
				}
				else if (secondPlayerName.empty())  {
					secondPlayerName = static_cast<std::string>(optarg);
				}
				else {
					std::cerr << "Error: Only 2 Players are allowed!" << '\n';
					return 1;
				}
				continue;

			default: /* '?' */
				fprintf(stderr, "Usage: %s [-i/I] [-s] [-t] numGames [-p] playerName\n",
						argv[0]);
				exit(EXIT_FAILURE);

			case -1:
				break;
		}
		break;
	}


	//~ std::cout << "Player 1: " << firstPlayerName << '\n';
	//~ std::cout << "Player 2: " << secondPlayerName << '\n';
	//~ std::cout << "Num Games: " << numGames << '\n';
	//~ std::cout << "Interactive: " << interactiveMode << '\n';

	//~ std::exit(1);

	try{
		// Trying to build the game. It might throw an exception.
		FIAR::Game game;
		// It worked, then starting
		game.start();
	}
	catch(const std::runtime_error& exception){
		std::cerr << "Error: " << exception.what() << '\n';
		return 1;
	}


	return 0;
}

