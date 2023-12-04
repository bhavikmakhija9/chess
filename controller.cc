#include "controller.h"
using namespace std;


// PUBLIC METHODS
// Method to be called in Main
Controller::Controller(Xwindow &w) : gd(new GraphicsDisplay(w)) , td(new TextDisplay), turn(White)
{
    b.defBoard();
    b.refreshLegalMoves();
    b.attachDisplay(td);
    b.attachDisplay(gd);
}

void Controller::setup(std::istream &in, std::ostream &out)
{

    turn = Colour::White;
    setupMode = true;
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
            b.notifyObservers();
        }
        else if (cmd == "-")
        {
            string loc;
            cin >> loc;
            auto coords = translateMove(loc);

            b.getSquare(coords.first, coords.second)->clearSquare();
            b.notifyObservers();
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
            else if (b.isChecked(Black) || b.isChecked(White))
            {
                out << "Atleast one of the Kings is in check" << endl;
            }
            else if (checkForStaleMate(out)) {

            }
            else
            {
                break;
            }
        }

        print(out);
        b.refreshLegalMoves();
    }
}

void Controller::startGame(std::istream &in, std::ostream &out) {
    string player1, player2, cmd;

    gameNotDone = true;

    in >> player1 >> player2;

    whitePlayer = translatePlayer(player1);

    blackPlayer = translatePlayer(player2);

    if (!setupMode) {
        b.defBoard();
        b.refreshLegalMoves();
        b.notifyObservers();
    }
    print(out);

    while (in >> cmd && gameNotDone) {
        if (cmd == "move")
        {
        PlayerType player = whitePlayer;
        if (turn == Black) {
            player = blackPlayer;
        }
            if (player == Human) {
            string initial, dest;
            in >> initial >> dest;
            makeMove(initial, dest, out, in);
       
         } else if (player == LV1) {
           pair<pair<int,int>,pair<int,int>> move = generateLV1Move(turn);
           makeMove(move.first.first, move.first.second, move.second.first, move.second.second,out,in);
         } else if (player == LV2) {
            pair<pair<int,int>,pair<int,int>> move = generateLV2Move(turn);
           makeMove(move.first.first, move.first.second, move.second.first, move.second.second,out,in);
         }else if (player == LV3){
            pair<pair<int,int>,pair<int,int>> move = generateLV3Move(turn);
           makeMove(move.first.first, move.first.second, move.second.first, move.second.second,out,in);
         }else if (player == LV4){
            pair<pair<int,int>,pair<int,int>> move = generateLV4Move(turn);
            makeMove(move.first.first, move.first.second, move.second.first, move.second.second,out,in);
         }
           print(out);
        }
         else if (cmd == "resign") {
            
            resign(out);
            break;
        }
    }
}

 float Controller::getWhiteWins() {
    return whiteWins;
 }

  float Controller::getBlackWins() {
    return blackWins;
 }


// PRIVATE METHODS

typedef bool (*colourchecked)(Colour);
typedef bool (*underAttack)(Square*, vector<Square*>&);

std::pair<int, int> Controller::translateMove(string str)
{
    int row = 8 - str[1] + '0';
    int col = str[0] - 'a';
    return std::pair<int, int>{row, col};
}

void Controller::filterValidMoves()
{
    vector<std::pair<int, int>> curCoords;
    vector<std::pair<int, int>> newCoords;
    for (int i = 0; i < b.boardDim; ++i)
    {
        for (int j = 0; j < b.boardDim; ++j)
        {
            if (b.getSquare(i, j)->getPiece())
            {
                for (auto n : *b.getSquare(i, j)->getPiece()->getValidMoves())
                {
                    if (n.x >= 0 && n.x < b.boardDim && n.y >= 0 && n.y < b.boardDim)
                    {

                        if (n.type == CASTLING) {
                            if(n.y > j){
                                if(!isValidCheckMove(i,j,i,j+1)){
                                    curCoords.emplace_back(std::pair<int, int>{i, j});
                                    newCoords.emplace_back(std::pair<int, int>{n.x, n.y});
                                }
                            }else if(n.y < j){
                                if(!isValidCheckMove(i,j,i,j-1)){
                                    curCoords.emplace_back(std::pair<int, int>{i, j});
                                    newCoords.emplace_back(std::pair<int, int>{n.x, n.y}); 
                                }
                            }
                        }
                        
                        if (!isValidCheckMove(i, j, n.x, n.y))
                        {
                            curCoords.emplace_back(std::pair<int, int>{i, j});
                            newCoords.emplace_back(std::pair<int, int>{n.x, n.y});
                        }
                    }
                    else
                    {
                        b.getSquare(i, j)->getPiece()->deleteMove(n.x, n.y);
                    }
                }
            }
        }
    }
    for (int i = 0; i < curCoords.size(); i++)
    {
        b.getSquare(curCoords[i].first, curCoords[i].second)->getPiece()->deleteMove(newCoords[i].first, newCoords[i].second);
    }
}

