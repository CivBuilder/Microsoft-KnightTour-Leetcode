/// @file knightType.h
/// @author Chris Catechis
/// @date 2/9/2022
/// @brief This program creates/implements the knightType class.
/// @note I did reference cppreference for std::sort and the vector class.

#ifndef KNIGHTTYPE_HPP_
#define KNIGHTTYPE_HPP_

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>

class knightType
{
public:
    struct position {
        position(int r = 0, int c = 0, int o = 0) {
        row = r;
        col = c;
        onwardMoves = o; 
        }

        int row;
        int col;
        int onwardMoves;
    };
    /// constructor that allocates a dim x dim vector to the board
    /// object and sets each element to 0, sets functionsCalled to 0
    /// @param dim dimensions of the board to be constructed. default = 8.
    knightType(int dim = 8);

    /// Initial call to rescurive function.
    /// @param r row coordinate
    /// @param c column coordinate
    /// @returns true if this is the first move.
    bool knightTour(int r, int c) { return knightTour(r, c, 1); }

    /// outputs the current chess board.
    void outputTour() const;

    private:
        /// Recursive function for knight movement.
        /// @param r row coordinate
        /// @param c column coordinate
        /// @param tourIndex current move in the tour.
        /// @returns true when tour is done.
        bool knightTour(int r, int c, int tourIndex);

        /// Checks moves available from the current coordinate as well as the
        /// moves from the potential next coordinate.
        /// @param r row coordinate
        /// @param c column coordinate
        /// @returns list of moves available from current and next positions.
        std::vector<position> getAvailableMoves(int r, int c);

        /// HELPER FUNCTION for getAvailableMoves, checks if move is legal.
        /// @param r move's row coordinate.
        /// @param c move's column coordinate.
        /// @returns true if move is legal.
        bool goodMove(int r, int c);

        /// Checks if the board is full, meaning no 0's are on the board.
        /// @returns true if the board is full.
        bool fullBoard();

        // private objects
        std::vector<std::vector<int>> board;  // chess board
        int functionsCalled;  // # of times knightTour has been called.
};

/// ---------------------------------------------------------------------------
/// knightType function definitions
/// ---------------------------------------------------------------------------

/// constructor that allocates a dim x dim vector to the board
/// object and sets each element to 0, sets functionsCalled to 0
/// @param dim dimensions of the board to be constructed. default = 8.
knightType::knightType(int dim) {
    board.resize(dim);
    for (int i = 0; i < dim; i++) {
        board[i].resize(dim);
    }
    functionsCalled = 0;
}

/// ---------------------------------------------------------------------------

/// outputs the current chess board.
void knightType::outputTour() const {
    int dim = board.size();  // used for iteration
    char colHeader = 'A';  // chars for columns
    char rowHeader = 'A';  // chars for rows

    // print out header
    std::cout << "    ";  // accounts for 0 in array
    for (int i = 0; i < dim; i++)  // horizontal setup
    {
        std::cout << std::setw(3) << colHeader;
        colHeader++;
    }
    std::cout << '\n';


    for (int i = 0; i < dim; ++i) {
        // print row header
        std::cout << std::setw(4) << rowHeader;
        rowHeader++;

        // print current position
        for (int j = 0; j < dim; ++j) {
            std::cout << std::setw(3) << board[i][j];
        }
        std::cout << '\n';
    }

    std::cout << '\n' <<"Functions called: " << functionsCalled << "\n\n";
}

/// ---------------------------------------------------------------------------
/// knightType Private members
/// ---------------------------------------------------------------------------

/// Recursive function for knight movement.
/// @param r row coordinate
/// @param c column coordinate
/// @param tourIndex current move in the tour.
/// @returns true when tour is done.
bool knightType::knightTour(int r, int c, int tourIndex) {
    std::vector<knightType::position> movesAvailable;  // vector returned from getAvailableMoves
    knightType::position nextMove;  // best move we can make
    knightType::position backupMove;  // in case nextMove returns false.
    size_t index = 0;  // current move

    // routine board upkeep.
    ++functionsCalled;
    board[r][c] = tourIndex;

    // base case
    if (fullBoard() == true) {
        return true;
    }

    movesAvailable = getAvailableMoves(r, c);

    // don't allow a move to go that would end the game.
    if (movesAvailable.size() == 0) {
        --functionsCalled;  // delete function call
        board[r][c] = 0;  // reset current square
        return false;
    }

    else { 
        // sort movesAvailable
        std::sort(movesAvailable.begin(), movesAvailable.end(), [](position a, position b) {
            return a.onwardMoves < b.onwardMoves;
        });

        // recursive call
        if (knightTour(movesAvailable[index].row, movesAvailable[index].col, 
                        tourIndex + 1) == true) {
            return true;
        }
        
        // // just in case recursion fails
        while (knightTour(movesAvailable[index].row, movesAvailable[index].col, 
                        tourIndex + 1) == false){
            index++;
        }

        if (index == movesAvailable.size()) { // index = size
            return false;
        }

        else {
            return true;
        }
    }
}

/// ---------------------------------------------------------------------------

/// Checks moves available from the current coordinate as well as the
/// moves from the potential next coordinate.
/// @param r row coordinate
/// @param c column coordinate
/// @returns list of moves available from current and next positions.
std::vector<knightType::position> knightType::getAvailableMoves(int r, int c) {
    const int SIZE = 8;  // size of modifier arrays
    std::vector<knightType::position> availableMoves;  // coordinate(s) of available moves
    int newRow;  // row coordinate for new move.
    int newCol;  // column coordinate for new move.
    int concurrentRow;  // row stemming from new row.
    int concurrentCol;  // col stemming from new col

    // row/column position kernels
    int rowModifier[SIZE] = {-1, -1, -2, -2, 1, 1, 2, 2};
    int colModifier[SIZE] = {-2, 2, -1, 1, -2, 2, -1, 1};

    for (int i = 0; i < SIZE ; ++i) {
    // iterate through modifiers, checking for good moves.
        newRow = r + rowModifier[i];  // set new positions.
        newCol = c + colModifier[i];
        if (goodMove(newRow, newCol)) {
            position newMove(newRow, newCol, 0);
            // check moves from newMove.
            for (int j = 0; j < SIZE; ++j) {
                concurrentRow = newRow + rowModifier[j];  // set new positions.
                concurrentCol = newCol + colModifier[j];
                if (goodMove(concurrentRow, concurrentCol)) {
                    ++newMove.onwardMoves;
                }
            }
            availableMoves.push_back(newMove);  // push new move onto the vector.
        }
    }

    return availableMoves;
}

/// ---------------------------------------------------------------------------

/// HELPER FUNCTION for getAvailableMoves, checks if move is legal.
/// @param r move's row coordinate.
/// @param c move's column coordinate.
/// @returns true if move is legal.
bool knightType::goodMove(int r, int c) {
    int dim = board.size();  // max size of board
    bool legal = false;  // if a move is legal.

    if (r >= 0 && r < dim && c >= 0 && c < dim && board[r][c] == 0) {
        legal = true;
    }

    return legal;
}

/// ---------------------------------------------------------------------------

/// Checks if the board is full, meaning no 0's are on the board.
/// @returns true if the board is full.
bool knightType::fullBoard() {
    int dim = board.size();
    bool full = true;  // used for returning. Returns false if full board.
    for (int i = 0; i < dim && full == true; ++i) {
        for (int j = 0; j < dim && full == true; ++j) {
            if (board[i][j] == 0) {  // if coordinate is > 0, board isn't full.
                full = false; 
            }
        }
    }

    return full;
}

#endif /* KNIGHTTYPE_HPP_ */

/* EOF */