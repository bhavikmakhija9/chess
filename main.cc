#include <string>
#include <iostream>
#include "controller.h"
using namespace std;

int main()
{   
    Xwindow xw;
    int colour = 1;

xw.drawString(12,25,"8");
xw.drawString(12,75,"7");
xw.drawString(12,125,"6");
xw.drawString(12,175,"5");
xw.drawString(12,225,"4");
xw.drawString(12,275,"3");
xw.drawString(12,325,"2");
xw.drawString(12,375,"1");
xw.drawString(50,412,"a");
xw.drawString(100,412,"b");
xw.drawString(150,412,"c");
xw.drawString(200,412,"d");
xw.drawString(250,412,"e");
xw.drawString(300,412,"f");
xw.drawString(350,412,"g");
xw.drawString(400,412,"h");


for (int i=25; i < 425; i+=50) {
    for (int j = 0; j < 400; j+=50) {
        xw.fillRectangle(i,j,50,50,colour);
        if (j != 350) {
            if (colour == 1) {
                colour = 0;
            } else {
                colour = 1;
            }
        }
    }
 }

    Controller control;

    string cmd, initial, dest;

    control.print(cout);

    while (cin >> cmd)
    {

        if (cmd == "game")
        { 
            control.startGame(cin, cout);
        }

        else if (cmd == "setup") {
           control.setup(cin, cout);
        }

        else {
            cout << "Invalid Command. Try Again" << endl;
        } 
    }
}