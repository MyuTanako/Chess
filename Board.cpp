#include "Board.hpp"
#include <cmath>

Board::Board()
{
    initBoard();
    reset();
}

void Board::reset()
{
    Piece black{PieceColor::Black, PieceType::Man};
    Piece white{PieceColor::White, PieceType::Man};
    Piece none{PieceColor::None, PieceType::Man};

    // Piece grid[BOARD_SIZE][BOARD_SIZE] 
    // = { {none,  black, none,  black, none,  black, none,  black},
    //     {black, none,  black, none,  black, none,  black, none},
    //     {none,  black, none,  black, none,  black, none,  black},
    //     {none,  none,  none,  none,  none,  none,  none,  none},
    //     {none,  none,  none,  none,  none,  none,  none,  none},
    //     {white, none,  white, none,  white, none,  white, none},
    //     {none,  white, none,  white, none,  white, none,  white},
    //     {white, none,  white, none,  white, none,  white, none}};

        Piece grid[BOARD_SIZE][BOARD_SIZE] 
    = { {none,  black, none,  black, none,  none,  none,  black},
        {black, none,  black, none,  none,  none,  white, none},
        {none,  black, none,  black, none,  none,  none,  black},
        {none,  none,  none,  none,  none,  none,  none,  none},
        {none,  none,  none,  none,  none,  none,  none,  none},
        {white, none,  white, none,  white, none,  white, none},
        {none,  white, none,  white, none,  white, none,  white},
        {white, none,  white, none,  white, none,  white, none}};

    for (int y = 0; y < BOARD_SIZE; ++y)
    {
        for (int x = 0; x < BOARD_SIZE; ++x)
        {
            grid_piece[y][x] = grid[y][x];
        }
    }


    // Black at top (y = 0, 1, 2)
    // for (int y = 0; y < 3; ++y)
    // {
    //     for (int x = 0; x < 8; ++x)
    //     {
    //         if ((x + y) % 2 != 0)
    //         {
    //             grid_piece[y][x] = {PieceColor::Black, PieceType::Man};
    //         }
    //         }
    // }
    // // White at bottom (y = 5, 6, 7)
    // for (int y = 5; y < 8; ++y)
    // {
    //     for (int x = 0; x < 8; ++x)
    //     {
    //         if ((x + y) % 2 != 0)
    //         {
    //             grid_piece[y][x] = {PieceColor::White, PieceType::Man};
    //         }
    //     }
    // }
}

void Board::initBoard() 
{
    for(int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if((i % 2 + j % 2) == 0)
            {
                grid_squares[i][j] = {SquereColor::Black};
            }
            else
            {
                grid_squares[i][j] = {SquereColor::White};
            }
        }
    }

    reset();
}

const Piece &Board::getPiece(int x, int y) const
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
        grid_piece[move.capturedPiece->y][move.capturedPiece->x] = Piece();
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
    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            if (grid_piece[y][x].color == color)
            {
                if (hasForcedJumpsForPiece(x, y))
                return true;
            }
        }
    }
    return false;
}

bool Board::hasForcedJumpsForPiece(int x, int y) const
{
    const Piece &p = grid_piece[y][x];
    if (p.isEmpty())
    {
        return false;
    }

    int dirS[4][2] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};

    if (p.type == PieceType::Man)
    {
        for (auto d : dirS)
        {
            int toX = x + d[0] * 2;
            int toY = y + d[1] * 2;

            auto move = createMove(x, y, toX, toY, p.color);
            if (move.has_value() && move->capturedPiece)
            {
                return true;
            }
        }
    } 
    else if (p.type == PieceType::King)
    {
        for (auto d : dirS)
        {
            int currentX = x + d[0];
            int currentY = y + d[1];

            bool foundEnemy = false;

            while (isInBounds(currentX, currentY))
            {
                const Piece &pathP = grid_piece[currentY][currentX];

                if (!pathP.isEmpty()) 
                {
                    if (pathP.color == p.color) 
                    {
                        break; // Blocked by friendly piece
                    }
                    else 
                    {
                        // Found an enemy! Check if the square directly behind it is empty.
                        if (foundEnemy)
                        {
                            break; // Cannot jump two enemies in a row
                        }
                        foundEnemy = true;
                    }
                }
                else
                {
                    if (foundEnemy)
                    {
                        // We found an enemy and now an empty square! This is a valid jump.
                        return true;
                    }
                }
                currentX += d[0];
                currentY += d[1];
            }
        }
    }
    return false;
}

