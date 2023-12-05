#ifndef PIECES_H
#define PIECES_H
#include <iostream>
#include <stdlib.h>
#include <vector>
using namespace std;

class Board;


// Two types of Colours for pieces and squares
enum Colour
{
    White,
    Black
};

// All the types of pices possible on the chess board
enum PieceType
{
    ROOK,
    PAWN,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};

// Type of Move pieces can make
enum MoveType
{
    STANDARD,
    CAPTURING,
    CASTLING,
    ENPASSANT,
    CHECKING
};

// struct to store move coords and type
struct Move
{
    int x, y;
    MoveType type;
};

class ChessPiece
{
protected:
    Colour mycolour;
    // A list of all valid moves the piece can make
    vector<Move> validMoves;

public:
    ChessPiece(Colour c);
    // Searches valid move list for the move given to the user
    bool isLegalMove(int newx, int newy, Colour turn);
    // Removes a move from the valid move list
    void deleteMove(int newx, int newy);
    // Refreshes valid moves list based on movement patterns
    // Does not account for if the move puts your king in check
    virtual void refreshLegalMoves(int x, int y, Board &b) = 0;
    Colour getColour() const;
    vector<Move> *getValidMoves();
    virtual PieceType getType() const = 0;
    virtual char getPieceChar() const = 0;
    virtual int getValue() const = 0;
    virtual ~ChessPiece();
};

class Pawn : public ChessPiece
{
    // Only true when the pawn has just moved two squares, this is for En Passant
    bool justMovedTwo = false;

public:
    Pawn(Colour c);
    void setMovedTwo(bool b);
    bool getMovedTwo();
    PieceType getType() const override;
    void refreshLegalMoves(int x, int y, Board &b) override;
    int getValue() const override;
    char getPieceChar() const override;
};

class Rook : public ChessPiece
{
    // Only true when the rook has moved, this is for castling
    bool moved;

public:
    Rook(Colour c);
    void setMoved(bool b);
    bool getMoved() const;
    PieceType getType() const override;
    int getValue() const override;
    void refreshLegalMoves(int x, int y, Board &b) override;
    char getPieceChar() const override;
};

class Bishop : public ChessPiece
{
public:
    Bishop(Colour c);
    PieceType getType() const override;
    int getValue() const override;
    void refreshLegalMoves(int x, int y, Board &b) override;
    char getPieceChar() const override;
};

class King : public ChessPiece
{
    // Only true when the king has moved, this is for castling
    bool moved;

public:
    King(Colour c);
    PieceType getType() const override;
    bool getMoved();
    void setMoved(bool b);
    int getValue() const override;
    void refreshLegalMoves(int x, int y, Board &b) override;
    char getPieceChar() const override;
};

class Queen : public ChessPiece
{
public:
    Queen(Colour c);
    PieceType getType() const override;
    int getValue() const override;
    void refreshLegalMoves(int x, int y, Board &b) override;
    char getPieceChar() const override;
};

class Knight : public ChessPiece
{
public:
    Knight(Colour c);
    PieceType getType() const override;
    int getValue() const override;
    void refreshLegalMoves(int x, int y, Board &b) override;
    char getPieceChar() const override;
};
#endif