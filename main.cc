#include <string>
#include <iostream>
#include "controller.h"
using namespace std;

int main (){
    Controller control;

    int x, y, newx, newy;

    control.print(cout);
    
    while(cin >> x >> y >> newx >> newy) {
        control.makeMove(x,y,newx,newy);
        control.print(cout);
    }
    
}