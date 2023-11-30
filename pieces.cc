#include "pieces.h"
#include "board.h"
#include <vector>
using namespace std;

ChessPiece::ChessPiece(Colour c) : c(c){};

Colour ChessPiece::getColour() { return c; }

vector<Move> *ChessPiece::getValidMoves() { return &validMoves; };

void ChessPiece::filterValidMoves(int x, int y, Board& b)
{
    refreshLegalMoves(x, y, b);
    vector<Move> temp;
    for (int i = 0; i < validMoves.size(); ++i)
    {    
        Board newB {b};
        if (!(resultsInCheck(x, y, validMoves[i], newB)))
        {
            temp.emplace_back(validMoves[i]);
        }
    }

    validMoves.clear();

    for (auto n : temp) {
        cout << "valid moves: "<< getPieceChar() << n.x << n.y <<endl;

    }

    validMoves = temp;
    
}

bool ChessPiece::resultsInCheck(int x, int y, Move m, Board boa)
{
    boa.makeMove(x, y, m.x, m.y);
    boa.refreshTempLegalMoves();
    // for (int i = 0; i < 8; ++i) {
    //     for (int j = 0; j < 8; ++j) {
    //         if (boa.getSquare(i,j)->getPiece()) {
    //             //boa.getSquare(i,j)->getPiece()->refreshLegalMoves(i, j, boa);
    //             //cout << boa.getSquare(i,j)->getPiece()->getPieceChar();
    //         } else {
    //             //cout << "*";
    //         }
    //     }
    //     //cout << endl;
    // }
    cout << getPieceChar() << " " << m.x << " " << m.y <<"' " << boa.isChecked(getColour()) << endl;
    return boa.isChecked(getColour());
}

