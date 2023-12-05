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

    // Turns a character into the corresponding ChessPiece
    unique_ptr<ChessPiece> translate(char c);
    
    // Sets player type, human or computer level 1-4
    PlayerType translatePlayer(string player);

    // Translates move from chess board notation to grid notation
    std::pair<int, int> translateMove(string str);

    // Changes what player's turn it is
    void toggleTurn();

    // Removes all moves that would result in your own king being put in check
    void filterValidMoves();

    // Makes a move, checks for a condition manipulating start bool, depending on the size of enemypieces
    // If enemyPieces = 0, we want to see if the king of colour col is in check
    // Finally, it undos the move
    void simulateMoveForCheckOrCapture(int x, int y, int newx, int newy, Colour col, bool& start, vector <Square *> &enemyPieces);

    // Checks to see if the King is whose turn it is is under check
    bool checkForCheck(std::ostream &out);

    // Checks to see if the king whose turn it is is under a checkmate
    bool checkForCheckMate(std::ostream &out);
    // Checks to see if the game is in a stalemate
    bool checkForStaleMate(ostream &out);

    // Checks to see if the potential move will leave its own king in check
    bool isValidCheckMove(int x, int y, int newx, int newy);

    // Checks to see if the potential move will check the opposing king
    bool checksOtherPlayer(int x, int y, int newx, int newy);

    // Checks to see if the piece can be captured by one of the enemy pieces
    bool isUnderAttack(Square* piece, vector <Square *> &enemyPieces);

    // Checks to see if the piece at x and y can be captured by an enemy piece after moving to newx and newy
    bool canBeCaptured(int x, int y, int newx, int newy, vector <Square *> &enemyPieces);

    // Called when a human player makes a move
    void makeMove(string initial, string dest, std::ostream &out, std::istream &in); 

    // Called when a Computer player makes a move
    void makeMove(int row, int col, int newRow, int newCol, ostream &out, istream &in);

    // The player whose turn it is will resign and lose the game
    void resign(std::ostream &out);

    // Computer Level 1 will make random legal move, returns the coords of the piece and the coords of the move it will make
    pair<pair<int,int>,pair<int,int>> generateLV1Move(Colour c);
    // Computer Level 2 will prioritize moves that put the enemies king in check, capturing moves, 
    // and if none exist it will make a random legal move
    pair<pair<int,int>,pair<int,int>> generateLV2Move(Colour c);
    // Computer Level 3 will prioritize moves that will avoid capture, if no piece is in danger or can be in danger after a move,
    // it will prioritize moves that put the enemies king in check, capturing moves, and if none of those 
    // exist it will make a random legal move
    pair<pair<int,int>,pair<int,int>> generateLV3Move(Colour c);
    // Computer Level 4 will be a mix of level 2 and level 3 behaviours depending on the game state
    pair<pair<int,int>,pair<int,int>> generateLV4Move(Colour c);
    // Prints the Text Display of the board
    void print(std::ostream &out) const;
    public:
        Controller(Xwindow &w);
        
        // Allows for the game board before a new game, to set pieces wherever wanted
        // Cannot exist setup if current layout cna either king in check, or a stalemate
        void setup(std::istream &in, std::ostream &out);
        
        // Will start the game and play until a resignation, a checkmate, or a stalemate
        void startGame(std::istream &in, std::ostream &out);

        float getWhiteWins();

        float getBlackWins();

};
#endif