#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Piece.hpp"

const int TILE_SIZE = 60;
const int BOARD_SIZE = 8;

struct Piece {
    bool isWhite;
    bool isQueen;
};

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
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                tile.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                tile.setFillColor((x + y) % 2 == 0 ? sf::Color(240, 217, 181) : sf::Color(181, 136, 99));

                // Highlight selected
                if (selected.has_value() && selected->x == x && selected->y == y) {
                    tile.setFillColor(sf::Color::Green);
                }

                window.draw(tile);

                if (board[y][x]) {
                    sf::CircleShape piece(TILE_SIZE / 2 - 10);
                    piece.setPosition(sf::Vector2f(x * TILE_SIZE + 10, y * TILE_SIZE + 10));
                    piece.setFillColor(board[y][x]->isWhite ? sf::Color::Red : sf::Color::Blue);
                    window.draw(piece);
                }
            }
        }
    }

private:
    std::vector<std::vector<Piece*>> board;
    std::optional<sf::Vector2i> selected;

    void initBoard() {
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                if ((x + y) % 2 != 0)
                    board[y][x] = new Piece{false, false};
            }
        }
        for (int y = 5; y < 8; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                if ((x + y) % 2 != 0)
                    board[y][x] = new Piece{true, false};
            }
        }
    }
};



int main()
{
	// sf::RenderWindow window( sf::VideoMode( { 500, 500 } ), "SFML works!" );
	sf::RenderWindow window(sf::VideoMode({BOARD_SIZE * TILE_SIZE + 200, BOARD_SIZE * TILE_SIZE}), "Checkers");
    Checkers game;
	// sf::CircleShape shape( 100.f );
	// shape.setFillColor( sf::Color::White );

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