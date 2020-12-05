#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
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

int main()
{

	/////////////////////////////////Display/////////////////////////////////
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "GUNSO!!", sf::Style::Close | sf::Style::Resize);
	window.setVerticalSyncEnabled(true);
	/*sf::View view(sf::Vector2f(1920.0f, 1080.0f), sf::Vector2f(1920.0f, 1080.0f));*/

	////////////////////////////
	int game = 0;
	sf::Event evnt;
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
	playerTexture.loadFromFile("Resource/Sprite/playerkeroro.png");	

	Player player(&playerTexture, sf::Vector2u(5, 6), 0.3f, 400.0f, 170.0f);


	/////////////////////////////////Jump/////////////////////////////////
	//jump
	std::vector<Platform> platforms;
	

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
			while (window.pollEvent(evnt))
			{
				switch (evnt.type)
				{
				case sf::Event::Closed:
					window.close();
					break;
				}
			}
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
				/*else if (play.getGlobalBounds(window)) {
					game = 0;
				}*/
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

		/*while (game == 3)
		{
			score.Draw(window);
			back.Draw(window);
			window.display();
			if (back.getGlobalBounds(window)) {
				back.setScale(sf::Vector2f(0.7f, 0.7f));
			}
			else {
				back.setScale(sf::Vector2f(0.6f, 0.6f));
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && back.getGlobalBounds(window))
			{
				game = 0;
			}
		}*/

		//quit
		while (game == 3)
		{
			window.close();
		}

		/////////////////////////////////////////////////////////////////////////

	
		//start
		while (game == 1)
		{
			deltaTime = clock.restart().asSeconds();
			if (deltaTime > 1.0f / 20.0f)
				deltaTime = 1.0f / 20.0f;

			while (window.pollEvent(evnt))
			{
				switch (evnt.type)
				{
				case sf::Event::Closed:
					window.close();
					break;
					/*case sf::Event::Resized:
						ResizeView(window, view);
						break;*/
				}

			}


			//ล็อคฉากไม่ให้เลื่อนเกินขอบซ้าย
			if (player.GetPosition().x >= 900 && player.GetPosition().x <= 6223)
			{
				/*view.setCenter(player.GetPosition().x, 540);*/
				player.GetPosition().x, 540;
			}


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





			window.clear();

			/*window.setView(view);*/
			window.draw(bg[0]);
			window.draw(bg[1]);

			//วาดฉากเลื่อน
			for (Background& background : backgrounds)
				background.Draw(window);

			player.Draw(window);

			for (Platform& platform : platforms)
				platform.Draw(window);

			window.display();

			
		}

		return 0;
	}
}

