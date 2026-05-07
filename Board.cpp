#include "Board.hpp"
#include <cmath>

Board::Board()
{
    
    initBoard();
    reset();
}

void Board::reset()
{
    // Black at top (y = 0, 1, 2)
    for (int y = 0; y < 3; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            if ((x + y) % 2 != 0)
            {
                grid_piece[y][x] = {PieceColor::Black, PieceType::Man};
            }
        }
  }
  // White at bottom (y = 5, 6, 7)
  for (int y = 5; y < 8; ++y)
  {
        for (int x = 0; x < 8; ++x)
        {
            if ((x + y) % 2 != 0)
            {
                grid_piece[y][x] = {PieceColor::White, PieceType::Man};
            }
        }
  }
}

void Board::initBoard()
{
    for(int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if((i % 2 + j % 2) == 0)
            {
                grid_squers[i][j] = {SquereColor::Black};
            }
            else
            {
                grid_squers[i][j] = {SquereColor::White};
            }
        }
    }

    reset();

}

const Piece &Board::getPieceAt(int x, int y) const
{
    if(isInBounds(x, y)) 
    {
        static Piece empty;
        return empty;
    }
    return grid_piece[y][x];
}

bool Board::isMoveValid(int fromX, int fromY, int toX, int toY, PieceColor currentTurn) const
{
    return 0;
}

void Board::makeMove(const Move &move)
{

}

bool Board::hasForcedJumps(PieceColor color) const
{
    return 0;
}

bool Board::isInBounds(int x, int y) const
{
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

std::optional<Move> Board::createMove(int fromX, int fromY, int toX, int toY, PieceColor currentTurn) const
{
    return std::nullopt;
}