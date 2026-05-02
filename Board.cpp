#include "Board.hpp"

class Board {
public:

// Constructor
Board();


void reset();

private:

// x - is upper left corner; y - down right corner.
sf::Vector2i positionInWindow;

void initBoard();

};