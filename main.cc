#include <string>
#include <iostream>
#include "controller.h"
using namespace std;

int main()
{
    Controller control;

    string cmd, initial, dest;

    control.print(cout);

    while (cin >> cmd)
    {

        if (cmd == "game")
        {
        }

        else if (cmd == "move")
        {
            cin >> initial >> dest;
            control.makeMove(initial, dest, cout, cin);
            control.print(cout);
        }

        else if (cmd == "setup") {
           control.setup(cin, cout);
        }

        else {
            cout << "Invalid Command. Try Again" << endl;
        } 
    }
}