void Controller::simulateMoveForCheckOrCapture(int x, int y, int newx, int newy, Colour col, bool& start, vector <Square *> &enemyPieces) {
    ChessPiece *tmp = b.getSquare(x, y)->getPiece();

    if (tmp)
    {
        bool hasMoved = true;
        bool enPassant = false;

        if (tmp->getType() == KING) {
            hasMoved = static_cast<King*>(tmp)->getMoved();
        }
        if (tmp->getType() == ROOK) {
            hasMoved = static_cast<Rook*>(tmp)->getMoved();
        }

        ChessPiece *dest = nullptr;
        if(tmp->getType() == PAWN){
            Pawn *ourPawn = static_cast<Pawn*>(tmp);

            for(auto n: *ourPawn->getValidMoves()){
                if(n.type == ENPASSANT && n.x == newx && n.y == newy) {
                    enPassant = true;
                    dest = new Pawn(*(static_cast<Pawn *>(b.getSquare(x, newy)->getPiece())));
                }
            }
        }


        if (b.getSquare(newx, newy)->getPiece())
        {
            if (b.getSquare(newx, newy)->getPiece()->getType() == PAWN)
            {
                dest = new Pawn(*(static_cast<Pawn *>(b.getSquare(newx, newy)->getPiece())));
            }
            else if (b.getSquare(newx, newy)->getPiece()->getType() == ROOK)
            {
                dest = new Rook(*(static_cast<Rook *>(b.getSquare(newx, newy)->getPiece())));
            }
            else if (b.getSquare(newx, newy)->getPiece()->getType() == BISHOP)
            {
                dest = new Bishop(*(static_cast<Bishop *>(b.getSquare(newx, newy)->getPiece())));
            }
            else if (b.getSquare(newx, newy)->getPiece()->getType() == KING)
            {
                dest = new King(*(static_cast<King *>(b.getSquare(newx, newy)->getPiece())));
            }
            else if (b.getSquare(newx, newy)->getPiece()->getType() == QUEEN)
            {
                dest = new Queen(*(static_cast<Queen *>(b.getSquare(newx, newy)->getPiece())));
            }
            else if (b.getSquare(newx, newy)->getPiece()->getType() == KNIGHT)
            {
                dest = new Knight(*(static_cast<Knight *>(b.getSquare(newx, newy)->getPiece())));
            }
        }

        b.makeMove(x, y, newx, newy);
        b.refreshLegalMoves();


        if (enemyPieces.size() == 0) { // Looking for if king is checked
            if (b.isChecked(col))
            {
                start = !start;
            }
        } else { // Looking for if piece can be captured
            if (isUnderAttack(b.getSquare(newx, newy), enemyPieces)) {
                start = !start;
            }
        }


        if(enPassant){
            b.getSquare(x,newy)->setPiece(dest);
            static_cast<Pawn*>(dest)->setMovedTwo(true);
            b.getSquare(newx,newy)->setPiece(nullptr);
        }else{
            b.getSquare(newx, newy)->setPiece(dest);
        }
        
        b.getSquare(x, y)->setPiece(tmp);

        if(!hasMoved){
            if (tmp->getType() == KING) {
                static_cast<King*>(tmp)->setMoved(false);
            }
            if (tmp->getType() == ROOK) {
                static_cast<Rook*>(tmp)->setMoved(false);
            }
        }

        b.refreshLegalMoves();
    }
}

bool Controller::isValidCheckMove(int x, int y, int newx, int newy)
{
    bool noError = true;
    vector<Square*> nonApplicable;
    simulateMoveForCheckOrCapture(x, y, newx, newy, turn, noError, nonApplicable);

    return noError;
}

