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
    Position(std::size_t x, std::size_t y);

    // Assignement operator
    Position& operator=(const Position& pos);

    // Accessors
    std::size_t x() const;
    std::size_t y() const;
    void setX(std::size_t val);
    void setY(std::size_t val);

    // I/O operators
    friend std::ostream& operator<<(std::ostream& stream, const Position& pos);
    friend std::istream& operator>>(std::istream& stream, Position& pos);

private:
    // Actual position
    std::size_t m_x;
    std::size_t m_y;

};

}// End namespace FIAR

#endif // POSITION_H
