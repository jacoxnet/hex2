#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "node.h"
#include "game.h"
#include "matrix.h"

Matrix *boardGraph;

// Default constructor & destructor
Game::Game()
{
}

Game::~Game()
{
}

bool Game::gameOver;
int Game::size;

void Game::initialize(int size)
{
    Game::size = size;
    boardGraph = new Matrix(size); 
    boardGraph->initializeNewGraph();
    gameOver = false;
}

// make player move - move is a string of form D3 (col D row 3)
// returns true if valid and move is made else false
bool Game::playerMove(const Node::Piece player, string& move)
{
    int row, col;
    // convert to row, column
    Node::getRC(move, row, col);
    // check if valid
    if (row == -1)
    {
        return false;
    }
    // return false if can't make move
    if (!boardGraph->addPiece(Matrix::getNodeIndex(row, col), player))
    {
        return false;
    }
    // move made
    cout << "Player move (" << player << ") " << move << endl;
    int sp = boardGraph->findSPath(player);
    // cout << "  (shortest path is now " << sp << ")" << endl;
    if (sp == 0)
    {
        Game::gameOver = true;
    }
    return true;
}

void Game::computerMove(const Node::Piece player, string& move)
{
    // nI and score will accummulate our best result
    int nI;
    double score = 0.0;
    // go through whole board to evaluate moves
    // for all empty squares
    for (int n = 0; n < Game::size * Game::size; n++)
    {
        if (boardGraph->getPiece(n) == Node::Piece::EMPTY)
        {
            // if empty, evaluate move
            double eval = boardGraph->evalPosition(n, player);
            cout << "node " << n << " score " << eval << endl;
            if (eval > score)
            {
                score = eval;
                nI = n;
            }
        }
    }
    boardGraph->addPiece(nI, player);
    move = Matrix::getName(nI);
    cout << "Computer move (" << player << ") " << move << endl;
    score = boardGraph->findSPath(player);
    // cout << "  (shortest path is now " << score << ")" << endl;
    if (score == 0)
    {
        Game::gameOver = true;
    }
}

void Game::displayBoard()
{
    // col header
    cout << "   ";
    for (int head = 0; head < Game::size; head++)
    {
        cout << (char) (65 + head) << "   ";
    }
    cout << "\n";
    for (int row = 0; row < Game::size; row++)
    {
        // these are the two strings we'll build up 
        // for printing each row - 1 line of nodes 1 of links
        string nodeLine = "";
        string linkLine = "";
        // indent to start lines
        nodeLine.string::append(2*row, ' ');
        linkLine.string::append(2*row + 4, ' ');
        // row header - (2 for num plus space)
        string s = to_string(row + 1);
        nodeLine.string::append(2-s.string::length(), ' ').string::append(s).string::append(" ");
        for (int col = 0; col < Game::size; col++)
        {
            if (col > 0)
            {
                nodeLine.append(" - ");
                linkLine.string::append("\\ / ");
            }
            Node::Piece content = boardGraph->getPiece(Matrix::getNodeIndex(row, col));
            if (content == Node::Piece::EMPTY)
            {
                nodeLine.string::append(".");
            }
            else if (content == Node::Piece::BLACK)
            {
                nodeLine.string::append("B");
            }
            else 
            {
                nodeLine.string::append("W");
            }   
        }
        cout << nodeLine << '\n';
        if (row + 1 < Game::size)           // don't print last line of links if at end
        {
            linkLine.string::append("\\");         // need a final link at end
            cout << linkLine << '\n';
        }
    }
}