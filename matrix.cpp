#include <iostream>
#include <algorithm>    // std::random_shuffle
#include <random>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

#include "node.h"
#include "matrix.h"
#include "game.h"

ostream& operator<<(ostream& os, const Node::Piece& p)
{
    switch (p)
    {
        case Node::Piece::BLACK:
            os << "Black";
            break;
        case Node::Piece::WHITE:
            os << "White";
            break;
        default:
            os << "Empty";
    }
    return os;
}

vector<list<int>> Matrix::edgeGraph;
vector<int> Matrix::wSourceList, Matrix::wTargetList, Matrix::bSourceList, Matrix::bTargetList;

// Default constructor for edgeGraph
// edgeGraph will need to be size * size
// to hold all nodes in one-dimensional vector
Matrix::Matrix(int size)
{
    Matrix::edgeGraph.resize(size * size);
    nodeList.resize(size * size);
}

Matrix::~Matrix()
{
}

void Matrix::copyMatrix(Matrix *pm)
{
    // copy nodeList
    for (int i = 0; i < Game::size * Game::size; i++)
    {
        pm->nodeList[i] = this->nodeList[i];
    }   
}

int Matrix::getNodeIndex(int row, int col)
{
    return row * Game::size + col;
}

void Matrix::getRC(int& row, int& col, int nodeIndex)
{
    row = nodeIndex / Game::size;
    col = nodeIndex % Game::size;
}

// removes connection if it exists from
// Node x to Node y
void Matrix::disconnect(int x, int y)
{
    // find y in x's list of outgoing connecting nodes
    for (auto it = Matrix::edgeGraph[x].begin(); it != Matrix::edgeGraph[x].end(); ++it)
    {
        if (*it == y)
        {
            it = Matrix::edgeGraph[x].erase(it);
        }
    }
    // erase symmetric connection, too
    for (auto it = Matrix::edgeGraph[y].begin(); it != Matrix::edgeGraph[y].end(); ++it)
    {
        if (*it == x)
        {
            it = Matrix::edgeGraph[y].erase(it);
        }
    }
}

// adds connection from Node x to Node y 
// first calls disconnect to delete any existing connection
// replace with this new one
void Matrix::connect(int x, int y)
{
    Matrix::disconnect(x, y);
    Matrix::edgeGraph[x].push_back(y);
    // connect symmetric nodes
    Matrix::edgeGraph[y].push_back(x);
}

// returns length of edge between node x and node y for player
// modified to return either INF or 0 if piece already there
int Matrix::getLength(int x, int y, Node::Piece player)
{
    for (auto it = Matrix::edgeGraph[x].begin(); it != Matrix::edgeGraph[x].end(); ++it)
    {
        if (*it == y)
        {
            // both have our tokens - return 0
            if ((nodeList[x].contents == player) && (nodeList[y].contents == player))
            {
                return 0;
            }
            else
            {
                return INFINITE;
            }
        }
    // didn't find - no connection
    }
    return INFINITE;
}

bool Matrix::isSource(int nodeIndex, Node::Piece player)
{    
    if (player == Node::Piece::BLACK)
    {
        return count(Matrix::bSourceList.begin(), Matrix::bSourceList.end(), nodeIndex) > 0;
    }
    if (player == Node::Piece::WHITE)
    {
        return count(Matrix::wSourceList.begin(), Matrix::wSourceList.end(), nodeIndex) > 0;
    }
    return false;
}

bool Matrix::isTarget(int nodeIndex, Node::Piece player)
{
    if (player == Node::Piece::BLACK)
    {
        return count(Matrix::bTargetList.begin(), Matrix::bTargetList.end(), nodeIndex) > 0;
    }
    if (player == Node::Piece::WHITE)
    {
        return count(Matrix::wTargetList.begin(), Matrix::wTargetList.end(), nodeIndex) > 0;
    }
    return false;
}

list<int> Matrix::getNeighbors(int nodeIndex)
{
    return Matrix::edgeGraph[nodeIndex];
}

vector<int> *Matrix::getEmptyNodes()
{
    vector<int> *emptyNodes = new vector<int>;
    for (auto it = nodeList.begin(); it != nodeList.end(); ++it)
    {
        if (it->contents == Node::Piece::EMPTY)
        {
            emptyNodes->push_back(it->nodeIndex);
        }
    }
    return emptyNodes;
}

double Matrix::evalPosition(int nodeIndex, Node::Piece player)
{
    // number of times to repeat
    const int repeat = 1000;
    int winCount = 0;
    Matrix *testM = new Matrix(Game::size);
    vector<int> *emptyNodes;
    // repeat this task repeat times
    for (int i = 0; i < repeat; i++)
    {
        this->copyMatrix(testM);
        // make the move to be evaluated on the test matrix
        testM->addPiece(nodeIndex, player);
        // get all the empty nodes of Test
        emptyNodes = testM->getEmptyNodes();
        // randomly shuffle
        random_shuffle(emptyNodes->begin(), emptyNodes->end());
        // start with other player
        Node::Piece p = Node::other(player);
        // play all empty nodes alternating players
        for (auto it = emptyNodes->begin(); it != emptyNodes->end(); ++it)
        {
            testM->addPiece(*it, p);
            p = Node::other(p);
        }
        // if shortest path is 0 we win
        if (testM->findSPath(player) == 0)
        {
            winCount++;
        }
        emptyNodes->clear();
    }
    delete emptyNodes;
    delete testM;
    return ((double) winCount) / repeat;
}

