#include "Board.hpp"
#include <cmath>

Board::Board()
{
    initBoard();
    reset();
}

void Board::reset()
{

}

void Board::initBoard()
{
    for(int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if((i % 2 + j % 2) == 0)
            {
                grid[i][j] = {PieceColor::Black};
            }
            else 
            {
                grid[i][j] = {PieceColor::White};
            }
        }
    }
}
    

