#pragma once

#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Piece.hpp"

const int TILE_SIZE = 60;

class Checkers {
public:
    Checkers();

    ~Checkers();

    void handleClick(int, int, sf::Vector2u);
    void draw(sf::RenderWindow&);

private:

    void endTurn();

    // std::vector<std::vector<Piece*>> board;
    std::optional<sf::Vector2i> selected;
    Board board;
    sf::RenderWindow window;
    PieceColor currentTurn;
    bool mustContinueJump;
};
