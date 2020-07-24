#pragma once
// Header file for creation of hex board for Coursera C++
#include <iostream>
#include <vector>

using namespace std;

#include "node.h"

class Game
{
    private:
        
    public:
        Game();
        ~Game();
        void initialize(int size);                 // initialize new board
        void displayBoard();                    // display created graph
        void computerMove(const Node::Piece player, string& move);
                                                // returns move in move
        bool playerMove(const Node::Piece player, string& move);
                                                // makes move in move
        static bool gameOver;
        static int size;
};