#include "board.h"
#include <vector>
#include <utility>
using namespace std;

void Square::clearSquare()
{
    if (!cp) {
        delete cp;
    }
    cp = nullptr;
}

void Square::setCoords(int x, int y)
{
    this->x = x;
    this->y = y;
}

int Square::getX()
{
    return x;
}

int Square::getY()
{
    return y;
}

bool Square::isEmpty() { return !cp; }

void Square::setPiece(ChessPiece *newCp)
{
    delete cp;
    cp = newCp;
}

ChessPiece *Square::getPiece() { return cp; };

void Square::attach(Observer *o) { observers.emplace_back(o); }

void Square::notifyAllObservers()
{
    for (auto o : observers)
    {
        o->notify(*this);
    }
}

Board::Board() {}

void Board::clearBoard()
{
    for (int i = 0; i < boardDim; i++)
    {
        for (int j = 0; j < boardDim; j++)
        {
            board[i][j].clearSquare();
        }
    }
    board.clear();
}

void Board::setWarFormationRows(int i, Colour c)
{
    board[i][0].setPiece(new Rook(c));
    board[i][1].setPiece(new Knight(c));
    board[i][2].setPiece(new Bishop(c));
    board[i][3].setPiece(new Queen(c));
    board[i][4].setPiece(new King(c));
    board[i][5].setPiece(new Bishop(c));
    board[i][6].setPiece(new Knight(c));
    board[i][7].setPiece(new Rook(c));
}

void Board::newBoard()
{
    board.resize(boardDim, vector<Square>(boardDim, Square()));
    td = new TextDisplay;
    for (int i = 0; i < boardDim; i++)
    {

        // Add pieces to the square as per standard chess formations depending on the row
        if (i == 0)
            setWarFormationRows(i, Colour::Black);
        else if (i == 1)
        {
            for (int j = 0; j < boardDim; j++)
            {
                board[i][j].setPiece(new Pawn(Colour::Black));
            }
        }
        else if (i == 6)
        {
            for (int j = 0; j < boardDim; j++)
            {
                board[i][j].setPiece(new Pawn(Colour::White));
            }
        }
        else if (i == 7)
            setWarFormationRows(i, Colour::White);

        // Set coords of the row
        for (int j = 0; j < boardDim; j++)
        {
            board[i][j].setCoords(i, j);
            board[i][j].attach(td);
        }
    }
}

void Board::makeMove(int x, int y, int newx, int newy)
{

    board[newx][newy].setPiece(board[x][y].getPiece());
    board[x][y].clearSquare();

    for (int i = 0; i < boardDim; i++)
    {
        for (int j = 0; j < boardDim; j++)
        {
            board[i][j].notifyAllObservers();
        }
    }
}

std::ostream &operator<<(std::ostream &out, const Board &b)
{
    out << *(b.td);
    return out;
}