bool ChessPiece::isLegalMove(int newx, int newy, Colour turn)
{
    if (getColour() != turn)
    {
        return false;
    }

    for (auto n : validMoves)
    {  cout << "valid moves: "<< getPieceChar() << n.x << n.y <<endl;

        cout << getPieceChar() << " " << n.x << " " << n.y << endl;
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
    int newx;
    // cout << "makes call";

    // validMoves.emplace_back(Move{5,0,MoveType::STANDARD}); //delete this later

    if (getColour() == Colour::White && x == 6)
    {

        ChessPiece *temp = b.getSquare(x - 1, y)->getPiece();
        ChessPiece *temp2 = b.getSquare(x - 2, y)->getPiece();

        if (!temp && !temp2)
        {
            Move doubleMove{x - 2, y, MoveType::STANDARD};
            moved = true;
            validMoves.emplace_back(doubleMove);
        }
    }
    else if (x == 1)
    {
        ChessPiece *temp = b.getSquare(x + 1, y)->getPiece();
        ChessPiece *temp2 = b.getSquare(x + 2, y)->getPiece();

        if (!temp && !temp2)
        {
            Move doubleMove{x + 2, y, MoveType::STANDARD};

            moved = true;
            validMoves.emplace_back(doubleMove);
        }
    }

    // normal moves
    if (getColour() == Colour::White && x > 0)
    {
        newx = x - 1;
        ChessPiece *temp = b.getSquare(newx, y)->getPiece();

        if (!temp)
        {
            Move move{newx, y, MoveType::STANDARD};

            validMoves.emplace_back(move);
        }
    }
    else if (x < b.boardDim - 1)
    {
        newx = x + 1;
        ChessPiece *temp = b.getSquare(newx, y)->getPiece();

        if (!temp)
        {
            Move move{newx, y, MoveType::STANDARD};

            validMoves.emplace_back(move);
        }
    }

    // capturing moves
    if (getColour() == Colour::White)
    {
        if (x < b.boardDim - 1)
        {
            ChessPiece *temp = b.getSquare(x - 1, y + 1)->getPiece();

            if (temp && temp->getColour() == Colour::Black)
            {
                Move move = {x - 1, y + 1, MoveType::CAPTURING};
                if (temp->getType() == KING && temp->getColour() != getColour())
                {
                    move = {x - 1, y + 1, MoveType::CHECKING};
                }
                validMoves.emplace_back(move);
            }
        }

        if (x > 0)
        {
            ChessPiece *temp2 = b.getSquare(x - 1, y - 1)->getPiece();

            if (temp2 && temp2->getColour() == Colour::Black)
            {
                Move move{x - 1, y - 1, MoveType::CAPTURING};
                if (temp2->getType() == KING && temp2->getColour() != getColour())
                {
                    move = {x - 1, y - 1, MoveType::CHECKING};
                }
                validMoves.emplace_back(move);
            }
        }
    }
    else
    {
        if (x < b.boardDim - 1)
        {
            ChessPiece *temp = b.getSquare(x + 1, y + 1)->getPiece();

            if (temp && temp->getColour() == Colour::Black)
            {
                Move move{x + 1, y + 1, MoveType::CAPTURING};
                if (temp->getType() == KING && temp->getColour() != getColour())
                {
                    move = {x + 1, y + 1, MoveType::CHECKING};
                }
                validMoves.emplace_back(move);
            }
        }

        if (x > 0)
        {
            ChessPiece *temp2 = b.getSquare(x + 1, y - 1)->getPiece();

            if (temp2 && temp2->getColour() == Colour::Black)
            {
                Move move{x + 1, y - 1, MoveType::CAPTURING};
                if (temp2->getType() == KING && temp2->getColour() != getColour())
                {
                    move = {x + 1, y - 1, MoveType::CHECKING};
                }
                validMoves.emplace_back(move);
            }
        }
    }
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

    // Down moves
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
            if (temp->getType() == KING && temp->getColour() != getColour())
            {
                move = {newX, y, MoveType::CHECKING};
            }
            validMoves.emplace_back(move);
            break;
        }
        Move move{newX, y, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newX++;
    }

    // Up Moves
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
            if (temp->getType() == KING && temp->getColour() != getColour())
            {
                move = {newX, y, MoveType::CHECKING};
            }
            validMoves.emplace_back(move);
            break;
        }
        Move move{newX, y, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newX--;
    }

    // Right Moves
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
            if (temp->getType() == KING && temp->getColour() != getColour())
            {
                move = {newX, y, MoveType::CHECKING};
            }
            validMoves.emplace_back(move);
            break;
        }
        Move move{x, newY, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newY++;
    }

    // Left Moves
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
            if (temp->getType() == KING && temp->getColour() != getColour())
            {
                move = {newX, y, MoveType::CHECKING};
            }
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
    int newX, newY;

    // Up Right Moves
    newX = x - 1;
    newY = y + 1;
    while (newX >= 0 && newY < b.boardDim)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() == getColour())
            {
                break;
            }
            Move move{newX, newY, MoveType::CAPTURING};
            if (temp->getType() == KING && temp->getColour() != getColour())
            {
                move = {newX, newY, MoveType::CHECKING};
            }
            validMoves.emplace_back(move);
            break;
        }
        Move move{newX, newY, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newX--;
        newY++;
    }

    // Down Right Moves
    newX = x + 1;
    newY = y + 1;
    while (newX < b.boardDim && newY < b.boardDim)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() == getColour())
            {
                break;
            }
            Move move{newX, newY, MoveType::CAPTURING};
            if (temp->getType() == KING && temp->getColour() != getColour())
            {
                move = {newX, newY, MoveType::CHECKING};
            }
            validMoves.emplace_back(move);
            break;
        }
        Move move{newX, newY, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newX++;
        newY++;
    }

    // Up Left Moves
    newX = x - 1;
    newY = y - 1;
    while (newX >= 0 && newY >= 0)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() == getColour())
            {
                break;
            }
            Move move{newX, newY, MoveType::CAPTURING};
            if (temp->getType() == KING && temp->getColour() != getColour())
            {
                move = {newX, newY, MoveType::CHECKING};
            }
            validMoves.emplace_back(move);
            break;
        }
        Move move{newX, newY, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newX--;
        newY--;
    }

    // Down Left Moves
    newX = x + 1;
    newY = y - 1;
    while (newX < b.boardDim && newY >= 0)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() == getColour())
            {
                break;
            }
            Move move{newX, newY, MoveType::CAPTURING};
            if (temp->getType() == KING && temp->getColour() != getColour())
            {
                move = {newX, newY, MoveType::CHECKING};
            }
            validMoves.emplace_back(move);
            break;
        }
        Move move{newX, newY, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newX++;
        newY--;
    }
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

    Colour myColour = getColour();

    if (x > 0)
    {
        ChessPiece *up = b.getSquare(x - 1, y)->getPiece(); // moving up the board
        if (up)
        {
            if (up->getColour() != myColour)
            {
                Move move = {x - 1, y, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        { // empty sqaure
            Move move = {x - 1, y, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }

    if (x < b.boardDim - 1)
    {

        ChessPiece *down = b.getSquare(x + 1, y)->getPiece(); // moving down the board

        if (down)
        {
            if (down->getColour() != myColour)
            {
                Move move = {x + 1, y, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        {
            Move move = {x + 1, y, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }

    if (y < b.boardDim - 1)
    {
        ChessPiece *right = b.getSquare(x, y + 1)->getPiece(); // moving right
        if (right)
        {
            if (right->getColour() != myColour)
            {
                Move move = {x, y + 1, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        {
            Move move = {x, y + 1, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }

    if (y > 0)
    {
        ChessPiece *left = b.getSquare(x, y - 1)->getPiece(); // moving left

        if (left)
        {
            if (left->getColour() != myColour)
            {
                Move move = {x, y - 1, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        {
            Move move = {x, y - 1, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }

    if (x > 0 && y < b.boardDim - 1)
    {
        ChessPiece *topRight = b.getSquare(x - 1, y + 1)->getPiece(); // moving upward diagonal right
        if (topRight)
        {
            if (topRight->getColour() != myColour)
            {
                Move move = {x - 1, y + 1, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        {
            Move move = {x - 1, y + 1, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }

    if (x > 0 && y > 0)
    {
        ChessPiece *topLeft = b.getSquare(x - 1, y - 1)->getPiece(); // moving upward diagonal left
        if (topLeft)
        {
            if (topLeft->getColour() != myColour)
            {
                Move move = {x - 1, y - 1, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        {
            Move move = {x - 1, y - 1, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }

    if (x < b.boardDim - 1 && y < b.boardDim - 1)
    {
        ChessPiece *bottomRight = b.getSquare(x + 1, y + 1)->getPiece(); // moving downward diagonal right

        if (bottomRight)
        {
            if (bottomRight->getColour() != myColour)
            {
                Move move = {x + 1, y + 1, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        {
            Move move = {x + 1, y + 1, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }

    if (x < b.boardDim - 1 && y > 0)
    {
        ChessPiece *bottomLeft = b.getSquare(x + 1, y - 1)->getPiece(); // moving downward diagonal left

        if (bottomLeft)
        {
            if (bottomLeft->getColour() != myColour)
            {
                Move move = {x + 1, y - 1, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        {
            Move move = {x + 1, y - 1, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }
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
    int newX, newY;

    // Down moves
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
            if (temp->getType() == KING && temp->getColour() != getColour())
            {
                move = {newX, y, MoveType::CHECKING};
            }
            validMoves.emplace_back(move);
            break;
        }
        Move move{newX, y, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newX++;
    }

    // Up Moves
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

    // Right Moves
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

    // Left Moves
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

    // Up Right Moves
    newX = x - 1;
    newY = y + 1;
    while (newX >= 0 && newY < b.boardDim)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() == getColour())
            {
                break;
            }
            Move move{newX, newY, MoveType::CAPTURING};
            if (temp->getType() == KING && temp->getColour() != getColour())
            {
                move = {newX, newY, MoveType::CHECKING};
            }
            validMoves.emplace_back(move);
            break;
        }
        Move move{newX, newY, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newX--;
        newY++;
    }

    // Down Right Moves
    newX = x + 1;
    newY = y + 1;
    while (newX < b.boardDim && newY < b.boardDim)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() == getColour())
            {
                break;
            }
            Move move{newX, newY, MoveType::CAPTURING};
            if (temp->getType() == KING && temp->getColour() != getColour())
            {
                move = {newX, newY, MoveType::CHECKING};
            }
            validMoves.emplace_back(move);
            break;
        }
        Move move{newX, newY, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newX++;
        newY++;
    }

    // Up Left Moves
    newX = x - 1;
    newY = y - 1;
    while (newX >= 0 && newY >= 0)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() == getColour())
            {
                break;
            }
            Move move{newX, newY, MoveType::CAPTURING};
            if (temp->getType() == KING && temp->getColour() != getColour())
            {
                move = {newX, newY, MoveType::CHECKING};
            }
            validMoves.emplace_back(move);
            break;
        }
        Move move{newX, newY, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newX--;
        newY--;
    }

    // Down Left Moves
    newX = x + 1;
    newY = y - 1;
    while (newX < b.boardDim && newY >= 0)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() == getColour())
            {
                break;
            }
            Move move{newX, newY, MoveType::CAPTURING};
            if (temp->getType() == KING && temp->getColour() != getColour())
            {
                move = {newX, newY, MoveType::CHECKING};
            }
            validMoves.emplace_back(move);
            break;
        }
        Move move{newX, newY, MoveType::STANDARD};
        validMoves.emplace_back(move);
        newX++;
        newY--;
    }
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
    int newX, newY;

    // Bottom Right
    newX = x + 2;
    newY = y + 1;
    if (newX < b.boardDim && newY < b.boardDim)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() != getColour())
            {
                Move move{newX, newY, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        {
            Move move{newX, newY, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }

    // Bottom Left
    newY = y - 1;
    if (newX < b.boardDim && newY >= 0)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() != getColour())
            {
                Move move{newX, newY, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        {
            Move move{newX, newY, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }

    // Top Right
    newX = x - 2;
    newY = y + 1;
    if (newX >= 0 && newY < b.boardDim)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() != getColour())
            {
                Move move{newX, newY, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        {
            Move move{newX, newY, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }

    // Top Left
    newY = y - 1;
    if (newX >= 0 && newY >= 0)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() != getColour())
            {
                Move move{newX, newY, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        {
            Move move{newX, newY, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }

    // Upper Right
    newX = x - 1;
    newY = y + 2;
    if (newX >= 0 && newY < b.boardDim)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() != getColour())
            {
                Move move{newX, newY, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        {
            Move move{newX, newY, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }

    // Bottom Right
    newX = x + 1;
    if (newX < b.boardDim && newY < b.boardDim)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() != getColour())
            {
                Move move{newX, newY, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        {
            Move move{newX, newY, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }

    // Upper Left
    newX = x - 1;
    newY = y - 2;
    if (newX >= 0 && newY >= 0)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() != getColour())
            {
                Move move{newX, newY, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        {
            Move move{newX, newY, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }

    // Bottom Left
    newX = x + 1;
    if (newX < b.boardDim && newY >= 0)
    {
        ChessPiece *temp = b.getSquare(newX, newY)->getPiece();
        if (temp)
        {
            if (temp->getColour() != getColour())
            {
                Move move{newX, newY, MoveType::CAPTURING};
                validMoves.emplace_back(move);
            }
        }
        else
        {
            Move move{newX, newY, MoveType::STANDARD};
            validMoves.emplace_back(move);
        }
    }
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
