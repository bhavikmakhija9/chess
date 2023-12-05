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
    // Coords of the square on the board
    int x, y;
    // List of Displays
    vector<Observer *> observers;
    // Square Colour
    Colour myC;

public:
    Square();
    Square(const Square &other);
    // Checks to see if a piece exists or ntt
    bool isEmpty();
    void setPiece(ChessPiece *newCp);
    // removes a piece from the square if it exists
    void clearSquare();
    void setCoords(int x, int y);
    int getX() const;
    int getY() const;
    Colour getColour() const;
    void setColour(Colour col);
    ChessPiece *getPiece();
    void attach(Observer *o);
    void notifyAllObservers();
    ~Square();
};

class Board
{
    vector<vector<Square>> board;
    // Sets the initial position of the first and last rows of a default chess board
    void setWarFormationRows(int i, Colour c);
public:
    const int boardDim = 8;
    Board();
    // Removes all pieces on the board
    void clearBoard();
    // Calls refreshLegalMoves for each piece on the board
    void refreshLegalMoves();
    // Will change the value of justMovedTwo for the Pawn given the colour
    void refreshForEnPassant(Colour c);
    // Sets the default piece positions on the board
    void defBoard();
    // Moves the piece at the given coords to the new given coords
    // If a piece exists at the new coords, it will be deleted
    void makeMove(int x, int y, int newx, int newy);
    // Adds the display to the observer list of every square
    void attachDisplay(Observer *o);
    // Clears every square on the board
    void emptyBoard();
    Square *getSquare(int x, int y);
    // Checks to see if the king is in checked based on if the colour not given has a MoveType CHECKING in one of its pieces valid moves
    bool isChecked(Colour c);
    // Checks to see if any piece is allowed to make a move on the board
    bool hasNoMoves(Colour c);
    void notifyObservers();
};
#endif
