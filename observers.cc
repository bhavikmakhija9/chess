#include <vector>
#include "board.h"
#include "window.h"

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
            if(j != 7){
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


GraphicsDisplay::GraphicsDisplay(Xwindow &xw) : w(xw) {
 

 int colour = 1;

   for (int i=25; i < 425; i+=50) {
    for (int j = 0; j < 400; j+=50) {
        w.fillRectangle(i,j,50,50,colour);
        if (j != 350) {
            if (colour == 1) {
                colour = 0;
            } else {
                colour = 1;
            }
        }
    }
 } 

w.drawString(12,25,"8");
w.drawString(12,75,"7");
w.drawString(12,125,"6");
w.drawString(12,175,"5");
w.drawString(12,225,"4");
w.drawString(12,275,"3");
w.drawString(12,325,"2");
w.drawString(12,375,"1");

w.drawString(50,412,"a");
w.drawString(100,412,"b");
w.drawString(150,412,"c");
w.drawString(200,412,"d");
w.drawString(250,412,"e");
w.drawString(300,412,"f");
w.drawString(350,412,"g");
w.drawString(400,412,"h");

w.drawPiece(0,0,1,3,"./pieces/Rook.xbm");
w.drawPiece(1,0,0,3,"./pieces/Knight.xbm");
w.drawPiece(2,0,1,3,"./pieces/Bishop.xbm");
w.drawPiece(3,0,0,3,"./pieces/Queen.xbm");
w.drawPiece(4,0,1,3,"./pieces/King.xbm");
w.drawPiece(5,0,0,3,"./pieces/Bishop.xbm");
w.drawPiece(6,0,1,3,"./pieces/Knight.xbm");
w.drawPiece(7,0,0,3,"./pieces/Rook.xbm");

w.drawPiece(0,1,0,3,"./pieces/Pawn.xbm");
w.drawPiece(1,1,1,3,"./pieces/Pawn.xbm");
w.drawPiece(2,1,0,3,"./pieces/Pawn.xbm");
w.drawPiece(3,1,1,3,"./pieces/Pawn.xbm");
w.drawPiece(4,1,0,3,"./pieces/Pawn.xbm");
w.drawPiece(5,1,1,3,"./pieces/Pawn.xbm");
w.drawPiece(6,1,0,3,"./pieces/Pawn.xbm");
w.drawPiece(7,1,1,3,"./pieces/Pawn.xbm");

w.drawPiece(0,7,0,2,"./pieces/Rook.xbm");
w.drawPiece(1,7,1,2,"./pieces/Knight.xbm");
w.drawPiece(2,7,0,2,"./pieces/Bishop.xbm");
w.drawPiece(3,7,1,2,"./pieces/Queen.xbm");
w.drawPiece(4,7,0,2,"./pieces/King.xbm");
w.drawPiece(5,7,1,2,"./pieces/Bishop.xbm");
w.drawPiece(6,7,0,2,"./pieces/Knight.xbm");
w.drawPiece(7,7,1,2,"./pieces/Rook.xbm");

w.drawPiece(0,6,1,2,"./pieces/Pawn.xbm");
w.drawPiece(1,6,0,2,"./pieces/Pawn.xbm");
w.drawPiece(2,6,1,2,"./pieces/Pawn.xbm");
w.drawPiece(3,6,0,2,"./pieces/Pawn.xbm");
w.drawPiece(4,6,1,2,"./pieces/Pawn.xbm");
w.drawPiece(5,6,0,2,"./pieces/Pawn.xbm");
w.drawPiece(6,6,1,2,"./pieces/Pawn.xbm");
w.drawPiece(7,6,0,2,"./pieces/Pawn.xbm");

}

void GraphicsDisplay::notify(Square &s) {
    int row = s.getX();
    int col = s.getY();

    int x = (col * ss) + 25;
    int y = row * ss;

     if (s.getColour() == Colour::Black)
        {
          w.fillRectangle(x,y,50,50,0); 
        }
    else 
        {
            w.fillRectangle(x,y,50,50,1);
        }


    if (s.getPiece())
    {   
        string piece(1,s.getPiece()->getPieceChar());
        w.drawString(x+22,y+25,piece);
    }
}




 GraphicsDisplay::~GraphicsDisplay() {}