string Matrix::getName(int nodeIndex)
{
    int row, column;
    Matrix::getRC(row, column, nodeIndex);
    return Node::getName(row, column);
}

void Matrix::initializeNewGraph()
{
    // populate connections 
    for (int row = 0; row < Game::size; row++)
    {
        for (int col = 0; col < Game::size; col++)
        {
            // create entry for nodeList
            nodeList[getNodeIndex(row, col)] = Node(getNodeIndex(row, col), Node::Piece::EMPTY);
            // now establish connections to other squares
            if (row > 0)
            {
                connect(getNodeIndex(row, col), getNodeIndex(row - 1, col));
                if (col < Game::size - 1)
                {
                    connect(getNodeIndex(row, col), getNodeIndex(row - 1, col + 1));
                }
            }
            if (col > 0)
            {
                connect(getNodeIndex(row, col), getNodeIndex(row, col - 1));
                if (row < Game::size - 1)
                {
                    connect(getNodeIndex(row, col), getNodeIndex(row + 1, col - 1));
                }
            }
            if (col < Game::size - 1)
            {
                connect(getNodeIndex(row, col), getNodeIndex(row, col + 1));
            }
            if (row < Game::size - 1)
            {
                connect(getNodeIndex(row, col), getNodeIndex(row + 1, col));
            }
        }
    }
    // initialize source and target lists
    for (int i = 0; i < Game::size; i++)
    {
        // black's sources are node indexes for row, col (0, 0), (0, 1), (0, 2) ...
        Matrix::bSourceList.push_back(getNodeIndex(0, i));
        Matrix::bTargetList.push_back(getNodeIndex(Game::size - 1, i));
    
        // white's sources are node indexes for row, col (0, 0), (1, 0), (2, 0) ...
        Matrix::wSourceList.push_back(getNodeIndex(i, 0));
        Matrix::wTargetList.push_back(getNodeIndex(i, Game::size - 1));
    }
}

// add a piece and return true or false if successful
bool Matrix::addPiece(int nodeIndex, Node::Piece p)
{
    // if square is not empty
    if (nodeList[nodeIndex].contents != Node::Piece::EMPTY)
    {
        return false;
    }
    else
    {
        nodeList[nodeIndex].contents = p;
        return true;
    }
}

// return contents of a square
Node::Piece Matrix::getPiece(int nodeIndex)
{
    return nodeList[nodeIndex].contents;
}

void Matrix::displayGraph()
{
    for (int i = 0; i < Game::size; i++)
    {
        cout << "node " << Matrix::getName(i) << ", neighbors ";
        list<int> n = Matrix::getNeighbors(i);
        for (auto it = n.begin(); it != n.end(); ++it)
        {
            cout << getName(*it) << " ( " << nodeList[*it].contents << ") ";
        }
        cout << endl;
    }
}

// find a path if exists return 0 else INFINITE
int Matrix::findSPath(Node::Piece player)
{
    vector<int> *ps;
    if (player == Node::Piece::BLACK)
    {
        ps = &bSourceList;
    }
    else
    {
        ps = &wSourceList;
    }
    int shortestPath = INFINITE;
    // iterate through sources
    for (int sourceCount = 0; sourceCount < Game::size; sourceCount++)
    {
        list<int> OpenList, ClosedList;             // Open holds vertices that haven't been visited
                                                   // Closed that have
        // put initial source on OpenList
        OpenList.push_back((*ps)[sourceCount]);
        // keep examining nodes until OpenList empty
        while (!OpenList.empty())
        {
            // get front element in OpenList 
            int u = OpenList.front();
            // remove the vertex with that index from Open
            OpenList.remove(u);
            // if we've retrieved target, we're done
            // removed code
            // Otherwise add to closed
            ClosedList.push_back(u);
            // now iterate through neighbors of index
            list<int> n = getNeighbors(u);
            // pv iterates through neighbors
            for (auto pv = n.begin(); pv != n.end(); ++pv)
            {
                // only do if not disconnect
                if (Matrix::getLength(u, *pv, player) != INFINITE)
                {
                    // if target we're done!
                    if (isTarget(*pv, player))
                    {
                        OpenList.clear();
                        shortestPath = 0;
                        break;
                    }
                    // only do this for nodes not in Closed or Open
                    if (count(ClosedList.begin(), ClosedList.end(), *pv) == 0) 
                    {
                        // add to open if not already on list
                        if (count(OpenList.begin(), OpenList.end(), *pv) == 0)
                        {
                            OpenList.push_back(*pv);
                        }
                    }
                }
            }
        }
    }
    return shortestPath;
}
