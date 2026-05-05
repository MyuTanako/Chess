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

	 void handleClick(int mouseX, int mouseY) {
        int x = mouseX / TILE_SIZE;
        int y = mouseY / TILE_SIZE;

        if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
            return;

        // If nothing selected yet
        if (!selected.has_value()) {
            if (board[y][x] != nullptr) {
                selected = sf::Vector2i(x, y);
            }
        } else {
            // Deselect if clicking same tile
            if (selected->x == x && selected->y == y) {
                selected.reset();
                return;
            }

            // For now: just move piece (no rules yet)
            if (board[selected->y][selected->x] != nullptr && board[y][x] == nullptr) {
                board[y][x] = board[selected->y][selected->x];
                board[selected->y][selected->x] = nullptr;
            }

            selected.reset();
        }
    }

    void draw(sf::RenderWindow& window) {
    }

private:
    std::vector<std::vector<Piece*>> board;
    std::optional<sf::Vector2i> selected;


};



int main()
{
	sf::RenderWindow window(sf::VideoMode({BOARD_SIZE * TILE_SIZE + 200, BOARD_SIZE * TILE_SIZE}), "Checkers");
    Checkers game;

	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
			{
				window.close();
			}
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        	{
            	if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                	window.close();
        	}
			else if(const auto* keyPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (keyPressed->button == sf::Mouse::Button::Left)
				{
                    game.handleClick(keyPressed->position.x, keyPressed->position.y);
				}
			}

			window.clear();
			game.draw(window);
			window.display();
		}
	}
}