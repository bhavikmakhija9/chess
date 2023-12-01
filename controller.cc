#include "controller.h"

using namespace std;

void Controller::setBoard(Board newB)
{
    b = newB;
}
Controller::Controller() : td(new TextDisplay), turn(White)
{
    b.defBoard();
    b.refreshLegalMoves();
    b.attachDisplay(td);
}

std::pair<int, int> Controller::translateMove(string str)
{
    int row = 8 - str[1] + '0';
    int col = str[0] - 'a';
    return std::pair<int, int>{row, col};
}

void Controller::filterValidMoves()
{
    vector<std::pair<int, int>> curCoords;
    vector<std::pair<int, int>> newCoords;
    for (int i = 0; i < b.boardDim; ++i)
    {
        for (int j = 0; j < b.boardDim; ++j)
        {
            if (b.getSquare(i, j)->getPiece())
            {
                for (auto n : *b.getSquare(i, j)->getPiece()->getValidMoves())
                {
                    if (n.x >= 0 && n.x < b.boardDim && n.y >= 0 && n.y < b.boardDim)
                    {

                        if (n.type == CASTLING) {
                            if(n.y > j){
                                if(!isValidCheckMove(i,j,i,j+1)){
                                    curCoords.emplace_back(std::pair<int, int>{i, j});
                                    newCoords.emplace_back(std::pair<int, int>{n.x, n.y});
                                }
                            }else if(n.y < j){
                                if(!isValidCheckMove(i,j,i,j-1)){
                                    curCoords.emplace_back(std::pair<int, int>{i, j});
                                    newCoords.emplace_back(std::pair<int, int>{n.x, n.y}); 
                                }
                            }
                        }
                        
                        if (!isValidCheckMove(i, j, n.x, n.y))
                        {
                            curCoords.emplace_back(std::pair<int, int>{i, j});
                            newCoords.emplace_back(std::pair<int, int>{n.x, n.y});
                        }
                    }
                    else
                    {
                        b.getSquare(i, j)->getPiece()->deleteMove(n.x, n.y);
                    }
                }
            }
        }
    }
    for (int i = 0; i < curCoords.size(); i++)
    {
        b.getSquare(curCoords[i].first, curCoords[i].second)->getPiece()->deleteMove(newCoords[i].first, newCoords[i].second);
    }
}

bool Controller::isValidCheckMove(int x, int y, int newx, int newy)
{
    ChessPiece *tmp = b.getSquare(x, y)->getPiece();
    bool noError = true;

    if (tmp)
    {
        bool hasMoved = true;
        if (tmp->getType() == KING) {
            hasMoved = static_cast<King*>(tmp)->getMoved();
        }
        if (tmp->getType() == ROOK) {
            hasMoved = static_cast<Rook*>(tmp)->getMoved();
        }
        ChessPiece *dest = nullptr;
        if (b.getSquare(newx, newy)->getPiece())
        {
            if (b.getSquare(newx, newy)->getPiece()->getType() == PAWN)
            {
                dest = new Pawn(*(static_cast<Pawn *>(b.getSquare(newx, newy)->getPiece())));
            }
            else if (b.getSquare(newx, newy)->getPiece()->getType() == ROOK)
            {
                dest = new Rook(*(static_cast<Rook *>(b.getSquare(newx, newy)->getPiece())));
            }
            else if (b.getSquare(newx, newy)->getPiece()->getType() == BISHOP)
            {
                dest = new Bishop(*(static_cast<Bishop *>(b.getSquare(newx, newy)->getPiece())));
            }
            else if (b.getSquare(newx, newy)->getPiece()->getType() == KING)
            {
                dest = new King(*(static_cast<King *>(b.getSquare(newx, newy)->getPiece())));
            }
            else if (b.getSquare(newx, newy)->getPiece()->getType() == QUEEN)
            {
                dest = new Queen(*(static_cast<Queen *>(b.getSquare(newx, newy)->getPiece())));
            }
            else if (b.getSquare(newx, newy)->getPiece()->getType() == KNIGHT)
            {
                dest = new Knight(*(static_cast<Knight *>(b.getSquare(newx, newy)->getPiece())));
            }
        }

        b.makeMove(x, y, newx, newy);
        b.refreshLegalMoves();

        if (b.isChecked(turn))
        {
            noError = false;
        }

        b.getSquare(newx, newy)->setPiece(dest);
        b.getSquare(x, y)->setPiece(tmp);

        if(!hasMoved){
            if (tmp->getType() == KING) {
                static_cast<King*>(tmp)->setMoved(false);
            }
            if (tmp->getType() == ROOK) {
                static_cast<Rook*>(tmp)->setMoved(false);
            }
        }

        b.refreshLegalMoves();
        return noError;
    }
}

