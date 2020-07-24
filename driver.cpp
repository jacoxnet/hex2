#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

#include "node.h"
#include "game.h"
#include "matrix.h"

Game myGame;

int main()
{
    cout << "Welcome to Hex" << endl;
    
    int bsize = 0;
    int sp = Matrix::INFINITE;
    while (bsize < 1 || bsize > 26)
    {
        cout << "Size of board to generate (1-26): ";
        cin >> bsize;
    }
    myGame.initialize(bsize);
    cout << "Okay, board of size " << bsize << " generated" << endl;
    myGame.displayBoard();
    cout << "You're White and move first. Computer is Black" << endl;
    while (!Game::gameOver)
    {
        string move;
        bool okay = false;
        while (!okay)
        {
            cout << "Your move: ";
            cin >> move;
            okay = myGame.playerMove(Node::Piece::WHITE, move);
        }
        if (Game::gameOver)
        {
            cout << "white wins" << endl;
            break;
        }
        myGame.computerMove(Node::Piece::BLACK, move);
        if (Game::gameOver)
        {
            cout << "black wins" << endl;
            break;
        }
        myGame.displayBoard();
    }    
    myGame.displayBoard();
    cout << "Thank you for playing" << endl;
    return 0;
}