#pragma once

#include "Piece.hpp"
#include <optional>
#include <vector>

struct Move {
  int fromX, fromY;
  int toX, toY;
  std::optional<std::pair<int, int>> capturedPiece;
};

enum class SquereColor { None, White, Black };

const int BOARD_SIZE = 8; 

class Board {
public:

// Constructor
Board();

void reset();
const Piece &getPieceAt(int x, int y) const;

// Checks if a proposed move is valid under current board state and turn rules
bool isMoveValid(int fromX, int fromY, int toX, int toY, PieceColor currentTurn) const;

// Updates the board state with the given move
void makeMove(const Move &move);

// Determines if a player has any forced jumps available
bool hasForcedJumps(PieceColor color) const;


private:

// The grid of pieces filling with no pieces
Piece grid_piece[BOARD_SIZE][BOARD_SIZE] = {PieceColor::None};

// The grid of Squere filling with no color
SquereColor grid_squers[BOARD_SIZE][BOARD_SIZE] = {SquereColor::None};

// Game board initialisation
void initBoard();

bool Board::isInBounds(int x, int y) const;
};