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

	Player player(&playerTexture, sf::Vector2u(5, 6), 0.3f, 400.0f, 200.0f);


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
		while (game == 0) {
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
			menu.Draw(window); 
			play.Draw(window);
			howto.Draw(window);
			highscore.Draw(window);
			exit.Draw(window);
			window.display();
			if (play.getGlobalBounds(window)) {
				play.setScale(sf::Vector2f(0.7f, 0.7f));
			}
			else {
				play.setScale(sf::Vector2f(0.6f, 0.6f));
			}
			if (howto.getGlobalBounds(window)) {
				howto.setScale(sf::Vector2f(0.7f, 0.7f));
			}
			else {
				howto.setScale(sf::Vector2f(0.6f, 0.6f));
			}
			if (highscore.getGlobalBounds(window)) {
				highscore.setScale(sf::Vector2f(0.7f, 0.7f));
			}
			else {
				highscore.setScale(sf::Vector2f(0.6f, 0.6f));
			}
			if (exit.getGlobalBounds(window)) {
				exit.setScale(sf::Vector2f(1.1f, 1.1f));
			}
			else {
				exit.setScale(sf::Vector2f(1.0f, 1.0f));
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (play.getGlobalBounds(window)) {
					game = 1;
				}
				else if (howto.getGlobalBounds(window)) {
					game = 2;
				}
				else if (highscore.getGlobalBounds(window)) {
					game = 3;
				}
				else if (exit.getGlobalBounds(window)) {
					game = 4;
				}
				/*else if (play.getGlobalBounds(window)) {
					game = 0;
				}*/
			}
		}

		while (game == 2) {
			howtoplay.Draw(window);
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
		}

		while (game == 3) { 
		
			hightscorebg.Draw(window);
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
		}

		while (game == 4) {
			window.close();
		}

		/////////////////////////////////////////////////////////////////////////

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

