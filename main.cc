#include <string>
#include <iostream>
#include "controller.h"
using namespace std;

int main()
{   
    Xwindow w;
    Controller control(w);

    string cmd, initial, dest;

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

    
        cout << "Final Score: " << endl;
        cout << "White: " << control.getWhiteWins()  << endl;
        cout << "Black: " << control.getBlackWins()  << endl;
    
}