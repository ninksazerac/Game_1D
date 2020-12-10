#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <algorithm>
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
//save score ให้เอาไปไว้ในหน้า strat
void savescore() 
{
	std::map<int, std::string, std::greater<int>> mapscore;

	std::fstream file;
	file.open("Resource/Score/score.txt");

	for (auto itr = mapscore.begin(); itr != mapscore.end(); ++itr)
	{
		file << itr->first << "\n";//score
		file << itr->second << "\n";//user
	}
	file.close();
}
//หน้า game over
std::map<int, std::string> Load() 
{
	std::map<int, std::string> score;
	std::fstream file;
	file.open("Resource/Score/score.txt");

	for (int i = 0; i < 5; ++i)
	{
		std::pair<int, std::string>data;
		file >> data.first;
		file >> data.second;
		score.insert(data);
	}
	file.close();
	return score;
}
//เอามาแสดงหน้าscore
void displayscores()
{
	auto listscore = Load();
	for (auto itr = listscore.begin(); itr != listscore.end(); ++itr)
	{
		std::string user = itr->second;
		int score = itr->first;

		/*displayname1(user, score);
		displayname2(user, score);
		displayname3(user, score);
		displayname4(user, score);
		displayname5(user, score);*/
	}

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

	/////////////////////////////////Hp/////////////////////////////////
	//ประกาศตัวแปร
	int hit = 0;
	int hitagain = 0;
	//เซ็ต hp
	sf::RectangleShape hp(sf::Vector2f(280.0f, 60.f));
	sf::Texture hpplay;
	hpplay.loadFromFile("Resource/Hp/hp1.png");
	hp.setTexture(&hpplay);
	hp.setPosition(30, 30);


	////////////////////////////Score
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
	sf::Texture backtomenuTexture;
	backtomenuTexture.loadFromFile("Resource/Menu/backtomenu.png");
	//bg Gameover
	sf::Texture bggameoverTexture;
	bggameoverTexture.loadFromFile("Resource/Menu/gameover.png");
	//bg Score
	sf::Texture bgscoreTexture;
	bgscoreTexture.loadFromFile("Resource/Score/scorebg.png");

	Menu bgmenu(&bgmenuTexture, sf::Vector2f(1.0f , 1.0f), sf::Vector2f(0.0f , 0.0f));
	Menu bgscore(&bgscoreTexture, sf::Vector2f(1.0f, 1.0f), sf::Vector2f(0.0f, 0.0f));
	Menu start(&startTexture, sf::Vector2f(0.5f , 0.5f), sf::Vector2f(710.0f , 350.0f));
	Menu score(&scoreTexture, sf::Vector2f(0.5f , 0.5f), sf::Vector2f(720.0f , 500.0f));
	Menu quit(&quitTexture, sf::Vector2f(0.5f , 0.5f), sf::Vector2f(720.0f, 650.0f));
	Menu back(&backTexture, sf::Vector2f(0.3f, 0.3f), sf::Vector2f(1500.0f, 900.0f));
	Menu backtomenu(&backtomenuTexture, sf::Vector2f(0.3f, 0.3f), sf::Vector2f(710.0f, 900.0f));
	Menu bggameover(&bggameoverTexture, sf::Vector2f(1.0f, 1.0f), sf::Vector2f(0.0f, 0.0f));

	

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

	Player player(&playerTexture, sf::Vector2u(5, 6), 0.3f, 400.0f, 200.0f);


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

	int enemySpawnTimer1 = 0;
	int enemySpawnTimer2 = 0;
	int enemySpawnNext1 = 260;
	int enemySpawnNext2 = 200;

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
	int shootTimer1 = 0;
	int shootTimer2 = 0;
	//bullet1
	sf::RectangleShape bullet1(sf::Vector2f(20.0f, 30.f));
	sf::Texture projectilespic1;
	projectilespic1.loadFromFile("Resource/Weapon/weapon.png");
	bullet1.setTexture(&projectilespic1);
	std::vector<RectangleShape> bullets1;
	bullets1.push_back(RectangleShape(bullet1));//วาดกระสุน ปิ๊วๆ

	/////////////////////////////////Item/////////////////////////////////
	sf::RectangleShape itemheart(sf::Vector2f(20.0f, 30.0f));
	sf::Texture itemheartTexture;
	itemheartTexture.loadFromFile("Resource/Item/itemheart.png");
	itemheart.setTexture(&itemheartTexture);
	itemheart.setPosition(1000, 450);
	std::vector < RectangleShape> hearts;
	hearts.push_back(RectangleShape(itemheart));
	//
	sf::Clock Itemclock;
	sf::RectangleShape item(sf::Vector2f(20.0f, 30.0f));  // ของ monepic
	sf::Texture allitem;
	allitem.loadFromFile("Resource/Item/itemheart.png");
	item.setTexture(&allitem);
	item.setPosition({ 1000, 450 });

	std::vector<RectangleShape> items; //items
	items.push_back(RectangleShape(item));
	float ItemDelay = Itemclock.getElapsedTime().asSeconds();
	int itemSpawnTimer = 0;
	int rand_item;

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

		//game over
		while (game == 4)
		{
			
			bggameover.Draw(window);
			backtomenu.Draw(window);
			window.display();
			if (backtomenu.getGlobalBounds(window)) {
				backtomenu.setScale(sf::Vector2f(0.5f, 0.5f));
			}
			else {
				backtomenu.setScale(sf::Vector2f(0.4f, 0.4f));
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (backtomenu.getGlobalBounds(window)) {
					game = 0;
				}
			}
			
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


			//bullet1
			if (shootTimer1 < 30)
				shootTimer1++;
			if (Keyboard::isKeyPressed(Keyboard::E) && shootTimer1 >= 30) //กดยิงงง
			{
				bullet1.setPosition(playerCenter);
				bullets1.push_back(RectangleShape(bullet1));
				shootTimer1 = 0;
			}

			for (size_t i = 1; i < bullets1.size(); i++)
			{
				bullets1[i].move(15.f, 0.f);

				if (bullets1[i].getPosition().x <= 15)
					bullets1.erase(bullets1.begin() + i);
				bullets1[i].move(-10.f, 0.f);
			}

			
			//enemy
			//mon 1
			//if (scoreCount <= 49)
			{
				if (enemySpawnTimer1 < 30)
				{
					enemySpawnTimer1++;
				}

				if (enemySpawnTimer1 >= enemySpawnNext1)
				{
					enemy1.setPosition(Vector2f(2000, 470));
					enemies1.push_back(RectangleShape(enemy1));
					enemySpawnTimer1 = 0;
					enemySpawnNext1 = 130 + rand() % 130;
				}
				enemySpawnTimer1++;
			}
			for (size_t i = 1; i < enemies1.size(); i++)
			{
				if (scoreCount >= 100) { enemies1[i].move(-1.5f, 0.0f); }
				if (scoreCount >= 200) { enemies1[i].move(-3.0f, 0.0f); }
				enemies1[i].move(-5.f, 0.f);
				if (player.GetGlobal().intersects(enemies1[i].getGlobalBounds()))
				{
					enemies1.erase(enemies1.begin() + i);
					//ชนแล้วเลือดลด
					if (hit == 0)
					{
						hit++;
						hitagain += hit;
						hpplay.loadFromFile("Resource/Hp/hp2.png");
					}
					if (hitagain == 2)
					{
						hpplay.loadFromFile("Resource/Hp/hp3.png");
					}
					if (hitagain == 3)
					{
						hpplay.loadFromFile("Resource/Hp/hp4.png");
					}
					if (hitagain == 4)
					{
						hpplay.loadFromFile("Resource/Hp/hp5.png");
					}
					if (hitagain == 5)
					{
						hpplay.loadFromFile("Resource/Hp/hp6.png");
					}
					if (hitagain == 6)
					{
						//game over
						
						game = 4;
					}
					
				}
				


				/*if (enemies1[i].getPosition().x > window.getSize().x);
					enemies1.erase(enemies1.begin() + i);*/
			}

			//mon 2
			if (scoreCount >= 50)
			{
				if (enemySpawnTimer2 < 50)
				{
					enemySpawnTimer2++;
				}

				if (enemySpawnTimer2 >= enemySpawnNext2)
				{
					enemy2.setPosition(Vector2f(2000, 470));
					enemies2.push_back(RectangleShape(enemy2));
					enemySpawnTimer2 = 0;
					enemySpawnNext2 = 100 + rand() % 100;
				}
				enemySpawnTimer2++;
			}
			for (size_t i = 1; i < enemies2.size(); i++)
			{
				if (scoreCount >= 150) { enemies2[i].move(-3.0f, 0.0f); }
				if (scoreCount >= 200) { enemies2[i].move(-4.0f, 0.0f); }
				enemies2[i].move(-5.f, 0.f);
				if (player.GetGlobal().intersects(enemies2[i].getGlobalBounds()))
				{
					enemies2.erase(enemies2.begin() + i);
					//ชนแล้วเลือดลด
					if (hit == 0)
					{
						hit++;
						hitagain += hit;
						hpplay.loadFromFile("Resource/Hp/hp2.png");
					}
					if (hitagain == 2)
					{
						hpplay.loadFromFile("Resource/Hp/hp3.png");
					}
					if (hitagain == 3)
					{
						hpplay.loadFromFile("Resource/Hp/hp4.png");
					}
					if (hitagain == 4)
					{
						hpplay.loadFromFile("Resource/Hp/hp5.png");
					}
					if (hitagain == 5)
					{
						hpplay.loadFromFile("Resource/Hp/hp6.png");
			
					}
					if (hitagain == 6)
					{
						//game over
						game = 4;
					}

				}

				
				/*if (enemies1[i].getPosition().x > window.getSize().x);
					enemies1.erase(enemies1.begin() + i);*/
			}

			//Collision
			//mon 1 b.1
				for (size_t i = 1; i < bullets1.size(); i++)
				{
					for (size_t k = 1; k < enemies1.size(); k++)
					{
						if (bullets1[i].getGlobalBounds().intersects(enemies1[k].getGlobalBounds()))
						{
							bullets1.erase(bullets1.begin() + i);
							enemies1.erase(enemies1.begin() + k);
							scoreCount += 5;
							break;
						}
					}
				}
			//mon 2 b.2
				for (size_t i = 1; i < bullets1.size(); i++)
				{
					for (size_t k = 1; k < enemies2.size(); k++)
					{
						if (bullets1[i].getGlobalBounds().intersects(enemies2[k].getGlobalBounds()))
						{
							bullets1.erase(bullets1.begin() + i);
							enemies2.erase(enemies2.begin() + k);
							scoreCount += 10;
							break;
						}
					}
				}

				hit = 0;
			/////////////////////////////////Item/////////////////////////////////

			if (itemSpawnTimer > 10)
			{
				rand_item = rand() % 5; // สุ่มให้ไอเท็มเกิด

				if (rand_item == 0) { allitem.loadFromFile("Resource/Item/itemheart.png"); }
				if (rand_item == 1) { allitem.loadFromFile("Resource/Item/itemdead.png"); }
				if (rand_item == 2) { allitem.loadFromFile("Resource/Item/itemscoreup.png"); }
				if (rand_item == 3) { allitem.loadFromFile("Resource/Item/itemscoredown.png"); }

			}
			int rand_x = (rand() % 800 ) + 100;

			if (itemSpawnTimer >= 20 && ItemDelay > 10)
			{
				item.setPosition(Vector2f(rand_x, 470.0f));
				items.push_back(RectangleShape(item));//วาดไอเทม
				itemSpawnTimer = 0;
				ItemDelay = Itemclock.restart().asSeconds();

			}

			itemSpawnTimer++;
			printf("%f\n", ItemDelay);

			for (size_t d = 1; d < items.size(); d++)
			{
				items[d].move(-5.0f, 0.0f);

				//if (items[d].getPosition().y > window.getSize().y)  // สปอน มอนให้เกิด
				//{
				//	items.erase(items.begin() + d);
				//	break;
				//}

				if (player.GetGlobal().intersects(items[d].getGlobalBounds()))
				{
					items.erase(items.begin() + d);
					// เก็บได้หัวใจ
					if (rand_item == 0) 
					{
						
						if (hitagain == 1) {
							hpplay.loadFromFile("Resource/Hp/hp1.png");
							hitagain--;
						}

						if (hitagain == 2) {   // เซ้ทการชนแล้วเลือดค่อยๆลด
							hpplay.loadFromFile("Resource/Hp/hp2.png");
							hitagain--;
						}

						if (hitagain == 3) {
							hpplay.loadFromFile("Resource/Hp/hp3.png");
							hitagain--;
						}

						if (hitagain == 4) {
							hpplay.loadFromFile("Resource/Hp/hp4.png");
							hitagain--;
						}

						if (hitagain == 5) {
							hpplay.loadFromFile("Resource/Hp/hp5.png");
							hitagain--;
						}

						if (hitagain == 6) {
							hpplay.loadFromFile("Resource/Hp/hp6.png");
							hitagain--;
						}
						if (hitagain == 7) {
							game = 4;
						}
					}
					// เก็บได้ dead  ตายเลย
					if (rand_item == 1) {    
						game = 4;

					}
					// เก็บได้ score++
					if (rand_item == 2) {
						scoreCount += 20;  
					}
					// เก็บได้ score--
					if (rand_item == 3) {
						scoreCount -= 10;  
					}



					ItemDelay = Itemclock.restart().asSeconds();
					break;
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
			//วาด hp
			window.draw(hp);
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
			for (size_t d = 1; d < items.size(); d++)
			{
				window.draw(items[d]);
			}
			for (size_t i = 1; i < bullets1.size(); i++)
			{
				window.draw(bullets1[i]);
			}
			
			
			//////////////////////////////////////////////////////////////////////////////////////////

			//ล็อคฉากไม่ให้เลื่อนเกินขอบซ้าย
			if (player.GetPosition().x >= 900 && player.GetPosition().x <= 6223)
			{
				//view.setCenter(player.GetPosition().x, 540);
				//player.GetPosition().x, 540;
				sf::Vector2f pos = player.GetPosition();
				pos.x = std::min(pos.x, 6223.0f);
				pos.x = std::max(pos.x, 900.0f);
				player.setPosition(pos);
			}

			for (Platform& platform : platforms)
				platform.Draw(window);

			

			window.display();

		}

		
	}

	return 0;
}