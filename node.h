#pragma once

#include <iostream>
#include <string>

using namespace std;

class Node
{
    public:
        enum class Piece
        {
            WHITE,
            BLACK,
            EMPTY
        };
        int nodeIndex;
        Piece contents;
        Node();
        ~Node();
        Node(int row, int col);
        Node(int nodeIndex);
        Node(int nodeIndex, Piece p);
        Node (const Node& oldNode);
        friend ostream& operator<<(ostream& os, const Piece& p);
        bool operator==(const Node& n) const { return this->nodeIndex == n.nodeIndex; }
        bool operator!=(const Node& n) const { return !operator==(n); }
        static Piece other(Piece p);
        static string getName(int row, int col);
                                        // converts row, col to A4
        static void getRC(const string& name, int& row, int& col);
                                        // converts A4 to row, col (passed by ref)
}; 