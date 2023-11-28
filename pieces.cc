#include "pieces.h"
using namespace std;

ChessPiece::ChessPiece(Colour c) : c(c){};

Colour ChessPiece::getColour() { return c; }

Pawn::Pawn(Colour c) : ChessPiece(c), moved(false){};
bool Pawn::isLegalMove(int x, int y, int newx, int newy, Board &b)
{
    return 1; // someone do this
}
PieceType Pawn::getType() { return PieceType::PAWN; }

char Pawn::getPieceChar() {
    if (getColour() == Colour::White){
        return 'P';
    } else {
        return 'p';
    }
}

ChessPiece::~ChessPiece() {}

Rook::Rook(Colour c) : ChessPiece(c), moved(false){};
bool Rook::isLegalMove(int x, int y, int newx, int newy, Board &b)
{
    return 1; // someone do this
}
PieceType Rook::getType() { return PieceType::ROOK; }
char Rook::getPieceChar() {
    if (getColour() == Colour::White){
        return 'R';
    } else {
        return 'r';
    }
}

Bishop::Bishop(Colour c) : ChessPiece(c){};
bool Bishop::isLegalMove(int x, int y, int newx, int newy, Board &b)
{
    return 1; // someone do this
}
PieceType Bishop::getType() { return PieceType::BISHOP; }
char Bishop::getPieceChar() {
    if (getColour() == Colour::White){
        return 'B';
    } else {
        return 'b';
    }
}

King::King(Colour c) : ChessPiece(c), moved(false){};
bool King::isLegalMove(int x, int y, int newx, int newy, Board &b)
{
    return 1; // someone do this
}
PieceType King::getType() { return PieceType::KING; }
char King::getPieceChar() {
    if (getColour() == Colour::White){
        return 'K';
    } else {
        return 'k';
    }
}

Queen::Queen(Colour c) : ChessPiece(c){};
bool Queen::isLegalMove(int x, int y, int newx, int newy, Board &b)
{
    return 1; // someone do this
}
PieceType Queen::getType() { return PieceType::QUEEN; }
char Queen::getPieceChar() {
    if (getColour() == Colour::White){
        return 'Q';
    } else {
        return 'q';
    }
}

Knight::Knight(Colour c) : ChessPiece(c){};
bool Knight::isLegalMove(int x, int y, int newx, int newy, Board &b)
{
    return 1; // someone do this
}
PieceType Knight::getType() { return PieceType::KNIGHT; }
char Knight::getPieceChar() {
    if (getColour() == Colour::White){
        return 'N';
    } else {
        return 'n';
    }
}

