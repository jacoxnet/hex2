#pragma once
#include <vector>
#include <list>
#include <string>

using namespace std;

#include "node.h"

class Matrix
{
    private:
        friend class Node;
        static vector<list<int>> edgeGraph;  // holds connection graph
        vector<Node> nodeList;           // holds nodes
        static vector<int> wSourceList, wTargetList, bSourceList, bTargetList;
                                    // source and target lists for players
        void disconnect(int x, int y);
        void connect(int x, int y);
        bool isSource(int NodeIndex, Node::Piece player);
        bool isTarget(int NodeIndex, Node::Piece player);
        list<int> getNeighbors(int nodeIndex);
        int getLength(int x, int y, Node::Piece player);
                                    // get length of edge depending on what
                                    // pieces are in x and y
        vector<int> *getEmptyNodes();        // return pvector of all empty nodeIndexes
        
    public:
        Matrix(int size);
        ~Matrix();
        void copyMatrix(Matrix *pm);            // copies to new matrix *pm
        void initializeNewGraph();         // sets up initial connections on blank hexboard
                                                // and init source list
        bool addPiece(int nodeIndex, Node::Piece player); // add piece to boeard
        Node::Piece getPiece(int nodeIndex);      // return contents
        void displayGraph();                // display edge graph
        double evalPosition(int NodeIndex, Node::Piece player); //evaluate position double E (0, 1)
        int findSPath(Node::Piece player);             // find a path from any source to a target
                                            // by applying modified Dijkstra algorithm
                                            // returns 0 or INFINITE
        static const int INFINITE = 0x3F3F3F3F;
        static int getNodeIndex(int row, int col);
        static void getRC(int& row, int& col, int nodeIndex);
        static string getName(int nodeIndex);
};