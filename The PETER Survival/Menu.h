#pragma once
#include<SFML\Graphics.hpp>
#include<iostream>
class Menu
{
public:
	Menu(sf::Texture* texture, sf::Vector2f scale, sf::Vector2f position);
	~Menu();
	void setScale(sf::Vector2f scale);
	void Draw(sf::RenderWindow& window);
	bool getGlobalBounds(sf::RenderWindow& window);

private:
	sf::Sprite body;
	sf::RectangleShape shape;
};