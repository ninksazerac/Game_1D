#include "Player.h"
#include "Animation.h"
Player::Player(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, float speed, float jumpHeight)
	:animation(texture, imageCount, switchTime)
{
	this->speed = speed;
	this->jumpHeight = jumpHeight;
	row = 0;
	faceRight = true;

	body.setSize(sf::Vector2f(110.0f, 120.0f)); //เซ็ตตัว player
	body.setOrigin(body.getSize() / 2.0f);
	body.setPosition(350.0f, 500.0f);

	body.setTexture(texture);
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
	velocity.x = 0.5f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		if (body.getPosition().x > 40.0f)
		{
			velocity.x -= speed;
		}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		if (body.getPosition().x < 1860.0f)
		{
			velocity.x += speed;
		}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) 
	{
		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canJump)
		if (body.getPosition().y > 450.0f)
		{
			canJump = false;

			velocity.y = -sqrtf(2.0f * 981.0f * jumpHeight);
			//square root (2.0f* gravity * jumpHeight);

		}
	canJump = true;





	//velocity
	velocity.y += 981.0f * deltaTime;

	if (velocity.x == 0.0f)
	{
		row = 0;
	}
	
	else
	{
		row = 1;
		if (velocity.x > 0.0f)
			faceRight = true;
		else
			faceRight = false;
	}

	animation.Update(row, deltaTime, faceRight);
	body.setTextureRect(animation.uvRect);
	body.move(velocity * deltaTime);
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);

}

void Player::OnColider(sf::Vector2f direction)
{
	if (direction.x < 0.0f)
	{
		//Colider on the left
		velocity.x = 0.0f;
	}
	else if (direction.x > 0.0f)
	{
		//Colider on the right
		velocity.x = 0.0f;
	}
	if (direction.y < 0.0f)
	{
		//Colider on the bottom
		velocity.y = 0.0f;
		canJump = true;
	}
	else if (direction.y > 0.0f)
	{
		//Colider on the top
		velocity.y = 0.0f;
	}
}

void Player::setPosition(sf::Vector2f position)
{
	body.setPosition(position);
}