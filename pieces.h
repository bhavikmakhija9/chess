#include <iostream>
#include <stdlib.h>

class Board;

using namespace std;

enum Colour
{
    White = 0,
    Black
};

enum PieceType
{
    ROOK,
    PAWN,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};

class ChessPiece
{
    Colour c;

public:
    ChessPiece(Colour c);
    virtual bool isLegalMove(int x, int y, int newx, int newy, Board &b) = 0;
    Colour getColour();
    virtual PieceType getType() = 0;
    virtual char getPieceChar() = 0;
    virtual ~ChessPiece();
};

class Pawn : public ChessPiece
{
    bool moved;

public:
    Pawn(Colour c);
    PieceType getType() override;
    bool isLegalMove(int x, int y, int newx, int newy, Board &b) override;
    char getPieceChar() override;
};

class Rook : public ChessPiece
{
    bool moved;

public:
    Rook(Colour c);
    PieceType getType() override;
    bool isLegalMove(int x, int y, int newx, int newy, Board &b) override;
     char getPieceChar() override;
};

class Bishop : public ChessPiece
{
public:
    Bishop(Colour c);
    PieceType getType() override;
    bool isLegalMove(int x, int y, int newx, int newy, Board &b) override;
     char getPieceChar() override;
};

class King : public ChessPiece
{
    bool moved;

public:
    King(Colour c);
    PieceType getType() override;
    bool isLegalMove(int x, int y, int newx, int newy, Board &b) override;
     char getPieceChar() override;
};

class Queen : public ChessPiece
{
public:
    Queen(Colour c);
    PieceType getType() override;
    bool isLegalMove(int x, int y, int newx, int newy, Board &b) override;
     char getPieceChar() override;
};

class Knight : public ChessPiece
{
public:
    Knight(Colour c);
    PieceType getType() override;
    bool isLegalMove(int x, int y, int newx, int newy, Board &b) override;
    char getPieceChar() override;
};
