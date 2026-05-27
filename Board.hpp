#pragma once

#include <SFML/Graphics.hpp>
#include "Piece.hpp"
#include <optional>
#include <vector>

struct Move {
  int fromX, fromY;
  int toX, toY;
  std::optional<sf::Vector2i> capturedPiece;
};

enum class SquereColor { None, White, Black };

const int BOARD_SIZE = 8; 

class Board {
public:

// Constructor
Board();

public:

void reset();
const Piece &getPiece(int x, int y) const;

// Checks if a proposed move is valid
bool isMoveValid(int fromX, int fromY, int toX, int toY, PieceColor currentTurn) const;

// Updates the board state with the given move
void makeMove(const Move &move);

// Return Move object, contain jumping logic
std::optional<Move> createMove(int fromX, int fromY, int toX, int toY, PieceColor currentTurn) const;

// Determines if a player has any forced jumps available
bool hasForcedJumps(PieceColor color) const;

// Function with logic to check if it has forced jumps
bool hasForcedJumpsForPiece(int x, int y) const;

// Determines the winner
PieceColor checkWin(PieceColor currentTurn);

// Determines if a player can make any moves
bool checkIfCanMove(PieceColor currentTurn);

// Function with logic to check if can move
bool checkIfCanMoveForPiece(int x, int y);

private:

// The grid of pieces filling with no pieces
Piece grid_piece[BOARD_SIZE][BOARD_SIZE] = {PieceColor::None};

// Game board initialisation
void initBoard();

// Check the bounds
bool Board::isInBounds(int x, int y) const;

// Check and promote piece to King
void promoteIfNeeded(int x, int y);
};