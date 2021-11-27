#include "Position.h"
#include <string>
#include <algorithm>

namespace FIAR{

// Standard constructor
Position::Position() : Position(0, 0){

}
// Copy constructor
Position::Position(const Position& pos) : Position(pos.m_x, pos.m_y){

}
// Constructor taking a position with 2 integers as input (actual constructor)
Position::Position(std::size_t x, std::size_t y) : m_x{x}, m_y{y}{

}

// Assignement operator
Position& Position::operator=(const Position& pos){
    m_x = pos.m_x;
    m_y = pos.m_y;
    return *this;
}

// Accessors
std::size_t Position::x() const{
    return m_x;
}
std::size_t Position::y() const{
    return m_y;
}

// I/O operators
std::ostream& operator<<(std::ostream& stream, const Position& pos){
    stream << "Position (" << pos.m_x << ", " << pos.m_y << ')';
    return stream;
}
using strIter_t = std::string::iterator;
std::istream& operator>>(std::istream& stream, Position& pos){
    std::string str;

    // Protection against empty strings. Sometimes, if cin >> was used before, it
    // seems that endline chars still remain somewhere and the getline function gets
    // called even if the user didn't enter anything. This acts as a protection
    // against empty strings anyway.
    while(str.size() <= 0) std::getline(stream, str);
    // Placing the end char back into the string to simulate a standard cin >> call
    stream.unget();

    // Looking for tokens
    strIter_t itBeg{ std::find(str.begin(), str.end(), '(') };
    strIter_t itEnd{ std::find(str.begin(), str.end(), ')') };
    strIter_t itSep{ std::find(str.begin(), str.end(), ',') };
    // Checking token existence
    if(itBeg == str.end() || itEnd == str.end() || itSep == str.end()){
        stream.setstate(std::ios::failbit);
        std::cerr << "Invalid format: tokens not found.\n";
    }
    // Checking token position
    else if(itBeg > itSep || itSep > itEnd){
        stream.setstate(std::ios::failbit);
        std::cerr << "Invalid format: tokens have bad positions.\n";
    }
    // Checking token space
    else if(itSep - itBeg <= 1 || itEnd - itSep <= 1){
        stream.setstate(std::ios::failbit);
        std::cerr << "Invalid format: no data found between the tokens.\n";
    }
    // Format looks ok, trying to extract
    else{
        try{
            std::size_t x{ std::stoul(std::string(itBeg + 1, itSep)) };
            std::size_t y{ std::stoul(std::string(itSep + 1, itEnd)) };
            pos.m_x = x;
            pos.m_y = y;
        }
        catch(...){
            stream.setstate(std::ios::failbit);
            std::cerr << "Invalid format: conversion to unsigned integer failed.\n";
        }
    }
    return stream;
}

}// End namespace FIAR
