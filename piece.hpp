#pragma once
#include <cmath>

enum class Color { White, Black };

struct Piece {
    Color color;

    // 'K','Q','R','B','N','P'
    char symbol;

    bool moved = false;
    Piece(Color c, char s) : color(c), symbol(s) {}
    virtual ~Piece() = default;
    // dr = toR - fromR, dc = toC - fromC
    virtual bool canMove(int dr, int dc) const { return true; }
};

struct King : Piece {
    King(Color c) : Piece(c,'K') {}
    bool canMove(int dr, int dc) const override {
        return std::abs(dr) <= 1 && std::abs(dc) <= 1 && !(dr == 0 && dc == 0);
    }
};

struct Queen : Piece {
    Queen(Color c) : Piece(c,'Q') {}
    bool canMove(int dr, int dc) const override {
        int adr = std::abs(dr), adc = std::abs(dc);
        return (dr == 0 && dc != 0) || (dc == 0 && dr != 0) || (adr == adc && adr != 0);
    }
};

struct Rook : Piece {
    Rook(Color c) : Piece(c,'R') {}
    bool canMove(int dr, int dc) const override {
        return (dr == 0 && dc != 0) || (dc == 0 && dr != 0);
    }
};

struct Bishop : Piece {
    Bishop(Color c) : Piece(c,'B') {}
    bool canMove(int dr, int dc) const override {
        return std::abs(dr) == std::abs(dc) && dr != 0;
    }
};

struct Knight : Piece {
    Knight(Color c) : Piece(c,'N') {}
    bool canMove(int dr, int dc) const override {
        int adr = std::abs(dr), adc = std::abs(dc);
        return (adr == 2 && adc == 1) || (adr == 1 && adc == 2);
    }
};

struct Pawn : Piece {
    Pawn(Color c) : Piece(c,'P') {}
    bool canMove(int dr, int dc) const override {
        int dir = (color == Color::White) ? 1 : -1;
        if (dc == 0) {
            if (dr == dir) return true;
            if (!moved && dr == 2 * dir) return true;
            return false;
        } else if (std::abs(dc) == 1 && dr == dir) {
            // Capture moves, board will check if there's an opponent piece there
            return true;
        }
        return false;
    }
};