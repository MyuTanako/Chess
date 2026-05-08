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
    if(!isInBounds(x, y)) 
    {
        static Piece empty;
        return empty;
    }
    return grid_piece[y][x];
}

bool Board::isMoveValid(int fromX, int fromY, int toX, int toY, PieceColor currentTurn) const
{
    if(!createMove(fromX, fromY, toX, toY, currentTurn).has_value())
    {
        return false;
    }
    return true;
}

void Board::makeMove(const Move &move)
{
    grid_piece[move.toY][move.toX] = grid_piece[move.fromY][move.fromX];
    grid_piece[move.fromY][move.fromX] = Piece();
    if (move.capturedPiece)
    {
        grid_piece[move.capturedPiece->x][move.capturedPiece->y] = Piece();
    }

    promoteIfNeeded(move.toX, move.toY);
}

void Board::promoteIfNeeded(int x, int y) {
    Piece &p = grid_piece[y][x];
    if (p.isEmpty() || p.type == PieceType::King)
        return;

    if (p.color == PieceColor::White && y == 0)
    {
        p.type = PieceType::King;
    }
    else if (p.color == PieceColor::Black && y == 7)
    {
        p.type = PieceType::King;
    }
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
    if (!isInBounds(fromX, fromY) || !isInBounds(toX, toY))
    {
        return std::nullopt;
    }

    const Piece &p = grid_piece[fromY][fromX];
    if (p.color != currentTurn)
    {
        return std::nullopt;
    }

    if (!grid_piece[toY][toX].isEmpty())
    {
        return std::nullopt;
    }

    if ((toX + toY) % 2 == 0)
    {
        return std::nullopt; // Moves only on dark squares
    }
        
    int dx = toX - fromX;
    int dy = toY - fromY;

    int absDx = std::abs(dx);
    int absDy = std::abs(dy);

    if (absDx != absDy || absDx == 0)
    {
        return std::nullopt; // Must be diagonal
    }

    int dirY = (p.color == PieceColor::White) ? -1 : 1;

    if (p.type == PieceType::Man)
    {
        // Regular move
        if (absDx == 1)
        {
            if (dy != dirY)
            {
                return std::nullopt; // Man can move forward only
            }
            return Move{fromX, fromY, toX, toY, std::nullopt};
        }

        // Jump move
        if (absDx == 2)
        {
            // Russian Checkers: Men can capture backward, so we don't constrain dy to
            // dirY * 2
            int midX = fromX + dx / 2;
            int midY = fromY + dy / 2;
            const Piece &midPiece = grid_piece[midY][midX];

            if (midPiece.isEmpty() || midPiece.color == p.color)
                return std::nullopt;

            return Move{fromX, fromY, toX, toY, sf::Vector2i(midX, midY)};
        }
        return std::nullopt;
    }
    else if (p.type == PieceType::King)
    {
        // King logic ("flying king")
        // Any diagonal distance. We track the pieces we hop over.
        int stepX = (dx > 0) ? 1 : -1;
        int stepY = (dy > 0) ? 1 : -1;

        int currentX = fromX + stepX;
        int currentY = fromY + stepY;

        int piecesCount = 0;
        std::optional<sf::Vector2i> capPiece = std::nullopt;

        while (currentX != toX && currentY != toY)
        {
            const Piece &pathP = grid_piece[currentY][currentX];
            if (!pathP.isEmpty()) {
                if (pathP.color == p.color)
                {
                    return std::nullopt; // Cannot jump over friends
                }
                piecesCount++;
                capPiece = sf::Vector2i(currentX, currentY);
            }
            currentX += stepX;
            currentY += stepY;
        }

        if (piecesCount == 0)
        {
            // Regular move for King
            return Move{fromX, fromY, toX, toY, std::nullopt};
        }
        else if (piecesCount == 1)
        {
            return Move{fromX, fromY, toX, toY, capPiece};
        } 
        else 
        {
            return std::nullopt; // Cannot jump over multiple pieces in one line
        }
    }

    
    return std::nullopt;
}