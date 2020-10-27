#include <SFML\Graphics.hpp>
#include <iostream>
#include "Player.h"


static const float VIEW_HEIGHT = 1080.0f;
void ResizeView(const sf::RenderWindow& window, sf::View& view)
{
	float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
	view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT);
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "The PETER Survival", sf::Style::Close | sf::Style::Resize);
	window.setVerticalSyncEnabled(true);
	sf::View view(sf::Vector2f(1920.0f, 1080.0f), sf::Vector2f(1920.0f, 1080.0f));
	sf::Texture bgtexture;
	sf::Sprite bg;
	
	bgtexture.loadFromFile("bg-game2.jpg");
	bg.setTexture(bgtexture);

	sf::Texture playerTexture;
	playerTexture.loadFromFile("doc-1.png");	





	Player player(&playerTexture, sf::Vector2u(5, 5), 0.3f, 200.0f);

	float deltaTime = 0.0f;
	sf::Clock clock;

	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();
		if (deltaTime > 1.0f / 20.0f)
			deltaTime = 1.0f / 20.0f;

		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				ResizeView(window, view);
				break;
			}


		}
		
		view.setCenter(player.GetPosition());
		player.Update(deltaTime);
		


		window.clear();
		window.setView(view);
		window.draw(bg);
		player.Draw(window);
		window.display();
	}

	return 0;

}

