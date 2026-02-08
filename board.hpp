#pragma once
#include <array>
#include <memory>
#include "piece.hpp"

class Board {
public:
// Initilizes an 8x8 grid of pointers to Pieces
    using Row = std::array<std::unique_ptr<Piece>, 8>;
    std::array<Row, 8> grid;

    // Clears bourd and then sets up the standard chess position
    Board() { clear(); }
    void clear();
    void initStandard();

    // Prints the board to console
    void print() const;

    // Moves a piece if valid, returns true if successful
    bool movePiece(int fromR, int fromC, int toR, int toC);

    // Returns pointer to piece at given position, or nullptr if empty
    Piece* at(int row, int col) const { return grid[row][col].get(); }
    
    // Check and checkmate detection
    bool isSquareAttacked(int r, int c, Color attacker) const;
    bool isInCheck(Color king) const;
    bool hasLegalMove(Color color) const;
    bool isCheckmate(Color king) const;

private:
// Checks if given position is within board bounds, returns true if valid
    bool inBounds(int row, int col) const { 
        return (row >= 0 && row < 8 && col >= 0 && col < 8); 
    }
    bool pathClear(int fromR, int fromC, int toR, int toC) const;
    Piece* findKing(Color color) const;
};