bool Controller::checksOtherPlayer(int x, int y, int newx, int newy)
{
    bool checksOther = false;
    Colour oppC = (turn ? Black : White);
    vector<Square*> nonApplicable;
    simulateMoveForCheckOrCapture(x, y, newx, newy, oppC, checksOther, nonApplicable);

    return checksOther;
}

bool Controller::isUnderAttack(Square* piece, vector <Square *> &enemyPieces) {
    for(auto m : enemyPieces){
        for(auto k : *m->getPiece()->getValidMoves()){
            if(k.type == CAPTURING && k.x == piece->getX() && k.y == piece->getY()){
                return true;
            }
        }
    }
    return false;
}

bool Controller::canBeCaptured(int x, int y, int newx, int newy, vector <Square *> &enemyPieces) {

    bool canBeCaptured = false;

    simulateMoveForCheckOrCapture(x, y, newx, newy, White, canBeCaptured, enemyPieces);

    return canBeCaptured;

}

void Controller::makeMove(string initial, string dest, ostream &out, istream &in)
{
    int row = translateMove(initial).first;
    int col = translateMove(initial).second;
    int newRow = translateMove(dest).first;
    int newCol = translateMove(dest).second;
   
   ChessPiece *tmp = b.getSquare(row, col)->getPiece();

    if (tmp) // checking if there is a piece on that square
    {
        for (auto n : *tmp->getValidMoves()) {

        }
        if (tmp->isLegalMove(newRow, newCol, turn)) // checking if the move is legal
        {

            if (b.getSquare(row, col)->getPiece()->getPieceChar() == 'p' && newRow == b.boardDim-1)
            {
                char newPiece;
                in >> newPiece;
                if (newPiece < 'a')
                {
                    newPiece += 32;
                }
                if (newPiece != 'k')
                {
                    b.getSquare(row, col)->setPiece(translate(newPiece));
                    b.makeMove(row, col, newRow, newCol);
                    toggleTurn();
                    b.refreshForEnPassant(turn);
                }
            }
            else if (b.getSquare(row, col)->getPiece()->getPieceChar() == 'P' && newRow == 0)
            {
                char newPiece;
                in >> newPiece;
                if (newPiece > 'Z')
                {
                    newPiece -= 32;
                }
                if (newPiece != 'K')
                {
                    b.getSquare(row, col)->setPiece(translate(newPiece));
                    b.makeMove(row, col, newRow, newCol);
                    toggleTurn();
                    b.refreshForEnPassant(turn);
                }
            }else if(b.getSquare(row, col)->getPiece()->getType() == PieceType::KING && col + 2 == newCol) {
                b.makeMove(row,col,newRow,newCol);
                b.makeMove(row, b.boardDim-1,newRow,col+1);
                toggleTurn();
                b.refreshForEnPassant(turn);
            } 
            else if (b.getSquare(row, col)->getPiece()->getType() == PieceType::KING && col - 2 == newCol) {
                b.makeMove(row,col,newRow,newCol);
                b.makeMove(row, 0,newRow,col-1);
                toggleTurn();
                b.refreshForEnPassant(turn);
            }
            else
            {
                bool enPassant = false;
                ChessPiece *ourPawn = b.getSquare(row, col)->getPiece();
                
                for(auto n: *ourPawn->getValidMoves()){
                    if(n.type == ENPASSANT && n.x == newRow && n.y == newCol) {
                        enPassant = true;
                    }
                }

                if(!enPassant){
                    b.makeMove(row, col, newRow, newCol);
                    toggleTurn();
                    b.refreshForEnPassant(turn);
                }else{
                    b.makeMove(row, col, newRow, newCol);
                    b.getSquare(row, newCol)->setPiece(nullptr);
                    toggleTurn();
                    b.refreshForEnPassant(turn);
                }
            }

            b.refreshLegalMoves();
            filterValidMoves();
            b.notifyObservers();
            
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
    if (checkForCheckMate(out))
    { 
        gameNotDone = false;
            if (turn == Black) {
            turn == White;
        }
        setupMode = false;
        
    }
    else if (checkForCheck(out))
    {
  
    }
    else if (checkForStaleMate(out))
    {
        gameNotDone = false;
        if (turn == Black) {
            turn == White;
        }
        setupMode = false;
    }
}

void Controller::makeMove(int row, int col, int newRow, int newCol, ostream &out, istream &in)
{
    b.refreshLegalMoves();
    filterValidMoves();
    
    ChessPiece *tmp = b.getSquare(row, col)->getPiece();

    if (tmp) // checking if there is a piece on that square
    {
        for (auto n : *tmp->getValidMoves()) {

        }
        if (tmp->isLegalMove(newRow, newCol, turn)) // checking if the move is legal
        {

            if (b.getSquare(row, col)->getPiece()->getPieceChar() == 'p' && newRow == b.boardDim-1)
            {
              
                
                    b.getSquare(row, col)->setPiece(translate('q'));
                    b.makeMove(row, col, newRow, newCol);
                    toggleTurn();
                    b.refreshForEnPassant(turn);
                }
            
            else if (b.getSquare(row, col)->getPiece()->getPieceChar() == 'P' && newRow == 0)
            {
                
                    b.getSquare(row, col)->setPiece(translate('Q'));
                    b.makeMove(row, col, newRow, newCol);
                    toggleTurn();
                    b.refreshForEnPassant(turn);
                
            }else if(b.getSquare(row, col)->getPiece()->getType() == PieceType::KING && col + 2 == newCol) {
                b.makeMove(row,col,newRow,newCol);
                b.makeMove(row, b.boardDim-1,newRow,col+1);
                toggleTurn();
                b.refreshForEnPassant(turn);
            } 
            else if (b.getSquare(row, col)->getPiece()->getType() == PieceType::KING && col - 2 == newCol) {
                b.makeMove(row,col,newRow,newCol);
                b.makeMove(row, 0,newRow,col-1);
                toggleTurn();
                b.refreshForEnPassant(turn);
            }
            else
            {
                bool enPassant = false;
                ChessPiece *ourPawn = b.getSquare(row, col)->getPiece();
                
                for(auto n: *ourPawn->getValidMoves()){
                    if(n.type == ENPASSANT && n.x == newRow && n.y == newCol) {
                        enPassant = true;
                    }
                }

                if(!enPassant){
                    b.makeMove(row, col, newRow, newCol);
                    toggleTurn();
                    b.refreshForEnPassant(turn);
                }else{
                    b.makeMove(row, col, newRow, newCol);
                    b.getSquare(row, newCol)->setPiece(nullptr);
                    toggleTurn();
                    b.refreshForEnPassant(turn);
                }
            }

            b.refreshLegalMoves();
            filterValidMoves();
            b.notifyObservers();
            
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
    if (checkForCheckMate(out))
    { 
        gameNotDone = false;
        setupMode = false;
    }
    else if (checkForCheck(out))
    {
  
    }
    else if (checkForStaleMate(out))
    {
        gameNotDone = false;
        setupMode = false;
    }
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

bool Controller::checkForCheck(ostream &out)
{
    if (b.isChecked(Black))
    {
        out << "Black is in check" << endl;
        return true;
    }

    if (b.isChecked(White))
    {
        out << "White is in check" << endl;
        return true;
    }

    return false;
}



bool Controller::checkForCheckMate(ostream &out)
{
    if (b.isChecked(Black) && b.hasNoMoves(Black))
    {
        out << "Checkmate!White Wins" << endl;
        whiteWins++;
        return true;
    }

    if (b.isChecked(White) && b.hasNoMoves(White))
    {
        out << "Checkmate!Black Wins" << endl;
        blackWins++;
     
        return true;
    }

    

    return false;
}

bool Controller::checkForStaleMate(ostream &out)
{
    if (!b.isChecked(Black) && b.hasNoMoves(Black))
    {
        out << "Stalemate!" << endl;
        whiteWins+=0.5;
        blackWins+=0.5;
        return true;
    }

    if (!b.isChecked(White) && b.hasNoMoves(White))
    {
        out << "Stalemate!" << endl;
        whiteWins+=0.5;
        blackWins+=0.5;
        return true;
    }



    bool insufficientPieces = false;
    pair<int, int> bishops {0, 0};
    pair<int, int> bishopSquareCol {0, 0};
    pair<int, int> rooks {0, 0};
    pair<int, int> queens {0, 0};
    pair<int, int> pawns {0, 0};
    pair<int, int> knights {0, 0};
    
    for (int i = 0; i < b.boardDim; i++) {
        for (int j = 0; j < b.boardDim; j++) {
            ChessPiece *temp = b.getSquare(i, j)->getPiece();
            if (temp) {
                switch (temp->getType()) {
                    case PAWN:
                        (temp->getColour()) ? ++pawns.first : ++pawns.second;
                        break;
                    case ROOK:
                        (temp->getColour()) ? ++rooks.first : ++rooks.second;
                        break;
                    case QUEEN:
                        (temp->getColour()) ? ++queens.first : ++queens.second;
                        break;
                    case KNIGHT:
                        (temp->getColour()) ? ++knights.first : ++knights.second;
                        break;
                    case BISHOP:
                        (temp->getColour()) ? ++bishops.first : ++bishops.second;
                        (b.getSquare(i ,j)->getColour()) ? ++bishopSquareCol.first : ++bishopSquareCol.second;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    if (rooks.first == 0 && rooks.second == 0 && queens.first == 0 && queens.second == 0 && pawns.first == 0 && pawns.second == 0) {
        if (bishops.first == 0 && bishops.second == 0 && knights.first == 0 && knights.second == 0) {
                // Case for king vs king
                insufficientPieces = true;
        } else if (((bishops.first == 1 && bishops.second == 0) || (bishops.first == 0 && bishops.second == 1)) 
                    && knights.first == 0 && knights.second == 0) {
                        // case for king and one bishop vs king
                        insufficientPieces = true;
        } else if (((knights.first == 1 && knights.second == 0) || (knights.first == 0 && knights.second == 1)) 
                    && bishops.first == 0 && bishops.second == 0) {
                        // case for king and one knight vs king
                        insufficientPieces = true;
        } else if (((knights.first == 2 && knights.second == 0) || (knights.first == 0 && knights.second == 2)) 
                    && bishops.first == 0 && bishops.second == 0) {
                        // case for king and two knight vs king
                        insufficientPieces = true;
        } else if (knights.first == 0 && knights.second == 0 && bishops.first == 1 && bishops.second == 1 
                    && ((bishopSquareCol.first == 2 && bishopSquareCol.second == 0) || 
                        (bishopSquareCol.first == 0 && bishopSquareCol.second == 2))) {
                        // case for king and bishop vs king and bishop on same colour square
                        insufficientPieces = true;

        }
    }
    if (insufficientPieces) {
        out << "Stalemate!" << endl;
    }

    return insufficientPieces;
}

void Controller::resign(ostream &out) {
    if (turn == Black) {
        out << "Black resigned. White wins!" << endl;
        whiteWins++;
        turn = White;
    } else {
       out << "White resigned. Black wins!" << endl; 
       blackWins++;
    }
    setupMode = false;
}

pair<pair<int,int>,pair<int,int>> Controller:: generateLV1Move(Colour c) {
  vector <Square *> &pieces = whitePieces;
  
  b.refreshLegalMoves();
  filterValidMoves();

  if (c == Black) 
  { pieces = blackPieces;
  } 
  


    pieces.clear();

    for (int i = 0; i < b.boardDim; ++i) {
             b.refreshLegalMoves();
              filterValidMoves();
        for (int j = 0; j <b.boardDim; ++j) {
            Square *tmp = b.getSquare(i,j);
               
            if(tmp->getPiece() && (tmp->getPiece()->getColour() == c) && (*tmp->getPiece()->getValidMoves()).size() != 0) {
                pieces.emplace_back(tmp);
            }
        }
    }
  
    
    srand(time(NULL));
    int random = rand()%pieces.size();

    Square * randomSquare = pieces.at(random);
    
    auto moves = *(randomSquare->getPiece()->getValidMoves());
    
    //srand(time(NULL));
    int random2 = rand()%moves.size(); //maybe messing up moving out of check

    Move randomMove = moves.at(random2);

   return {{randomSquare->getX(), randomSquare->getY()}, {randomMove.x, randomMove.y}};

} 


pair<pair<int,int>,pair<int,int>> Controller:: generateLV2Move(Colour c) {
  vector <Square *> &pieces = whitePieces;

  b.refreshLegalMoves();
  filterValidMoves(); 

  if (c == Black) 
  { 
    pieces = blackPieces;
  } 

    pieces.clear();

    bool noCheckingMove = true;

    bool noCapturingMove = true;

    int x,y,newX,newY;
        
    for (int i = 0 ; i < b.boardDim; ++i) {
        for (int j = 0; j <b.boardDim; ++j) {
            Square *tmp = b.getSquare(i,j);
            if(tmp->getPiece() && tmp->getPiece()->getColour() == c && (*tmp->getPiece()->getValidMoves()).size() != 0) {
                pieces.emplace_back(tmp);
            }
        }
    }

    for(auto square: pieces) {
          b.refreshLegalMoves();
          filterValidMoves();
       auto m = *(square->getPiece()->getValidMoves());
        for (auto move : m) {
             if(checksOtherPlayer(square->getX(), square->getY(), move.x, move.y)) {
                noCheckingMove = false;
                noCapturingMove = false;
                x = square->getX();
                y = square->getY();
                newX = move.x;
                newY = move.y;
             }
        }
     }

       if (noCheckingMove) {
        for(auto square: pieces) {
              b.refreshLegalMoves();
                filterValidMoves();
            auto m = *(square->getPiece()->getValidMoves());
            for (auto move : m) {
                if(move.type == CAPTURING || move.type == ENPASSANT) {
                noCapturingMove = false;
                x = square->getX();
                y = square->getY();
                newX = move.x;
                newY = move.y;
             }
        }
     }
     }

     if (noCheckingMove && noCapturingMove) {
       return generateLV1Move(c);
     }
     return {{x, y}, {newX, newY}};

}

pair<pair<int,int>,pair<int,int>> Controller:: generateLV3Move(Colour c) {
  vector <Square *> &pieces = whitePieces;
  vector <Square *> &enemyPieces = blackPieces;
  
  b.refreshLegalMoves();
  filterValidMoves(); 

  if (c == Black) 
  { 
    enemyPieces = whitePieces;
    pieces = blackPieces;
  } 

  pieces.clear();
  enemyPieces.clear();

  bool noDefendingMoves = true;
  int x, y, newX, newY;

  vector <Square *> gettingAttacked;

    for (int i = 0 ; i < b.boardDim; ++i) {
        for (int j = 0; j <b.boardDim; ++j) {
            Square *tmp = b.getSquare(i,j);
            if(tmp->getPiece() && tmp->getPiece()->getColour() == c && (*tmp->getPiece()->getValidMoves()).size() != 0) {
                pieces.emplace_back(tmp);
            }
            if(tmp->getPiece() && tmp->getPiece()->getColour() != c && (*tmp->getPiece()->getValidMoves()).size() != 0) {
                enemyPieces.emplace_back(tmp);
            }
        }
    }

    for(auto n : pieces){
        if (isUnderAttack(n, enemyPieces)) gettingAttacked.emplace_back(n);
    }

    if (gettingAttacked.size() != 0) {
        for (auto square: gettingAttacked) {
            b.refreshLegalMoves();
            filterValidMoves();
            auto m = *(square->getPiece()->getValidMoves());
            for (auto move : m) {
                if (!canBeCaptured(square->getX(), square->getY(), move.x, move.y, enemyPieces)) {
                    x = square->getX();
                    y = square->getY();
                    newX = move.x;
                    newY = move.y;
                    noDefendingMoves = false;
                }
            }
        }
    }else{
    
        for (auto square: pieces) {
            auto m = *(square->getPiece()->getValidMoves());
            b.refreshLegalMoves();
            filterValidMoves();
            for (auto move : m) {
                if (!canBeCaptured(square->getX(), square->getY(), move.x, move.y, enemyPieces)) {
                    x = square->getX();
                    y = square->getY();
                    newX = move.x;
                    newY = move.y;
                    noDefendingMoves = false;
                }
            }
        }
    }
    
    if (noDefendingMoves) {
        return generateLV2Move(c);
    }

    return {{x, y}, {newX, newY}};

}

pair<pair<int,int>,pair<int,int>> Controller:: generateLV4Move(Colour c) {
    vector <Square *> &pieces = whitePieces;
    vector <Square *> &enemyPieces = blackPieces;
  
    b.refreshLegalMoves();
    filterValidMoves(); 

    if (c == Black) 
    { 
        enemyPieces = whitePieces;
        pieces = blackPieces;
    } 

    pieces.clear();
    enemyPieces.clear();

    for (int i = 0 ; i < b.boardDim; ++i) {
        for (int j = 0; j <b.boardDim; ++j) {
            Square *tmp = b.getSquare(i,j);
            if(tmp->getPiece() && tmp->getPiece()->getColour() == c && (*tmp->getPiece()->getValidMoves()).size() != 0) {
                pieces.emplace_back(tmp);
            }
            if(tmp->getPiece() && tmp->getPiece()->getColour() != c && (*tmp->getPiece()->getValidMoves()).size() != 0) {
                enemyPieces.emplace_back(tmp);
            }
        }
    } //have updated values for where pieces are

    int x, y, newX, newY;

    vector <Square *> gettingAttacked;

    for(auto square: pieces) {
        b.refreshLegalMoves();
        filterValidMoves();
        auto m = *(square->getPiece()->getValidMoves());
        for (auto move : m) {
             if(checksOtherPlayer(square->getX(), square->getY(), move.x, move.y) 
                    && !canBeCaptured(square->getX(), square->getY(), move.x, move.y, enemyPieces)) {
                x = square->getX();
                y = square->getY();
                newX = move.x;
                newY = move.y;
                return {{x, y}, {newX, newY}};
             }
        }
     } //likes the idea of safe checking

     for(auto square: pieces) {
              b.refreshLegalMoves();
                filterValidMoves();
            auto m = *(square->getPiece()->getValidMoves());
            for (auto move : m) {
                if((move.type == CAPTURING || move.type == ENPASSANT) && !canBeCaptured(square->getX(), square->getY(), move.x, move.y, enemyPieces)) {
                x = square->getX();
                y = square->getY();
                newX = move.x;
                newY = move.y;
                return {{x, y}, {newX, newY}};
             }
        }
     } //likes the idea of safe capturing

    for(auto square: pieces) {
              b.refreshLegalMoves();
                filterValidMoves();
            auto m = *(square->getPiece()->getValidMoves());
            for (auto move : m) { //added bounce check here idk why but seems like its needed
                x = square->getX();
                y = square->getY();
                newX = move.x;
                newY = move.y;
                if((move.type == CAPTURING || move.type == ENPASSANT) && (newX >= 0 && newX < b.boardDim-1) && (newY >= 0 && newY < b.boardDim-1) &&
                    (b.getSquare(newX,newY)->getPiece()->getValue() >= b.getSquare(x,y)->getPiece()->getValue())) {
                return {{x, y}, {newX, newY}};
             }
        }
     } //likes the idea of trading similar or better material

    for(auto n : pieces){
        if (isUnderAttack(n, enemyPieces)) gettingAttacked.emplace_back(n);
    }

    if (gettingAttacked.size() != 0) {
        for (auto square: gettingAttacked) {
            b.refreshLegalMoves();
            filterValidMoves();
            auto m = *(square->getPiece()->getValidMoves());
            for (auto move : m) {
                if (!canBeCaptured(square->getX(), square->getY(), move.x, move.y, enemyPieces)) {
                    x = square->getX();
                    y = square->getY();
                    newX = move.x;
                    newY = move.y;
                    return {{x, y}, {newX, newY}};
                }
            }
        }
    } //likes the idea of not protecting material

    vector <pair<pair<int,int>,pair<int,int>>> safeMoves; //this vector keeps track of all moves

    for (auto square: pieces) {
            auto m = *(square->getPiece()->getValidMoves());
            b.refreshLegalMoves();
            filterValidMoves();
            for (auto move : m) {
                if (!canBeCaptured(square->getX(), square->getY(), move.x, move.y, enemyPieces)) {
                    x = square->getX();
                    y = square->getY();
                    newX = move.x;
                    newY = move.y;
                    pair<pair<int,int>,pair<int,int>> aMove = {{x, y}, {newX, newY}};
                    safeMoves.emplace_back(aMove);
                }
            }
        } //rather make a safe move than a dumb move

        if(safeMoves.size() != 0){
            srand(time(NULL));
            int random = rand()%safeMoves.size();
            return safeMoves[random];
        }

    return generateLV1Move(c); //found no good moves so might as well move randomly
}

PlayerType Controller::translatePlayer(string player) {
    if (player == "human") {
        return Human;
    } else if (player == "computer1") {
        return LV1;
    } else if (player == "computer2") {
        return LV2;
    } else if (player == "computer3"){
        return LV3;
    } else if (player == "computer4"){
        return LV4;
    }
}
