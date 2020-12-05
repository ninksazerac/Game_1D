#pragma once
#include "Player.h"
#include <map>

class Game
{
private:

	//Window
	sf::RenderWindow* window;

	//Resources
	std::map<std::string, sf::Texture*>initTextures;

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