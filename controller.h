#ifndef CONTROL_H
#define CONTROL_H
#include "board.h"
#include <string>
#include <iostream>
#include <utility>

class Controller
{
    Board b;
    TextDisplay *td;
    ChessPiece *translate(char c); //turns a character into the corresponding ChessPiece
    Colour turn;
    std::pair<int, int> translateMove(string str); //translates move from chess board notation to grid notation
    void toggleTurn();

public:
    Controller();
    void makeMove(string initial, string dest, std::ostream &out);
    void print(std::ostream &out) const;
    void setup(std::istream &in, std::ostream &out);
};
#endif