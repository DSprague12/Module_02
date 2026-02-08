#include <iostream>
#include <string>
#include "board.hpp"

void parseMove(const std::string& move, int& fromR, int& fromC, int& toR, int& toC) {
    // Format: e2-e4 or e2 e4
    if (move.length() >= 5) {
        fromC = move[0] - 'a';
        fromR = move[1] - '1';
        toC = move[3] - 'a';
        toR = move[4] - '1';
    }
}

int main() {
    Board b;
    b.initStandard();
    
    Color currentPlayer = Color::White;
    bool gameRunning = true;
    
    std::cout << "Chess Game - Move pieces using algebraic notation (e.g., e2-e4)\n\n";
    
    // Main game loop
    while (gameRunning) {
        b.print();
        
        std::string playerColor = (currentPlayer == Color::White) ? "White" : "Black";
        
        if (b.isCheckmate(currentPlayer)) {
            std::cout << playerColor << " is in checkmate! Game over.\n";
            gameRunning = false;
            break;
        }
        
        if (b.isInCheck(currentPlayer)) {
            std::cout << playerColor << " is in check!\n";
        }
        
        std::cout << playerColor << " to move. Enter move (e.g., e2-e4) or 'quit': ";
        
        std::string move;
        std::getline(std::cin, move);
        
        if (move == "quit") {
            gameRunning = false;
            break;
        }
        
        if (move.length() < 5) {
            std::cout << "Invalid move format. Use e2-e4\n\n";
            continue;
        }
        
        int fromR, fromC, toR, toC;
        parseMove(move, fromR, fromC, toR, toC);
        
        // Validate coordinates
        if (fromC < 0 || fromC > 7 || fromR < 0 || fromR > 7 ||
            toC < 0 || toC > 7 || toR < 0 || toR > 7) {
            std::cout << "Invalid coordinates.\n\n";
            continue;
        }
        
        Piece* movingPiece = b.at(fromR, fromC);
        if (!movingPiece) {
            std::cout << "No piece at that square.\n\n";
            continue;
        }
        
        if (movingPiece->color != currentPlayer) {
            std::cout << "That's not your piece.\n\n";
            continue;
        }
        
        if (b.movePiece(fromR, fromC, toR, toC)) {
            currentPlayer = (currentPlayer == Color::White) ? Color::Black : Color::White;
            std::cout << '\n';
        } else {
            std::cout << "Illegal move.\n\n";
        }
    }
    
    return 0;
}