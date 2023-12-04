#ifndef CONTROL_H
#define CONTROL_H
#include "board.h"
#include <string>
#include <iostream>
#include <utility>
#include <random>

enum PlayerType{Human, LV1, LV2, LV3, LV4};

class Controller
{   PlayerType whitePlayer;
    PlayerType blackPlayer;
    Board b;
    Colour turn;
    TextDisplay *td;
    GraphicsDisplay *gd;
    bool gameNotDone = true;
    bool setupMode = false;
    float whiteWins=0;
    float blackWins=0;
    vector <Square *> blackPieces;
    vector <Square *> whitePieces;
    ChessPiece *translate(char c); //turns a character into the corresponding ChessPiece
    PlayerType translatePlayer(string player);
    std::pair<int, int> translateMove(string str); //translates move from chess board notation to grid notation
    void toggleTurn();
    void print(std::ostream &out) const;
    void filterValidMoves();
    void simulateMoveForCheckOrCapture(int x, int y, int newx, int newy, Colour col, bool& start, vector <Square *> &enemyPieces);
    bool isValidCheckMove(int x, int y, int newx, int newy);
    bool checkForCheck(std::ostream &out);
    bool checkForCheckMate(std::ostream &out);
    bool checkForStaleMate(ostream &out);
    bool checksOtherPlayer(int x, int y, int newx, int newy);
    bool isUnderAttack(Square* piece, vector <Square *> &enemyPieces);
    bool canBeCaptured(int x, int y, int newx, int newy, vector <Square *> &enemyPieces);
    void makeMove(string initial, string dest, std::ostream &out, std::istream &in); 
    void makeMove(int row, int col, int newRow, int newCol, ostream &out, istream &in);
    void resign(std::ostream &out);
    pair<pair<int,int>,pair<int,int>> generateLV1Move(Colour c);
    pair<pair<int,int>,pair<int,int>> generateLV2Move(Colour c);
    pair<pair<int,int>,pair<int,int>> generateLV3Move(Colour c);
    pair<pair<int,int>,pair<int,int>> generateLV4Move(Colour c);
    
    public:
        Controller(Xwindow &w);
        
        void setup(std::istream &in, std::ostream &out);
        
        void startGame(std::istream &in, std::ostream &out);

        float getWhiteWins();

        float getBlackWins();

};
#endif