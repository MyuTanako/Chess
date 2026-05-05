#include <SFML/Graphics.hpp>
#include "Checkers.hpp"

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