bool Board::isInBounds(int x, int y) const
{
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

std::optional<Move> Board::createMove(int fromX, int fromY, int toX, int toY, PieceColor currentTurn) const
{
    //check bounds
    if (!isInBounds(fromX, fromY) || !isInBounds(toX, toY))
    {
        return std::nullopt;
    }

    //check turn
    const Piece &p = grid_piece[fromY][fromX];
    if (p.color != currentTurn)
    {
        return std::nullopt;
    }

    //check empty
    if (!grid_piece[toY][toX].isEmpty())
    {
        return std::nullopt;
    }

    // moves only on dark squares
    if ((toX + toY) % 2 == 0)
    {
        return std::nullopt; 
    }
        
    int dx = toX - fromX;
    int dy = toY - fromY;

    int absDx = std::abs(dx);
    int absDy = std::abs(dy);
    
    // must be diagonal
    if (absDx != absDy || absDx == 0)
    {
        return std::nullopt; 
    }

    //direction of Y
    //for white is -1 because the direction of Y must decrease
    //for black is 1 because the direction of Y must increase
    int dirY = (p.color == PieceColor::White) ? -1 : 1;

    //logic for regular piece
    if (p.type == PieceType::Man)
    {
        // Regular diagonal move
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
            // Russian Checkers: Men can capture backward, so we don't constrain dy to dirY * 2
            int midX = fromX + dx / 2;
            int midY = fromY + dy / 2;
            const Piece &midPiece = grid_piece[midY][midX];

            if (midPiece.isEmpty() || midPiece.color == p.color)
                return std::nullopt;

            return Move{fromX, fromY, toX, toY, sf::Vector2i(midX, midY)};
        }
        return std::nullopt;
    }
    else if (p.type == PieceType::King) //logic for King piece
    {
        // King logic ("flying king")
        // Any diagonal distance. We track the pieces we hop over.
        int stepX = (dx > 0) ? 1 : -1;
        int stepY = (dy > 0) ? 1 : -1;

        int currentX = fromX + stepX;
        int currentY = fromY + stepY;

        int piecesCount = 0;
        std::optional<sf::Vector2i> capPiece = std::nullopt;

        //Check all diagonal squares from current position to expection position 
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

PieceColor Board::checkWin(PieceColor currentTurn)
{
    //If all pieces one color was catched
    int blackCounter = 0;
    int whiteCounter = 0;
    for (int y = 0; y < BOARD_SIZE; ++y)
    {
        for (int x = 0; x < BOARD_SIZE; ++x)
        {
            if(grid_piece[y][x].color == PieceColor::Black)
            {
                blackCounter++;
            }
            if(grid_piece[y][x].color == PieceColor::White)
            {
                whiteCounter++;
            }
        }
    }

    if(blackCounter == 0)
    {
        return PieceColor::White;
    }
    if(whiteCounter == 0)
    {
        return PieceColor::Black;
    }

    //If there no moves
    for (int y = 0; y < BOARD_SIZE; ++y)
    {
        for (int x = 0; x < BOARD_SIZE; ++x)
        {
            if(grid_piece[y][x].color == PieceColor::Black)
            {
                blackCounter++;
            }
            if(grid_piece[y][x].color == PieceColor::White)
            {
                whiteCounter++;
            }
        }
    }




    return PieceColor::None;

}

bool Board::checkIfCanMove(PieceColor currentTurn)
{
    if(hasForcedJumps(currentTurn))
    {
        return true;
    }

    for (int y = 0; y < BOARD_SIZE; ++y)
    {
        for (int x = 0; x < BOARD_SIZE; ++x)
        {
            if (grid_piece[y][x].color == currentTurn)
            {
                if (checkIfCanMoveForPiece(x, y))
                return true;
            }
        }
    }
    return false;
}

bool Board::checkIfCanMoveForPiece(int x, int y)
{
    return false;
}