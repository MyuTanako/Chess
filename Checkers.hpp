#pragma once

#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Piece.hpp"

const int TILE_SIZE = 60;

class Checkers {
public:
    Checkers() {
        board.resize(BOARD_SIZE, std::vector<Piece*>(BOARD_SIZE, nullptr));
        initBoard();
    }

        ~Checkers() {
        for (auto& row : board)
            for (auto& p : row)
                delete p;
    }

    void handleClick(int mouseX, int mouseY);

    void draw(sf::RenderWindow& window);

private:
    std::vector<std::vector<Piece*>> board;
    std::optional<sf::Vector2i> selected;
};
