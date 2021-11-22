#include "Board.h"

// HALLO, ICH BIN EIN KOMMENTAR!!

namespace FIAR
{
Board::Board(std::size_t sizeX, std::size_t sizeY, char defaultSymbol)
    : m_sizeX{ sizeX }
    , m_sizeY{ sizeY }
    , m_defaultSymbol{ defaultSymbol }{
    //~ array = std::vector<char>(size_y, 'a');
    //array = std::vector<std::vector<char>>(size_x, std::vector<char>(size_y, default_symbol));

    // Building the board
    buildBoard();
}

// TODO: using indexes starting with 1 could be pretty "dangerous" imo
bool Board::add_symbol(std::size_t posX, std::size_t posY, char symbol){
    // Cases where the symbol can not be added
    if (posX < 1 || posX > m_sizeX) return false;
    if (posY < 1 || posY > m_sizeY) return false;
    if (m_array[posX - 1][posY - 1] != m_defaultSymbol) return false;

    // Adding the symbol
    m_array[posX - 1][posY - 1] = symbol;
    return true;
}

// TODO: Probably pretty expensive: not quite my tempo
std::vector<std::vector<char>> Board::get_array(){
    return m_array;
}

//std::vector<std::vector<int>> Board::find_winning_sequences( void ){
std::vector<WinningSequence> Board::find_winning_sequences( void ){
    std::vector<WinningSequence> found_sequences {};
    //~ std::vector< int > found_solution {};

    // test for horizontal sequences
    for(std::size_t i{0}; i < m_sizeX; ++i){
        for(std::size_t j {0}; j < m_sizeY - 4; ++j){
            if(m_array[i][j] != m_defaultSymbol
                    && m_array[i][j] == m_array[i][j+1]
                    && m_array[i][j] == m_array[i][j+2]
                    && m_array[i][j] == m_array[i][j+3]
                    && m_array[i][j] == m_array[i][j+4]){
                found_sequences.push_back( { i+1, j+1 , 0 } );
            }
        }
    }

    // test for diagonal sequences
    for(std::size_t i{0}; i < m_sizeX - 4; ++i){
        for(std::size_t j{0}; j < m_sizeY - 4; ++j){
            if (m_array[i][j] != m_defaultSymbol
                    && m_array[i][j] == m_array[i+1][j+1]
                    && m_array[i][j] == m_array[i+2][j+2]
                    && m_array[i][j] == m_array[i+3][j+3]
                    && m_array[i][j] == m_array[i+4][j+4]){
                found_sequences.push_back( { i+1, j+1, 1} );
            }
        }
    }

    // test for vertical sequences
    for(std::size_t i{0}; i < m_sizeX - 4; ++i){
        for(std::size_t j{0}; j < m_sizeY; ++j){
            if(m_array[i][j] != m_defaultSymbol
                    && m_array[i][j] == m_array[i+1][j]
                    && m_array[i][j] == m_array[i+2][j]
                    && m_array[i][j] == m_array[i+3][j]
                    && m_array[i][j] == m_array[i+4][j]){
                found_sequences.push_back( {i+1, j+1, 2} );
            }
        }
    }

    // test for anti diagonal sequences
    for(std::size_t i{0}; i < m_sizeX - 4; ++i){
        for(std::size_t j{4}; j < m_sizeY; ++j){
            if(m_array[i][j] != m_defaultSymbol
                    && m_array[i][j] == m_array[i+1][j-1]
                    && m_array[i][j] == m_array[i+2][j-2]
                    && m_array[i][j] == m_array[i+3][j-3]
                    && m_array[i][j] == m_array[i+4][j-4]){
                found_sequences.push_back( {i+1,j+1,3} );
            }
        }
    }

    return found_sequences;
}

// Building the board, taking care of the dimensions
void Board::buildBoard(){
    // Clearing the previous board
    m_array.clear();

    // Control part of the function
    if(m_sizeX == 0 && m_sizeY == 0){
        return;
    }
    else if(m_sizeX == 0){
        m_sizeY = 0;
        return;
    }
    else if(m_sizeY == 0){
        m_sizeX = 0;
        return;
    }

    // Building a new board
    // TODO: check whether this step can fail with std::vector
    // Eventually adding an upper limit to the dimensions
    m_array = std::vector<std::vector<char>>(m_sizeX, std::vector<char>(m_sizeY, m_defaultSymbol));
}

}// End namespace FIAR


