#pragma once

#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Piece.hpp"

const int TILE_SIZE = 100;

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
    PieceColor winner = PieceColor::None;
    bool mustContinueJump;

    sf::Texture boardTex;
    sf::Texture blackPieceTex;
    sf::Texture whitePieceTex;
    sf::Texture kingGunTex;
};
