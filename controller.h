#include "board.h"

class Controller {
    Board b;
    TextDisplay* td;
    public:
        Controller();
        void makeMove(int x, int y, int newx, int newy);
        void print(std::ostream& out) const;
};