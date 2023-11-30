#include "controller.h"

using namespace std;

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

void Controller::makeMove(string initial, string dest, ostream &out)
{
    int row = translateMove(initial).first;
    int col = translateMove(initial).second;
    int newRow = translateMove(dest).first;
    int newCol = translateMove(dest).second;
    b.refreshLegalMoves();

    ChessPiece *tmp = b.getSquare(row, col)->getPiece();

    if (tmp) // checking if there is a piece on that square
    {

        if (tmp->isLegalMove(newRow, newCol, turn)) // checking if the move is legal
        {
            b.makeMove(row, col, newRow, newCol);
            b.refreshLegalMoves();
            toggleTurn();
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
    checkForCheck(out);
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

            else
            {
                break;
            }
        }

        print(cout);
        b.refreshLegalMoves();
    }
}

void Controller::checkForCheck(ostream &out)
{
    if (b.isChecked(Black))
    {
        out << "Black is in check" << endl;
    }

    if (b.isChecked(White))
    {
        out << "White is in check" << endl;
    }
}