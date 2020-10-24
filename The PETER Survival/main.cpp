#include <SFML\Graphics.hpp>
#include <iostream>
#include "Player.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1300, 800), "SFML Tutorial", sf::Style::Close | sf::Style::Resize);
	sf::Texture playerTexture;
	playerTexture.loadFromFile("doc-1.png");

	Player player(&playerTexture, sf::Vector2u(5, 5), 0.3, 150.0f);

	float deltaTime = 0.0f;
	sf::Clock clock;


	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();

		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}


		player.Update(deltaTime);


		window.clear(sf::Color(100, 100, 150));
		player.Draw(window);
		window.display();
	}

	return 0;

}
