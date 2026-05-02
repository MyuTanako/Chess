#pragma once

#include "Piece.hpp"
#include <optional>
#include <vector>

struct Move {
  int fromX, fromY;
  int toX, toY;
  std::optional<std::pair<int, int>> capturedPiece;
};

class Board {
public:

// Constructor
Board();

void reset();

private:
// x - is upper left corner; y - down right corner.
sf::Vector2i positionInWindow;

// Game board initialisation
void initBoard();

};