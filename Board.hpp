#pragma once

#include "Piece.hpp"
#include <optional>
#include <vector>

struct Move {
  int fromX, fromY;
  int toX, toY;
  std::optional<std::pair<int, int>> capturedPiece;
};

const int BOARD_SIZE = 8; 

class Board {
public:

// Constructor
Board();

void reset();

private:

// The grid of pieces filling with no pieces
Piece grid[BOARD_SIZE][BOARD_SIZE] = {PieceColor::None};

// Game board initialisation
void initBoard();

};