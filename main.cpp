#include <SFML/Graphics.hpp>
#include "Checkers.hpp"

int WinMain()
{
	unsigned int x_size = BOARD_SIZE * TILE_SIZE + (BOARD_SIZE * TILE_SIZE * 0.25);
	unsigned int y_size = BOARD_SIZE * TILE_SIZE + (BOARD_SIZE * TILE_SIZE * 0.03);
	sf::RenderWindow window(sf::VideoMode({x_size, y_size}), "Checkers");
	sf::Image icon;
	icon.loadFromFile("img/icon.png");
	window.setIcon({icon.getSize().x, icon.getSize().y}, icon.getPixelsPtr());

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
                    game.handleClick(keyPressed->position.x, keyPressed->position.y, window.getSize());
				}
			}

			window.clear();
			game.draw(window);
			window.display();
		}
	}
}