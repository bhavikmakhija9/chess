#include "board.h"
#include <vector>
#include <utility>
using namespace std;

Square::Square() : x{0}, y{0} {};

Square::Square(const Square &other)
{
    x = other.x;
    y = other.y;
    myC = other.myC;
    if (cp)
    {
        delete cp;
    }
    if (other.cp)
    {
        switch (other.cp->getType())
        {
        case PAWN:
            cp = new Pawn(*(static_cast<Pawn *>(other.cp)));
            break;
        case ROOK:
            cp = new Rook(*(static_cast<Rook *>(other.cp)));
            break;
        case BISHOP:
            cp = new Bishop(*(static_cast<Bishop *>(other.cp)));
            break;
        case QUEEN:
            cp = new Queen(*(static_cast<Queen *>(other.cp)));
            break;
        case KNIGHT:
            cp = new Knight(*(static_cast<Knight *>(other.cp)));
            break;
        case KING:
            cp = new King(*(static_cast<King *>(other.cp)));
            break;
        default:
            break;
        }
    }
    else
    {
        cp = nullptr;
    }
}

void Square::clearSquare()
{
    if (cp)
    {
        delete cp;
    }
    cp = nullptr;
}

void Square::setCoords(int x, int y)
{
    this->x = x;
    this->y = y;
}

int Square::getX() const
{
    return x;
}

int Square::getY() const
{
    return y;
}

Colour Square::getColour() const
{
    return myC;
}

void Square::setColour(Colour col)
{
    myC = col;
}

bool Square::isEmpty() { return !cp; }

void Square::setPiece(ChessPiece *newCp)
{
    // if (cp)
    // {
    //     delete cp;
    // }
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


Square::~Square()
{
    if (cp)
    {
        delete cp;
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

void Board::refreshLegalMoves()
{
    for (int i = 0; i < boardDim; i++)
    {
        for (int j = 0; j < boardDim; j++)
        {
            if (board[i][j].getPiece())
            {
                board[i][j].getPiece()->refreshLegalMoves(i, j, *this);
            }
        }
    }
}

void Board::defBoard()
{
    Colour col = White;
    board.resize(boardDim, vector<Square>(boardDim, Square()));
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
        else {
            for (int j = 0; j < boardDim; j++)
            {
                board[i][j].setPiece(nullptr);
            }
        }

        // Set coords of the row and the colour of the squares
        for (int j = 0; j < boardDim; j++)
        {
            board[i][j].setCoords(i, j);
            board[i][j].setColour(col);
            if (j != boardDim - 1)
            {
                if (col == Colour::White)
                {
                    col = Colour::Black;
                }
                else
                {
                    col = Colour::White;
                }
            }
        }
    }
}

void Board::refreshForEnPassant(Colour c)
{
    for (int i = 0; i < boardDim; ++i)
    {
        for (int j = 0; j < boardDim; ++j)
        {
            if (getSquare(i, j)->getPiece() && getSquare(i, j)->getPiece()->getType() == PAWN && getSquare(i, j)->getPiece()->getColour() == c)
            {
                static_cast<Pawn *>(getSquare(i, j)->getPiece())->setMovedTwo(false);
                break;
            }
        }
    }
}

void Board::makeMove(int x, int y, int newx, int newy)
{

    // For Castling
    if (board[x][y].getPiece()->getType() == KING)
    {
        King *ptr = dynamic_cast<King *>(board[x][y].getPiece());
        if (ptr)
        {
            ptr->setMoved(true);
        }
    }
    else if (board[x][y].getPiece()->getType() == ROOK)
    {
        Rook *ptr = dynamic_cast<Rook *>(board[x][y].getPiece());
        if (ptr)
        {
            ptr->setMoved(true);
        }
    }
    // For En Passant
    else if (board[x][y].getPiece()->getType() == PAWN)
    {
        Pawn *ptr = static_cast<Pawn *>(board[x][y].getPiece());
        if (x + 2 == newx || x - 2 == newx)
        {
            ptr->setMovedTwo(true);
        }
    }

    board[newx][newy].setPiece(board[x][y].getPiece());

    board[x][y].setPiece(nullptr);

    board[x][y].clearSquare();

}

void Board::attachDisplay(Observer *o)
{
    for (int i = 0; i < boardDim; i++)
    {
        for (int j = 0; j < boardDim; j++)
        {
            board[i][j].attach(o);
        }
    }
}

Square *Board::getSquare(int x, int y)
{
    return &board[x][y];
}

void Board::emptyBoard()
{
    for (int i = 0; i < boardDim; i++)
    {
        for (int j = 0; j < boardDim; j++)
        {
            board[i][j].clearSquare();
        }
    }
}

void Board::notifyObservers()
{
    for (int i = 0; i < boardDim; i++)
    {
        for (int j = 0; j < boardDim; j++)
        {
            board[i][j].notifyAllObservers();
        }
    }
}

bool Board::isChecked(Colour c)
{
    for (int i = 0; i < boardDim; i++)
    {
        for (int j = 0; j < boardDim; j++)
        {
            if (board[i][j].getPiece())
            {
                vector<Move> *ptr = board[i][j].getPiece()->getValidMoves();
                for (auto n : *ptr)
                {
                    if (n.type == CHECKING && board[i][j].getPiece()->getColour() != c)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Board::hasNoMoves(Colour c)
{
    for (int i = 0; i < boardDim; i++)
    {
        for (int j = 0; j < boardDim; j++)
        {
            if (board[i][j].getPiece() && board[i][j].getPiece()->getColour() == c)
            {
                vector<Move> *ptr = board[i][j].getPiece()->getValidMoves();
                if (ptr->size() != 0)
                {
                    return false;
                }
            }
        }
    }
    return true;
}
