#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "Player.h"
#include "Platform.h"
#include "Background.h"
#include "Menu.h"

static const float VIEW_HEIGHT = 900.0f;
void ResizeView(const sf::RenderWindow& window, sf::View& view) //set display ให้คงที่เวลาขยาย หด
{
	float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
	view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT);
}

using namespace sf;
using namespace std;

int main()
{

	/////////////////////////////////Display/////////////////////////////////
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "GUNSO!!", sf::Style::Close | sf::Style::Resize);
	window.setVerticalSyncEnabled(true);
	/*sf::View view(sf::Vector2f(1920.0f, 1080.0f), sf::Vector2f(1920.0f, 1080.0f));*/
	sf::Image icon;
	if (!icon.loadFromFile("Resource/Icon/icon.png"))
	{
		return -1;
	}
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());  // เซ้ท icon เกม

	////////////////////////////
	int game = 0;
	sf::Event evnt;
	int  scoreCount = 0;
	srand(time(NULL));

	////////////////////////////Text
	sf::Font word;
	word.loadFromFile("Resource/Font/RobotoMono-BoldItalic.ttf");

	sf::Text scoreText;
	scoreText.setPosition(1800, 40);
	scoreText.setCharacterSize(50);
	scoreText.setFont(word);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setString(to_string(scoreCount));

	sf::Text score1;
	score1.setPosition(1450, 40);
	score1.setCharacterSize(50);
	score1.setFont(word);
	score1.setFillColor(sf::Color::White);
	score1.setString("Score :");
	

	

	/////////////////////////////////Menu/////////////////////////////////
	sf::Texture bgmenuTexture;
	bgmenuTexture.loadFromFile("Resource/Menu/menubg1.png");
	sf::Texture startTexture;
	startTexture.loadFromFile("Resource/Menu/startmenu.png");
	sf::Texture scoreTexture;
	scoreTexture.loadFromFile("Resource/Menu/score.png");
	sf::Texture quitTexture;
	quitTexture.loadFromFile("Resource/Menu/quit.png");
	sf::Texture backTexture;
	backTexture.loadFromFile("Resource/Menu/back.png");
	//Score
	sf::Texture bgscoreTexture;
	bgscoreTexture.loadFromFile("Resource/Score/scorebg.png");

	Menu bgmenu(&bgmenuTexture, sf::Vector2f(1.0f , 1.0f), sf::Vector2f(0.0f , 0.0f));
	Menu bgscore(&bgscoreTexture, sf::Vector2f(1.0f, 1.0f), sf::Vector2f(0.0f, 0.0f));
	Menu start(&startTexture, sf::Vector2f(0.5f , 0.5f), sf::Vector2f(710.0f , 350.0f));
	Menu score(&scoreTexture, sf::Vector2f(0.5f , 0.5f), sf::Vector2f(720.0f , 500.0f));
	Menu quit(&quitTexture, sf::Vector2f(0.5f , 0.5f), sf::Vector2f(720.0f, 650.0f));
	Menu back(&backTexture, sf::Vector2f(0.3f, 0.3f), sf::Vector2f(1500.0f, 900.0f));

	

	/////////////////////////////////Background/////////////////////////////////
	//พื้นหลัง
	sf::Texture bgtexture[2];
	sf::Sprite bg[2];
	bgtexture[0].loadFromFile("Resource/Background/bggame3.png");
	bgtexture[1].loadFromFile("Resource/Background/bggame3.png");
	bg[0].setPosition(sf::Vector2f(-100, 0));
	bg[1].setPosition(sf::Vector2f(0, 0));//เซ็ตตำแหน่งฉาก ให้ชิดซ้ายไม่มีขอบดำ

	bg[0].setTexture(bgtexture[0]);
	bg[1].setTexture(bgtexture[1]);


	/////////////////////////////////Player/////////////////////////////////
	//player
	sf::Texture playerTexture;
	playerTexture.loadFromFile("Resource/Sprite/playergiroro.png");	

	Player player(&playerTexture, sf::Vector2u(5, 6), 0.3f, 400.0f, 170.0f);


	/////////////////////////////////Jump/////////////////////////////////
	//jump
	std::vector<Platform> platforms;

	/////////////////////////////////Enemy/////////////////////////////////
	// v.1
	sf::RectangleShape enemy1(sf::Vector2f(60.0f, 80.f));
	sf::Texture enemy1pic;
	enemy1pic.loadFromFile("Resource/Sprite/mon1.png");
	enemy1.setTexture(&enemy1pic);
	std::vector<RectangleShape> enemies1;
	enemies1.push_back(RectangleShape(enemy1)); //วาดมอน1

	int enemySpawnTimer = 0;
	sf::Clock enemyclock;
	float enemyDelay = enemyclock.getElapsedTime().asSeconds();

	sf::RectangleShape enemy2(sf::Vector2f(60.0f, 80.f));
	sf::Texture enemy2pic;
	enemy2pic.loadFromFile("Resource/Sprite/mon2.png");
	enemy2.setTexture(&enemy2pic);
	std::vector<RectangleShape> enemies2;
	enemies2.push_back(RectangleShape(enemy2));

	sf::Vector2f playerCenter;
	


	/////////////////////////////////Bullet/////////////////////////////////
	int shootTimer = 0;

	sf::RectangleShape bullet(sf::Vector2f(30.0f, 40.f));
	sf::Texture projectilespic;
	projectilespic.loadFromFile("Resource/Weapon/weapon.png");
	bullet.setTexture(&projectilespic);
	std::vector<RectangleShape> bullet1;
	bullet1.push_back(RectangleShape(bullet));//วาดกระสุน ปิ๊วๆ
	

	/////////////////////////////////Auto run background/////////////////////////////////
	//พื้นหลังต่อไปเรื่อยๆ
	std::vector<Background> backgrounds; //ชื่อ backgrounds
	backgrounds.push_back(Background(&bgtexture[0], -100.f));
	backgrounds.push_back(Background(&bgtexture[1], -100.f));


	/////////////////////////////////Platform/////////////////////////////////
	//platform
	platforms.push_back(Platform(nullptr, sf::Vector2f(2500.0f, 200.0f), sf::Vector2f(900.0f, 650.f)));
	/*platforms.push_back(Platform(nullptr, sf::Vector2f(2500.0f, 20.0f), sf::Vector2f(900.0f, 0.f)));*/
	/*platforms.push_back(Platform(nullptr, sf::Vector2f(1000.0f, 200.0f), sf::Vector2f(900.0f, 900.f)));*/
	
	


	float deltaTime = 0.0f;
	sf::Clock clock;

	

	//เปิดเกมมา
	while (window.isOpen())
	{

		while (game == 0)
		{
			
			
			//วาดหน้าเมนู
			bgmenu.Draw(window);
			start.Draw(window);
			score.Draw(window);
			quit.Draw(window);
			window.display();
			if (start.getGlobalBounds(window)) {
				start.setScale(sf::Vector2f(0.6f, 0.6f));
			}
			else {
				start.setScale(sf::Vector2f(0.5f, 0.5f));
			}
			if (score.getGlobalBounds(window)) {
				score.setScale(sf::Vector2f(0.6f, 0.6f));
			}
			else {
				score.setScale(sf::Vector2f(0.5f, 0.5f));
			}
			if (quit.getGlobalBounds(window)) {
				quit.setScale(sf::Vector2f(0.6f, 0.6f));
			}
			else {
				quit.setScale(sf::Vector2f(0.5f, 0.5f));
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (start.getGlobalBounds(window)) {
					game = 1;
				}
				else if (score.getGlobalBounds(window)) {
					game = 2;
				}
				else if (quit.getGlobalBounds(window)) {
					game = 3;
				}
			}
		}

		//score ติดย้อนกลับ
		while (game == 2)
		{
			bgscore.Draw(window);
			back.Draw(window);
			window.display();
			if (back.getGlobalBounds(window)) {
				back.setScale(sf::Vector2f(0.4f, 0.4f));
			}
			else {
				back.setScale(sf::Vector2f(0.3f, 0.3f));
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && back.getGlobalBounds(window))
			{
				game = 0;
			}
		}

		//quit
		while (game == 3)
		{
			window.close();
		}

		/////////////////////////////////////////////////////////////////////////

	
		//start
		while (game == 1)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				game = 0;
			}
			deltaTime = clock.restart().asSeconds();
			if (deltaTime > 1.0f / 20.0f)
				deltaTime = 1.0f / 20.0f;



			//Update
			//player shoot
			playerCenter = Vector2f(player.GetPosition().x - 15, player.GetPosition().y + 10);


			//bullet
			if (shootTimer < 30)
				shootTimer++;
			if (Keyboard::isKeyPressed(Keyboard::E) && shootTimer >= 30) //กดยิงงง
			{
				bullet.setPosition(playerCenter);
				bullet1.push_back(RectangleShape(bullet));
				shootTimer = 0;
			}

			for (size_t i = 1; i < bullet1.size(); i++)
			{
				bullet1[i].move(15.f, 0.f);

				if (bullet1[i].getPosition().x <= 15)
					bullet1.erase(bullet1.begin() + i);
			}

			//enemy
			//mon 1
			if (scoreCount <= 49)
			{
				if (enemySpawnTimer < 30)
				{
					enemySpawnTimer++;
				}

				if (enemySpawnTimer >= 120)
				{
					enemy1.setPosition(Vector2f(2000, 470));
					enemies1.push_back(RectangleShape(enemy1));
					enemySpawnTimer = 0;
				}
				enemySpawnTimer++;
			}
			for (size_t i = 1; i < enemies1.size(); i++)
			{
				enemies1[i].move(-5.f, 0.f);


				/*if (enemies1[i].getPosition().x > window.getSize().x);
					enemies1.erase(enemies1.begin() + i);*/
			}

			//mon 2
			if (scoreCount >= 50)
			{
				if (enemySpawnTimer < 50)
				{
					enemySpawnTimer++;
				}

				if (enemySpawnTimer >= 70)
				{
					enemy2.setPosition(Vector2f(2000, 470));
					enemies2.push_back(RectangleShape(enemy2));
					enemySpawnTimer = 0;
				}
				enemySpawnTimer++;
			}
			for (size_t i = 1; i < enemies2.size(); i++)
			{
				enemies2[i].move(-5.f, 0.f);
				

				
				/*if (enemies1[i].getPosition().x > window.getSize().x);
					enemies1.erase(enemies1.begin() + i);*/
			}

			//Collision
			//mon 1
				for (size_t i = 1; i < bullet1.size(); i++)
				{
					for (size_t k = 1; k < enemies1.size(); k++)
					{
						if (bullet1[i].getGlobalBounds().intersects(enemies1[k].getGlobalBounds()))
						{
							bullet1.erase(bullet1.begin() + i);
							enemies1.erase(enemies1.begin() + k);
							scoreCount += 5;
							break;
						}
					}
				}
			//mon 2
				for (size_t i = 1; i < bullet1.size(); i++)
				{
					for (size_t k = 1; k < enemies2.size(); k++)
					{
						if (bullet1[i].getGlobalBounds().intersects(enemies2[k].getGlobalBounds()))
						{
							bullet1.erase(bullet1.begin() + i);
							enemies2.erase(enemies2.begin() + k);
							scoreCount += 10;
							break;
						}
					}
				}


			//////////////////////////////////////////////////////////////////////////////////////////

			player.Update(deltaTime);

			//platform
			sf::Vector2f direction;
			/*for (Platform& platform : platforms)
				if (platform.GetCollider().CheckCollision(player.GetCollider(), 1.0f))
					player.OnColider(direction);*/
			for (Platform& platform : platforms)
				if (player.GetGlobal().intersects(platform.GetGlobal()))
				{
					if (platform.GetCollider().CheckCollision(player.GetCollider(), 1.0f))
						player.OnColider(direction);

					player.setPosition(sf::Vector2f(player.GetPosition().x, 500.0f));

				}

			
			

			



			//updateพื้นหลังรันเรื่อยๆ

			for (Background& background : backgrounds)
				background.Update(deltaTime);



			/////////////////////////////////Draw or Render/////////////////////////////////
			window.clear();
			
			

			/*window.setView(view);*/
			window.draw(bg[0]);
			window.draw(bg[1]);


			//วาดฉากเลื่อน
			for (Background& background : backgrounds)
				background.Draw(window);

			player.Draw(window);

			//วาด score
			scoreText.setString(to_string(scoreCount));
			window.draw(score1);
			window.draw(scoreText);
			///////////////////////////////// Draw shoots & enemies1 /////////////////////////////////
			
			
			for (size_t i = 1; i < enemies1.size(); i++)
			{
				window.draw(enemies1[i]);
			}
			for (size_t i = 1; i < enemies2.size(); i++)
			{
				window.draw(enemies2[i]);
			}
			for (size_t i = 1; i < bullet1.size(); i++)
			{
				window.draw(bullet1[i]);
			}

			//////////////////////////////////////////////////////////////////////////////////////////

			//ล็อคฉากไม่ให้เลื่อนเกินขอบซ้าย
			if (player.GetPosition().x >= 900 && player.GetPosition().x <= 6223)
			{
				/*view.setCenter(player.GetPosition().x, 540);*/
				player.GetPosition().x, 540;
			}

			for (Platform& platform : platforms)
				platform.Draw(window);

			

			window.display();

		}

		return 0;
	}
}