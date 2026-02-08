#include "board.hpp"
#include <iostream>

void Board::clear() {
    // Resets all pointers in the grid to null
    for (auto &row : grid) for (auto &p : row) p.reset();
}

// Sets up the standard chess position with pieces in their initial places
void Board::initStandard() {
    clear();
    // Define chess peices
    const char back[8] = {'R','N','B','Q','K','B','N','R'};

    for (int c = 0; c < 8; ++c) {
        // White back rank
        switch (back[c]) {
            case 'R': grid[0][c] = std::make_unique<Rook>(Color::White); break;
            case 'N': grid[0][c] = std::make_unique<Knight>(Color::White); break;
            case 'B': grid[0][c] = std::make_unique<Bishop>(Color::White); break;
            case 'Q': grid[0][c] = std::make_unique<Queen>(Color::White); break;
            case 'K': grid[0][c] = std::make_unique<King>(Color::White); break;
        }
        grid[1][c] = std::make_unique<Pawn>(Color::White);

        // Black back rank
        grid[6][c] = std::make_unique<Pawn>(Color::Black);
        switch (back[c]) {
            case 'R': grid[7][c] = std::make_unique<Rook>(Color::Black); break;
            case 'N': grid[7][c] = std::make_unique<Knight>(Color::Black); break;
            case 'B': grid[7][c] = std::make_unique<Bishop>(Color::Black); break;
            case 'Q': grid[7][c] = std::make_unique<Queen>(Color::Black); break;
            case 'K': grid[7][c] = std::make_unique<King>(Color::Black); break;
        }
    }
}

// Prints the board to console
void Board::print() const {
    for (int r = 7; r >= 0; --r) {
        std::cout << r + 1 << " ";
        for (int c = 0; c < 8; ++c) {
            if (grid[r][c]) {
                // White pieces in bright white, Black pieces in bright black (gray)
                if (grid[r][c]->color == Color::White) {
                    std::cout << "\033[97m" << grid[r][c]->symbol << "\033[0m ";
                } else {
                    std::cout << "\033[90m" << grid[r][c]->symbol << "\033[0m ";
                }
            } else {
                std::cout << ". ";
            }
        }
        std::cout << '\n';
    }
    std::cout << "  a b c d e f g h\n";
}

// Moves a piece if valid, returns true if successful
bool Board::movePiece(int fromR, int fromC, int toR, int toC) {
    if (!inBounds(fromR, fromC) || !inBounds(toR, toC)) return false;
    auto &src = grid[fromR][fromC];
    if (!src) return false;

    int dr = toR - fromR;
    int dc = toC - fromC;

    // Can't capture your own pieces
    if (grid[toR][toC] && grid[toR][toC]->color == src->color) return false;

    // Ask piece if the movement shape is allowed
    if (!src->canMove(dr, dc)) return false;

    // Check if path is clear for moving pieces
    if (!pathClear(fromR, fromC, toR, toC)) return false;

    // If it's a King, check if destination is under attack
    if (src->symbol == 'K') {
        Color enemy = (src->color == Color::White) ? Color::Black : Color::White;
        if (isSquareAttacked(toR, toC, enemy)) return false;
    }

    // Move
    grid[toR][toC] = std::move(src);
    grid[toR][toC]->moved = true;
    return true;
}

bool Board::pathClear(int fromR, int fromC, int toR, int toC) const {
    int dr = (toR > fromR) ? 1 : (toR < fromR) ? -1 : 0;
    int dc = (toC > fromC) ? 1 : (toC < fromC) ? -1 : 0;
    
    int r = fromR + dr;
    int c = fromC + dc;
    
    while (r != toR || c != toC) {
        if (grid[r][c]) return false;
        r += dr;
        c += dc;
    }
    return true;
}

Piece* Board::findKing(Color color) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (grid[r][c] && grid[r][c]->color == color && grid[r][c]->symbol == 'K') {
                return grid[r][c].get();
            }
        }
    }
    return nullptr;
}

bool Board::isSquareAttacked(int r, int c, Color attacker) const {
    for (int ar = 0; ar < 8; ++ar) {
        for (int ac = 0; ac < 8; ++ac) {
            Piece* p = grid[ar][ac].get();
            if (!p || p->color != attacker) continue;
            
            int dr = r - ar;
            int dc = c - ac;
            
            if (p->canMove(dr, dc) && pathClear(ar, ac, r, c)) {
                return true;
            }
        }
    }
    return false;
}

bool Board::isInCheck(Color king) const {
    Piece* k = findKing(king);
    if (!k) return false;
    
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (grid[r][c].get() == k) {
                Color enemy = (king == Color::White) ? Color::Black : Color::White;
                return isSquareAttacked(r, c, enemy);
            }
        }
    }
    return false;
}

bool Board::hasLegalMove(Color color) const {
    // We need to simulate moves, so we cast away const temporarily
    Board* mutableThis = const_cast<Board*>(this);
    
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = mutableThis->grid[r][c].get();
            if (!p || p->color != color) continue;
            
            for (int tr = 0; tr < 8; ++tr) {
                for (int tc = 0; tc < 8; ++tc) {
                    if (r == tr && c == tc) continue;
                    
                    int dr = tr - r;
                    int dc = tc - c;
                    
                    if (!p->canMove(dr, dc)) continue;
                    if (!pathClear(r, c, tr, tc)) continue;
                    if (mutableThis->grid[tr][tc] && mutableThis->grid[tr][tc]->color == color) continue;
                    
                    // Simulate move
                    mutableThis->grid[tr][tc].swap(mutableThis->grid[r][c]);
                    
                    bool legal = !isInCheck(color);
                    
                    // Undo move
                    mutableThis->grid[tr][tc].swap(mutableThis->grid[r][c]);
                    
                    if (legal) return true;
                }
            }
        }
    }
    return false;
}

bool Board::isCheckmate(Color king) const {
    return isInCheck(king) && !hasLegalMove(king);
}