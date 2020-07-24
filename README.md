# HEX2

This C++ console program implements the game of Hex. It allows the user to specify
the size of the board (1-26), and then the user plays white while the computer
plays black.

This implementation uses a Monte Carlo simulation for determing the computer's move.
For each possible move, it repeatedly plays random games until the board is full and a
winner is determined. It chooses the move with the best winning percentage in these
simulations.

With a large enough number of repetitions, this algorithm plays surprisingly well. Even
at only 1000 repetitions, it plays well enough to consistently beat a beginner.

This program was written for Coursera's C++ for C Programmers, Part B.

## Files

- *driver.cpp Main function. Gets the size of the board and inputs player moves. Calls methods in the HexBoard class implement functionality of the game.
- *game.cpp*, *game.h* Implements a simple representation of the hex board using a two-dimensional graph. Methods exist to make player moves, computer moves, and display the board.
- *matrix.cpp*, *matrix.h* Implements the underlying functional representation of the board as a graph whose nodes are edges from one playing square to another. Includes the functionality implementing the Dijkstra shortest-path algorithm (variant) to determine the winner, and the Monte Carlo evaluation process. The `const int repeat` in `Matrix::evalPosition` determines the number of repetitions.
- *node.cpp*, *node.h*. Class to represent a node of the graph (which is an edge from one square to another).