void Controller::makeMove(string initial, string dest, ostream &out, istream &in)
{
    int row = translateMove(initial).first;
    int col = translateMove(initial).second;
    int newRow = translateMove(dest).first;
    int newCol = translateMove(dest).second;
    b.refreshLegalMoves();
    filterValidMoves();

    ChessPiece *tmp = b.getSquare(row, col)->getPiece();

    if (tmp) // checking if there is a piece on that square
    {
        if (tmp->isLegalMove(newRow, newCol, turn)) // checking if the move is legal
        {

            if (b.getSquare(row, col)->getPiece()->getPieceChar() == 'p' && newRow == 7)
            {
                char newPiece;
                in >> newPiece;
                if (newPiece < 'a')
                {
                    newPiece += 32;
                }
                if (newPiece != 'k')
                {
                    b.getSquare(row, col)->setPiece(translate(newPiece));
                    b.makeMove(row, col, newRow, newCol);
                    toggleTurn();
                }
            }
            else if (b.getSquare(row, col)->getPiece()->getPieceChar() == 'P' && newRow == 0)
            {
                char newPiece;
                in >> newPiece;
                if (newPiece > 'Z')
                {
                    newPiece -= 32;
                }
                if (newPiece != 'K')
                {
                    b.getSquare(row, col)->setPiece(translate(newPiece));
                    b.makeMove(row, col, newRow, newCol);
                    toggleTurn();
                }
            }else if(b.getSquare(row, col)->getPiece()->getType() == PieceType::KING){
                if (col + 2 == newCol) {
                    b.makeMove(row,col,newRow,newCol);
                    b.makeMove(row, b.boardDim-1,newRow,col+1);
                } else if (col - 2 == newCol){
                    b.makeMove(row,col,newRow,newCol);
                    b.makeMove(row, 0,newRow,col-1);
                }
            }
            else //here
            {
                b.makeMove(row, col, newRow, newCol);
                toggleTurn();
            }

            b.refreshLegalMoves();
            filterValidMoves();
        }
        else
        {
            out << "Illegal Move" << endl;
        }
    }
    else
    {
        out << "No Piece there" << endl;
    }
    if (checkForCheckMate(out))
    {
    }
    else if (checkForCheck(out))
    {
    }
    else if (checkForStaleMate(out))
    {
    }
}

ChessPiece *Controller::translate(char c)
{
    switch (c)
    {
    case 'p':
        return new Pawn(Colour::Black);
    case 'P':
        return new Pawn(Colour::White);
    case 'b':
        return new Bishop(Colour::Black);
    case 'B':
        return new Bishop(Colour::White);
    case 'r':
        return new Rook(Colour::Black);
    case 'R':
        return new Rook(Colour::White);
    case 'k':
        return new King(Colour::Black);
    case 'K':
        return new King(Colour::White);
    case 'q':
        return new Queen(Colour::Black);
    case 'Q':
        return new Queen(Colour::White);
    case 'n':
        return new Knight(Colour::Black);
    case 'N':
        return new Knight(Colour::White);
    default:
        return nullptr;
    }
}

