#include <string>
#include <iostream>
#include "board.h"

int main (){
    Board b;

    b.newBoard();


    int x, y, newx, newy;

    cout << b;
    
    while(cin >> x >> y >> newx >> newy) {
        b.makeMove(x,y,newx,newy);
        cout << b;
    }
    
}