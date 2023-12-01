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
    notifyAllObservers();
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

Colour Square::getColour()
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

    notifyAllObservers();
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

Board &Board::operator=(const Board &other)
{
    board = other.board;
    return *this;
}

Board ::Board(const Board &other) { board = other.board; }

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

        // Set coords of the row
        for (int j = 0; j < boardDim; j++)
        {
            board[i][j].setCoords(i, j);
            board[i][j].setColour(col);
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

void Board::makeMove(int x, int y, int newx, int newy)
{
    board[newx][newy].setPiece(board[x][y].getPiece());
    board[x][y].setPiece(nullptr);

    board[x][y].clearSquare();

    for (int i = 0; i < boardDim; i++)
    {
        for (int j = 0; j < boardDim; j++)
        {
            board[i][j].notifyAllObservers();
        }
    }
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
