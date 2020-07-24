#include <iostream>
#include <sstream>

using namespace std;

#include "node.h"
#include "matrix.h"

Node::Node()
{
}

Node::Node(int row, int col)
    : nodeIndex(Matrix::getNodeIndex(row, col)), contents(Piece::EMPTY)
{
}

Node::Node(int nodeIndex)
    : nodeIndex(nodeIndex), contents(Piece::EMPTY)
{
}

Node::Node(const Node& oldNode)
    : nodeIndex(oldNode.nodeIndex), contents(oldNode.contents)
{
}

Node::Node(int nodeIndex, Node::Piece p)
    : nodeIndex(nodeIndex), contents(p)
{
}

Node::~Node()
{
}

Node::Piece Node::other(Node::Piece p)
{
    if (p == Node::Piece::WHITE) return Node::Piece::BLACK;
    if (p == Node::Piece::BLACK) return Node::Piece::WHITE;
    return Node::Piece::EMPTY;
}

// returns name e.g. A5, B6)
string Node::getName(int row, int col)
{
    stringstream buffer;
    buffer << (char) (col + 'A') << row + 1;
    return buffer.str();
}

// get row column reference from name
// if error sets them to -1
void Node::getRC(const string& name, int& row, int& col)
{
    try
    {
        col = toupper(name[0]) - 'A';
        row = stoi(name.substr(1,string::npos)) - 1;
    }
    catch (...)
    {
        col = row = -1;
    }
}