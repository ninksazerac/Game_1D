#include "Menu.h"

Menu::Menu(sf::Texture* texture, sf::Vector2f scale, sf::Vector2f position)
{
	this->body.setScale(scale);
	this->body.setPosition(position);
	this->body.setTexture(*texture);
}

Menu::~Menu()
{
}

void Menu::setScale(sf::Vector2f scale)
{
	this->body.setScale(scale);
}

void Menu::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

bool Menu::getGlobalBounds(sf::RenderWindow& window)
{
	return this->body.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
}