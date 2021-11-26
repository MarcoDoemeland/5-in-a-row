#include "Board.h"
#include <string>
#include <iomanip>


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
bool Board::add_symbol(const Position& pos, char symbol){
    return add_symbol(pos.x(), pos.y(), symbol);
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

// Telling whether the board is valid
bool Board::isValid() const{
    return (m_sizeX > 0 && m_sizeY > 0);
}
// Returning the number of slots (the area) of the board
std::size_t Board::area() const{
    return m_sizeX * m_sizeY;
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
    m_array = board_t(m_sizeX, std::vector<char>(m_sizeY, m_defaultSymbol));
}

// Overloading the << insertion operator
std::ostream& operator<<(std::ostream& stream, const Board& board){
    // TODO: switching from one to the other could be a board option
    // Both work, both are cool
    //if(board.isValid()) MarcoPrint::marcoPrint(stream, board.m_sizeY, board.m_array);
    if(board.isValid()) JosephPrint::josephPrint(stream, board.m_sizeY, board.m_array);
    return stream;
}

// Printing the board (Marco format)
namespace MarcoPrint{
void marcoPrint(std::ostream& stream, std::size_t sizeY, const board_t& array){
    stream << '\n' << '\n';
    MarcoPrint::print_array_number_row( stream, sizeY );
    MarcoPrint::print_array_top_row( stream, sizeY );
    for (std::size_t i {0}; i < array.size(); ++i){
        MarcoPrint::print_array_empty_row( stream, sizeY );
        MarcoPrint::print_array_filled_row( stream, i+1, array[i] );
        MarcoPrint::print_array_bottom_row( stream, sizeY );
    }
    stream << '\n' << '\n';
    stream.flush();
}
void print_array_top_row(std::ostream& stream, std::size_t size){
    stream << "    ";
    for (std::size_t i {0}; i < size; ++i) stream << "____";
    stream << "_" << '\n';
}
void print_array_bottom_row(std::ostream& stream, std::size_t size){
    stream << "    ";
    for (std::size_t i {0}; i < size; ++i) stream << "|___";
    stream << "|" << '\n';
}
void print_array_empty_row(std::ostream& stream, std::size_t size){
    stream << "    ";
    for (std::size_t i {0}; i < size; ++i) stream << "|   ";
    stream << "|" << '\n';
}
void print_array_filled_row(std::ostream& stream, std::size_t pos, const std::vector<char>& row){
    int padlen_left;
    int padlen_right;
    std::string str{ std::to_string(pos) };
    padlen_left = (4 + 1 - str.size()) / 2;
    padlen_right = 4 - str.size() - padlen_left;
    printf("%*s%s%*s", padlen_left, "", str.c_str(), padlen_right, "");
    for (auto i_content: row) stream << "| " << i_content << " ";
    stream << "|" << std::endl;
}
void print_array_number_row(std::ostream& stream, std::size_t size){
    int padlen_left;
    int padlen_right;
    std::string i_str;
    stream << R"( x\y)";
    for (std::size_t i {0}; i < size; ++i){
        i_str = std::to_string(i+1);
        padlen_left = (3 + 1 - i_str.size()) / 2;
        padlen_right = 3 - i_str.size() - padlen_left;
        printf(" %*s%s%*s", padlen_left, "", i_str.c_str(), padlen_right, "");
    }
    stream << " \n";
}
}// End namespace MarcoPrint

namespace JosephPrint{
// Width taken by a column
constexpr int g_colW{ 4 };
void josephPrint(std::ostream& stream, std::size_t sizeY, const board_t& array){
    stream << '\n' << '\n';
    printHorizontalHead(stream, sizeY);
    printHorizontalLine(stream, sizeY);
    for (std::size_t i {0}; i < array.size(); ++i){
        printHorizontalData(stream, i + 1, array[i]);
        printHorizontalLine(stream, sizeY);
    }
    stream << '\n' << '\n';
    stream.flush();
}
void printHorizontalHead(std::ostream& stream, std::size_t size){
    stream << " x\\y";
    for(std::size_t i{0}; i < size; ++i) stream << getFormatedNumber(i + 1);
    stream << '\n';
}
void printHorizontalLine(std::ostream& stream, std::size_t size){
    stream << "    ";
    for (std::size_t i {0}; i < size; ++i) stream << "+---";
    stream << "+\n";
}
void printHorizontalData(std::ostream& stream, std::size_t pos, const std::vector<char>& row){
    stream << getFormatedNumber(pos);
    for(char content: row) stream << "| " << content << ' ';
    stream << "|\n";
}
std::string getFormatedNumber(int number){
    static std::string convStr;
    static std::string outStr;
    static std::size_t margin;
    outStr = "    ";
    convStr = std::to_string(number);
    margin = 1 + (g_colW - convStr.size()) / 2;
    for(std::size_t j{margin}; j < outStr.size() && j - margin < convStr.size(); ++j)
        outStr[j] = convStr[j - margin];
    return outStr;
}
}// End namespace JosephPrint

}// End namespace FIAR
