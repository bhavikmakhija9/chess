#include <vector>
#include <iostream>
#include "pieces.h"
#include "observers.h"
using namespace std;

class Square
{
    ChessPiece *cp = nullptr;
    int x, y;
    vector<Observer*> observers;

public:
    bool isEmpty();
    void setPiece(ChessPiece *newCp);
    void clearSquare();
    void setCoords(int x, int y);
    int getX();
    int getY();
    ChessPiece *getPiece();
    void attach(Observer* o);
    void notifyAllObservers();
};

class Board
{
    const int boardDim = 8;
    vector<vector<Square>> board;
    void setWarFormationRows(int i, Colour c);
    //TextDisplay *td;
public:
    Board();
    void clearBoard();
    void newBoard();
    void makeMove(int x, int y, int newx, int newy);
    void attachDisplay(Observer* o);

    //friend std::ostream &operator<<(std::ostream &out, const Board &b);
};
