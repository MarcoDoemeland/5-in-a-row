#include <cassert>	// for assertions
#include <cstdlib>	// for std::exit
#include <iostream>
#include <exception>


#include "Game.h"
#include "Printer.h"
#include "Defines.h"

int main(int argc, char* argv[]){
    // Displaying the arguments
    for(int i{ 0 }; i < argc; ++i) std::cout << argv[i] << '\n';

    try{
        // Trying to build the game. It might throw an exception.
        FIAR::Game game;
        // It worked, then starting
        game.start();
    }
    catch(const std::exception& exception){
        std::cerr << "Error: " << exception.what() << '\n';
        return 1;
    }

	return 0;
}
/*int mainDump(){
    // call a function in another file
    //~ myPrintHelloMake();

    //~ std::string a {"huhu"};

    //~ std::cout << typeid("huhuaaasdasdasd asdasd").name() << '\n';

    //~ int a { 3 << 1 };
    //~ std::cout << a << '\n';
    //~ std::cout << testy(2) << '\n';
    //~ std::cout << blubb << '\n';
    //~ return 0;

    //~ std::cerr << "test" << '\n';
    //~ std::cerr << "blargh";
    //~ std::cout << "hi" << '\n';
    //~ std::exit(1);
    //~ assert(-1 > 0.0);
    //~ std::cout << "huhu" << '\n';
    //~ return 0;
    //~ std::abort();

    //~ void a;

    FIAR::Printer printer;
    FIAR::Board board ( FIAR::g_boardSizeX, FIAR::g_boardSizeY, FIAR::g_defaultSymbol );
    //FIAR::PlayerHuman player_1{ &board, "Guy XY" };
    //FIAR::PlayerRandom player_2{ &board };
    //FIAR::Game game ( &board, &player_1, &player_2, &printer );

    //~ std::cout << typeid(player_1).name() << '\n';
    //~ std::cout << ( 7 & 1) << '\n';
    //~ return 0;

    //~ board.array[0][0] = 'x';
    //~ board.array[4][7] = 'x';
    //~ board.array[1][1] = 'x';

    //~ std::vector<std::vector<int>> a;

    //~ std::vector<int> b {1,2,3};
    //~ a.push_back( b );
    //~ a.push_back( b );

    //~ a.insert( a.end(), b.begin(), b.end() );

    //~ printer.print_array( a) );
    //~ for (auto i : a)
    //~ {
        //~ for (auto j : i)
        //~ {
            //~ std::cout << j << " ";
        //~ }
        //~ std::cout << std::endl;

    //~ }
    //~ return 0;

    //~ if (a.empty())
    //~ {
        //~ std::cout << "yooo" << std::endl;
    //~ }
    //~ return 0;

    //~ field.resize(R);
    //~ mat.resize(R, std::vector<int>(C));
    //~ for (int i = 0 ; i < R ; ++i) {
        //~ field[i].resize(C);
    //~ }

    //~ std::cout << boarda.size() << std::endl;
    //~ std::cout << boarda[0].size() << std::endl;
    //~ std::cout << "'" << boarda[0][0] << "'" << std::endl;

    //~ std::cout << "huhu" << std::endl;
    //~ std::cout << board.size_x << std::endl;
    //~ std::cout << board.size_y << std::endl;
    //~ std::cout << board.array[0] << std::endl;
    //~ std::cout << board.array[0][0] << std::endl;

}*/
