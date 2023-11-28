#include "controller.h"

Controller::Controller(): td{new TextDisplay} {
    b.newBoard();
    b.attachDisplay(td);
}

void Controller::makeMove(int x, int y, int newx, int newy) {b.makeMove(x, y, newx, newy);}

void Controller::print(std::ostream& out) const {out << *td;}