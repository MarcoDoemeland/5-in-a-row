#ifndef POSITION_H
#define POSITION_H

#include <iostream>

namespace FIAR{

class Position{
public:
    // Standard constructor
    Position();
    // Copy constructor
    Position(const Position& pos);
    // Constructor taking a position with 2 integers as input (actual constructor)
    Position(int x, int y);

    // Assignement operator
    Position& operator=(const Position& pos);


    // Accessors
    int x() const;
    int y() const;

    // I/O operators
    friend std::ostream& operator<<(std::ostream& stream, const Position& pos);
    friend std::istream& operator>>(std::istream& stream, Position& pos);

private:
    // Actual position
    int m_x;
    int m_y;

};


// algebraic operations for Position
const Position operator+(const Position& pos1, const Position& pos2);	// addition of Positions
const Position operator-(const Position& pos1, const Position& pos2);	// subtraction of Positions
const Position operator*(const int& factor, const Position& pos);	// scalar multiplication
const Position operator*(const Position& pos, const int& factor);	// scalar multiplication


}// End namespace FIAR

#endif // POSITION_H
