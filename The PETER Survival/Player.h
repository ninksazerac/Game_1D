#pragma once
#include <SFML\Graphics.hpp>
#include "Animation.h"
#include "Collider.h"
class Player
{
public:
	Player(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, float speed, float jumpHeight);
	~Player();


	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window);
	void OnColider(sf::Vector2f direction);
	void setPosition(sf::Vector2f position);
	
	
	
	sf::Vector2f GetPosition() { return body.getPosition(); }
	Collider GetCollider() { return Collider(body); }
	sf::FloatRect GetGlobal() { return body.getGlobalBounds(); }
private:
	sf::RectangleShape body;
	Animation animation;
	unsigned int row;
	float speed;
	bool faceRight;

	sf::Vector2f velocity;
	bool canJump;
	float jumpHeight;
};