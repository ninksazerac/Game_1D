#include <SFML/Graphics.hpp>
#include <iostream>
#include "Animation.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1300, 800), "SFML Tutorial", sf::Style::Close | sf::Style::Resize);
	sf::RectangleShape player(sf::Vector2f(80.0f, 100.0f));
	player.setPosition(206.0f, 206.0f);
	sf::Texture playerTexture;
	playerTexture.loadFromFile("d1.png");
	player.setTexture(&playerTexture);

	Animation animation(&playerTexture, sf::Vector2u(3, 3), 0.3f);

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
				case sf::Event::Resized:
					printf("New window width: %i New window height: %i\n", evnt.size.width, evnt.size.height);
					break;
				case sf::Event::TextEntered:
					if (evnt.text.unicode < 128)
					{
					printf("%c", evnt.text.unicode);
					}
					
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			player.move(-0.1f, 0.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			player.move(0.1f, 0.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			player.move(0.0f, -0.1f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			player.move(0.0f, 0.1f);
		}

		animation.Update(0, deltaTime);
		player.setTextureRect(animation.uvRect);


		window.clear(sf::Color(150,150,150));
		window.draw(player);
		window.display();
	}
	
	return 0;

}