#include "pieces.h"
#include "board.h"
#include <vector>
using namespace std;

ChessPiece::ChessPiece(Colour c) : c(c){};

Colour ChessPiece::getColour() { return c; }

bool ChessPiece::isLegalMove(int newx, int newy, Colour turn)
{
    if (getColour() != turn)
    {
        return false;
    }

    for (auto n : validMoves)
    {
        cout << newx << "-"<<n.x<<" "<<newy<<"-"<<n.y<<endl;
        if (newx == n.x && newy == n.y)
        {
            return true;
        }
    }
    return false;
}

Pawn::Pawn(Colour c) : ChessPiece(c), moved(false){};
void Pawn::refreshLegalMoves(int x, int y, Board &b)
{
    validMoves.clear();
    int newx, newy;
    //cout << "makes call";

    //validMoves.emplace_back(Move{5,0,MoveType::STANDARD}); //delete this later

    // double moves
    
    if (!moved)
    {
        if (getColour() == Colour::White)
        {

            ChessPiece *temp = b.getSquare(x+1, y)->getPiece();
            ChessPiece *temp2 = b.getSquare(x+2, y)->getPiece();

            if (!temp && !temp2)
            {
                Move doubleMove{x+2, y, MoveType::STANDARD};

                moved = true;
                validMoves.emplace_back(doubleMove);
            }
        }
        else
        {
            ChessPiece *temp = b.getSquare(x-1, y)->getPiece();
            ChessPiece *temp2 = b.getSquare(x-2, y)->getPiece();

            if (!temp && !temp2)
            {
                Move doubleMove{x-2, y, MoveType::STANDARD};

                moved = true;
                validMoves.emplace_back(doubleMove);
            }
        }
    }

    // normal moves
    if (getColour() == Colour::White)
    {
        newx = x-1;
        ChessPiece *temp = b.getSquare(newx, y)->getPiece();

        if (!temp)
        {
            Move move{newx, y, MoveType::STANDARD};

            validMoves.emplace_back(move);
        }
    }
    else
    {
        newx = x+1;
        ChessPiece *temp = b.getSquare(newx, y)->getPiece();

        if (!temp)
        {
            Move move{newx, y, MoveType::STANDARD};

            validMoves.emplace_back(move);
        }
    }

    // capturing moves 
    /*
    if (getColour() == Colour::White)
    {
        if (x < b.boardDim)
        {
            ChessPiece *temp = b.getSquare(x + 1, y + 1)->getPiece();

            if (temp && temp->getColour() == Colour::Black)
            {
                Move move{x + 1, y + 1, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }

        if (x > 0)
        {
            ChessPiece *temp2 = b.getSquare(x - 1, y + 1)->getPiece();

            if (temp2 && temp2->getColour() == Colour::Black)
            {
                Move move{x - 1, y + 1, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
    }
    else
    {
        if (x < b.boardDim)
        {
            ChessPiece *temp = b.getSquare(x + 1, y - 1)->getPiece();

            if (temp && temp->getColour() == Colour::Black)
            {
                Move move{x + 1, y - 1, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }

        if (x > 0)
        {
            ChessPiece *temp2 = b.getSquare(x - 1, y - 1)->getPiece();

            if (temp2 && temp2->getColour() == Colour::Black)
            {
                Move move{x - 1, y - 1, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
    }
    cout<<"current x,y"<< x << "-" << y<< endl <<validMoves.size()<<endl;
    if(validMoves.size()>0){
        cout << "valid move:" << validMoves[0].x << "-" << validMoves[0].y<< endl;
    } */
    //cout<<"current x,y"<< x << "-" << y<< endl <<validMoves.size()<<endl;
}

PieceType Pawn::getType() { return PieceType::PAWN; }

char Pawn::getPieceChar()
{
    if (getColour() == Colour::White)
    {
        return 'P';
    }
    else
    {
        return 'p';
    }
}

ChessPiece::~ChessPiece() {}

Rook::Rook(Colour c) : ChessPiece(c), moved(false){};
void Rook::refreshLegalMoves(int x, int y, Board &b)
{
    validMoves.clear();
    int newX, newY;
    
    //  there is an issue here fs, right moves up and up moves right?? 
    //  yeah idek lol what is causing this ngl

    // Right moves
    newX = x + 1;
    while (newX < b.boardDim)
    {
        ChessPiece *temp = b.getSquare(newX, y)->getPiece();
        if (temp)
        {
            if (temp->getColour() == getColour())
            {
                break;
            }
            Move move{newX, y, MoveType::CAPTURING};
            validMoves.emplace_back(move);
            break;
        }
        Move move{newX, y, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newX++;
    }

    // Left Moves
    newX = x - 1;
    while (newX >= 0)
    {
        ChessPiece *temp = b.getSquare(newX, y)->getPiece();
        if (temp)
        {
            if (temp->getColour() == getColour())
            {
                break;
            }
            Move move{newX, y, MoveType::CAPTURING};
            validMoves.emplace_back(move);
            break;
        }
        Move move{newX, y, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newX--;
    }

    // Up Moves
    newY = y + 1;
    while (newY < b.boardDim)
    {
        ChessPiece *temp = b.getSquare(x, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() == getColour())
            {
                break;
            }
            Move move{x, newY, MoveType::CAPTURING};
            validMoves.emplace_back(move);
            break;
        }
        Move move{x, newY, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newY++;
    }

    // Down Moves
    newY = y - 1;
    while (newY >= 0)
    {
        ChessPiece *temp = b.getSquare(x, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() == getColour())
            {
                break;
            }
            Move move{x, newY, MoveType::CAPTURING};
            validMoves.emplace_back(move);
            break;
        }
        Move move{x, newY, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newY--;
    }
}
PieceType Rook::getType() { return PieceType::ROOK; }
char Rook::getPieceChar()
{
    if (getColour() == Colour::White)
    {
        return 'R';
    }
    else
    {
        return 'r';
    }
}

Bishop::Bishop(Colour c) : ChessPiece(c){};
void Bishop::refreshLegalMoves(int x, int y, Board &b)
{
    validMoves.clear();
    
    // 
}
PieceType Bishop::getType() { return PieceType::BISHOP; }
char Bishop::getPieceChar()
{
    if (getColour() == Colour::White)
    {
        return 'B';
    }
    else
    {
        return 'b';
    }
}

King::King(Colour c) : ChessPiece(c), moved(false){};
void King::refreshLegalMoves(int x, int y, Board &b)
{
    validMoves.clear();
    // someone do this
}
PieceType King::getType() { return PieceType::KING; }
char King::getPieceChar()
{
    if (getColour() == Colour::White)
    {
        return 'K';
    }
    else
    {
        return 'k';
    }
}

Queen::Queen(Colour c) : ChessPiece(c){};
void Queen::refreshLegalMoves(int x, int y, Board &b)
{
    validMoves.clear();
    // someone do this
}
PieceType Queen::getType() { return PieceType::QUEEN; }
char Queen::getPieceChar()
{
    if (getColour() == Colour::White)
    {
        return 'Q';
    }
    else
    {
        return 'q';
    }
}

Knight::Knight(Colour c) : ChessPiece(c){};
void Knight::refreshLegalMoves(int x, int y, Board &b)
{
    validMoves.clear();
    // do this
}
PieceType Knight::getType() { return PieceType::KNIGHT; }
char Knight::getPieceChar()
{
    if (getColour() == Colour::White)
    {
        return 'N';
    }
    else
    {
        return 'n';
    }
}