void Controller::toggleTurn()
{
    if (turn == Black)
    {
        turn = White;
    }
    else
    {
        turn = Black;
    }
}

void Controller::print(std::ostream &out) const { out << *td; }

void Controller::setup(std::istream &in, std::ostream &out)
{

    turn = Colour::White;
    b.emptyBoard();
    string cmd;
    while (in >> cmd)
    {
        if (cmd == "+")
        {
            char piece;
            string loc;
            cin >> piece >> loc;
            auto coords = translateMove(loc);
            b.getSquare(coords.first, coords.second)->setPiece(translate(piece));
        }
        else if (cmd == "-")
        {
            string loc;
            cin >> loc;
            auto coords = translateMove(loc);

            b.getSquare(coords.first, coords.second)->clearSquare();
        }

        else if (cmd == "=")
        {
            string colour;
            cin >> colour;

            if (colour == "black")
            {
                turn = Black;
            }
            else
            {
                turn = White;
            }
        }
        else if (cmd == "done")
        {
            int bKing = 0;
            int wKing = 0;
            bool PawnOnFirstOrLast = false;

            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    ChessPiece *tmp = b.getSquare(i, j)->getPiece();
                    if ((i == 0 || i == 7) && tmp)
                    {
                        if (tmp->getPieceChar() == 'p' || b.getSquare(i, j)->getPiece()->getPieceChar() == 'P')
                        {
                            PawnOnFirstOrLast = true;
                        }
                    }

                    if (tmp)
                    {
                        if (tmp->getPieceChar() == 'K')
                            wKing++;

                        if (tmp->getPieceChar() == 'k')
                            bKing++;
                    }
                }
            }

            if (PawnOnFirstOrLast)
            {
                out << "There is a Pawn on the first or last row" << endl;
            }

            else if (bKing != 1)
            {
                out << "There is not exactly one Black King" << endl;
            }

            else if (wKing != 1)
            {
                out << "There is not exactly one White King" << endl;
            }
            else if (b.isChecked(Black) || b.isChecked(White))
            {
                out << "Atleast one of the Kings is in check" << endl;
            }

            else
            {
                break;
            }
        }

        print(cout);
        b.refreshLegalMoves();
    }
}

bool Controller::checkForCheck(ostream &out)
{
    if (b.isChecked(Black))
    {
        out << "Black is in check" << endl;
        return true;
    }

    if (b.isChecked(White))
    {
        out << "White is in check" << endl;
        return true;
    }

    return false;
}

void Controller::makeMove(int x, int y, int newx, int newY)
{
    b.refreshLegalMoves();

    ChessPiece *tmp = b.getSquare(x, y)->getPiece();

    if (tmp) // checking if there is a piece on that square
    {

        if (tmp->isLegalMove(newx, newY, turn)) // checking if the move is legal
        {
            b.makeMove(x, y, newx, newY);
            b.refreshLegalMoves();
            toggleTurn();
        }
    }
}

bool Controller::checkForCheckMate(ostream &out)
{
    if (b.isChecked(Black) && b.hasNoMoves(Black))
    {
        out << "White Wins" << endl;
        return true;
    }

    if (b.isChecked(White) && b.hasNoMoves(White))
    {
        out << "Black Wins" << endl;
        return true;
    }

    return false;
}

bool Controller::checkForStaleMate(ostream &out)
{
    if (!b.isChecked(Black) && b.hasNoMoves(Black))
    {
        out << "Stalemate!" << endl;
        return true;
    }

    if (!b.isChecked(White) && b.hasNoMoves(White))
    {
        out << "Stalemate!" << endl;
        return true;
    }

    return false;
}

void Controller::resign(ostream &out) {
    if (turn == Black) {
        out << "Black resigned. White wins!" << endl;
    } else {
       out << "White resigned. Black wins!" << endl; 
    }
}
