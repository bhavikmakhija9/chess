#include <vector>
#include "board.h"

using namespace std;

TextDisplay::TextDisplay()
{
    theDisplay = vector<vector<char>>(8, vector<char>(8, '_'));

    char curr = ' ';

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            theDisplay[i][j] = curr;

            if (curr == '_') // change after
            {
                curr = ' ';
            }
            else
            {
                curr = '_';
            }
        }
    }

    theDisplay[0][0] = 'r';
    theDisplay[0][1] = 'n';
    theDisplay[0][2] = 'b';
    theDisplay[0][3] = 'q';
    theDisplay[0][4] = 'k';
    theDisplay[0][5] = 'b';
    theDisplay[0][6] = 'n';
    theDisplay[0][7] = 'r';

    for (int i = 0; i < 8; ++i)
    {
        theDisplay[1][i] = 'p';
        theDisplay[6][i] = 'P';
    };

    theDisplay[7][0] = 'R';
    theDisplay[7][1] = 'N';
    theDisplay[7][2] = 'B';
    theDisplay[7][3] = 'Q';
    theDisplay[7][4] = 'K';
    theDisplay[7][5] = 'B';
    theDisplay[7][6] = 'N';
    theDisplay[7][7] = 'R';
}

void TextDisplay::notify(Square &s)
{
    int x = s.getX();
    int y = s.getY();

    if (s.isEmpty())
    {
        if (s.getColour() == Colour::Black)
        {
            theDisplay[x][y] = '_';
        }
        else if (s.getColour() == Colour::White)
        {
            theDisplay[x][y] = ' '; // change after
        }
    }

    else
    {
        theDisplay[x][y] = s.getPiece()->getPieceChar();
    }
}

TextDisplay::~TextDisplay() {}

ostream &operator<<(ostream &out, const TextDisplay &td)
{   out << "\n";
    for (int i = 0; i < 8; ++i)
    {
        out << 8 - i << " ";
        for (int j = 0; j < 8; ++j)
        {
            out << td.theDisplay[i][j];
        }
        out << endl;
    }
    out << "\n";
    out << "  "
        << "abcdefgh" << endl;
    out << "\n";
    return out;
}
