#include "Checkers.hpp"


Checkers::Checkers()
{

}

Checkers::~Checkers()
{

}

void Checkers::handleClick(int mouseX, int mouseY)
{
    int x = mouseX / TILE_SIZE;
    int y = mouseY / TILE_SIZE;

    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
        return;

    // If nothing selected yet
    if (!selected.has_value())
    {
        if (board[y][x] != nullptr)
        {
            selected = sf::Vector2i(x, y);
        }
    }
    else 
    {
        // Deselect if clicking same tile
        if (selected->x == x && selected->y == y)
        {
            selected.reset();
            return;
        }

        // For now: just move piece (no rules yet)
        if (board[selected->y][selected->x] != nullptr && board[y][x] == nullptr)
        {
            board[y][x] = board[selected->y][selected->x];
            board[selected->y][selected->x] = nullptr;
        }

        selected.reset();
    }
}