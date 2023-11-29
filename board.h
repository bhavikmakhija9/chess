#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <iostream>
#include "pieces.h"
#include "observers.h"
using namespace std;

class Square
{
    ChessPiece *cp = nullptr;
    int x, y;
    vector<Observer *> observers;

public:
    bool isEmpty();
    void setPiece(ChessPiece *newCp);
    void clearSquare();
    void setCoords(int x, int y);
    int getX();
    int getY();
    ChessPiece *getPiece();
    void attach(Observer *o);
    void notifyAllObservers();
};

class Board
{
    vector<vector<Square>> board;
    void setWarFormationRows(int i, Colour c);
public:
    const int boardDim = 8;
    Board();
    void clearBoard();
    void refreshLegalMoves();
    void defBoard();
    void makeMove(int x, int y, int newx, int newy);
    void attachDisplay(Observer *o);
    void emptyBoard();
    Square *getSquare(int x, int y);
};
#endif
