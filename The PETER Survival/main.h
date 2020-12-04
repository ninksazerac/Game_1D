#pragma once
#include "Player.h"

class Game
{
private:

	//Window
	sf::RenderWindow* window;


	//World
	sf::Sprite worldBackground;


	//Player
	Player* player;

	//Private function
	void initWindow();
	void initTextures();
	void initGUI();
	void initWorld();


public:
};