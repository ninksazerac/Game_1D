#pragma once
#include "Sword.h"
#include<SFML\Graphics.hpp>
#include<iostream>
class Bullet
{
public:
	Bullet(sf::Texture* texture, int* effect);
	~Bullet();
	void Draw(sf::RenderWindow& window);
	sf::Vector2f getPosition();
	sf::Vector2f getHalfSize();
	sf::Vector2f positionPlayer;
	sf::Event* evnt;
	bool canAttack();

private:
	int* effect;
	void action();
	std::vector<Sword> sword;
	int fire_num = 30;
	float deltaTime = 0.0f;
	sf::Clock clock;
	// debug sword press
	bool isEPrees = false;

	// Test
	int temp = 0;
};