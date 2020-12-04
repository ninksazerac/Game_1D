#pragma once
#include <SFML/Graphics.hpp>
class Background
{
public:
	Background(sf::Texture* texture, float speed);

	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window);
private:
	sf::RectangleShape body[2];
	sf::Vector2f position;
	sf::Vector2f size;

	float speed;
};

