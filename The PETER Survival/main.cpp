#define _CRT_SECURE_NO_WARNINGS
#include <SFML\Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include "Player.h"
#include "Platform.h"
#include "Background.h"
#include "Menu.h"
#include "TextBox.h"

int getRand(int min, int max) {
	float f = rand() / (float)RAND_MAX;
	return min + (max - min) * f;
}

size_t GetFileSize(FILE* stream) {
	if (stream == nullptr) return 0;
	int sg = ftell(stream);
	fseek(stream, 0, SEEK_END);
	int pos = ftell(stream);
	fseek(stream, sg, SEEK_SET);
	return pos;
}

void LoadHighscoreList(std::vector<std::pair<int, std::string>>* pListScore) {
	FILE* file;
	char temp[25];
	std::string nameArr[6];
	int scoreArr[6];
	bool collectHS = false;        /// r คือ read file
	file = fopen("score.dat", "r");
	if (file) {
		if (GetFileSize(file) > 0) {
			for (int i = 0; i < 5 && !feof(file); i++) {
				if (!fscanf(file, "%s %d\n", temp, &scoreArr[i]))
					break;
				nameArr[i] = temp;
				pListScore->push_back(std::make_pair(scoreArr[i], nameArr[i]));
			}
		}
		fclose(file);
	}
}

static const float VIEW_HEIGHT = 900.0f;
void ResizeView(const sf::RenderWindow& window, sf::View& view) //set display ให้คงที่เวลาขยาย หด
{
	float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
	view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT);
}

static void SavePlayerData(const std::map<int, std::string, std::greater<int>>& data)
{
	std::ofstream file;//output file
	file.open("score.dat");

	if (!file.is_open()) return;
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		file.write((char*)&itr->first, sizeof(int));//first คือ score

		size_t strSize = itr->second.size();
		file.write((char*)&strSize, sizeof(size_t));
		file.write(itr->second.data(), strSize);
	}

	file.close();
}

static std::map<int, std::string, std::greater<int>> LoadPlayerData()
{
	std::map<int, std::string, std::greater<int>> res;

	std::ifstream file;//input file
	file.open("score.dat");

	if (!file.is_open()) return res;

	while (!file.eof())
	{
		std::pair<int, std::string> pair;

		int score;
		file.read((char*)&score, sizeof(int));

		size_t strSize;
		std::string userName;
		file.read((char*)&strSize, sizeof(size_t));
		userName.resize(strSize);

		pair.first = score;
		pair.second = userName;
		res.insert(pair);
	}
}
//bool game = true;
struct Item {
	sf::RectangleShape obj;
	int type;
};

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

	//////////////////////////// username , score

	sf::Text username;
	username.setFont(word);
	username.setFillColor(sf::Color::White);
	username.setCharacterSize(70);
	username.setPosition(350, 680);

	sf::Text scoreplayer;
	scoreplayer.setFont(word);
	scoreplayer.setFillColor(sf::Color::White);
	scoreplayer.setCharacterSize(70);
	scoreplayer.setPosition(1400, 680);

	//////////////////////////////// text name /////////////////////////////////////////

	sf::Text usernames_all[5];
	sf::Text scoreplayers_all[5];
	//name
	usernames_all[0].setFont(word);
	usernames_all[0].setCharacterSize(50);
	usernames_all[0].setPosition(660, 250);

	usernames_all[1].setFont(word);
	usernames_all[1].setCharacterSize(50);
	usernames_all[1].setPosition(660, 350);

	usernames_all[2].setFont(word);
	usernames_all[2].setCharacterSize(50);
	usernames_all[2].setPosition(660, 450);

	usernames_all[3].setFont(word);
	usernames_all[3].setCharacterSize(50);
	usernames_all[3].setPosition(660, 550);

	usernames_all[4].setFont(word);
	usernames_all[4].setCharacterSize(50);
	usernames_all[4].setPosition(660, 650);

	// score
	scoreplayers_all[0].setFont(word);
	scoreplayers_all[0].setCharacterSize(50);
	scoreplayers_all[0].setPosition(1250, 250);

	scoreplayers_all[1].setFont(word);
	scoreplayers_all[1].setCharacterSize(50);
	scoreplayers_all[1].setPosition(1250, 350);

	scoreplayers_all[2].setFont(word);
	scoreplayers_all[2].setCharacterSize(50);
	scoreplayers_all[2].setPosition(1250, 450);

	scoreplayers_all[3].setFont(word);
	scoreplayers_all[3].setCharacterSize(50);
	scoreplayers_all[3].setPosition(1250, 550);

	scoreplayers_all[4].setFont(word);
	scoreplayers_all[4].setCharacterSize(50);
	scoreplayers_all[4].setPosition(1250, 650);


	/////////////////////////////////Sound/////////////////////////////////
	sf::Music bgsound1;
	if (!bgsound1.openFromFile("Resource/Sound/sound2.wav"))
	{
		printf("error1\n");
	}
	bgsound1.setVolume(10);
	bgsound1.setLoop(true);
	bgsound1.play();
	

	//click
	sf::SoundBuffer soundclick;
	soundclick.loadFromFile("Resource/Sound/click.wav");
	sf::Sound soundclicks;
	soundclicks.setVolume(50.f);
	soundclicks.setBuffer(soundclick);


	//ยิง
	sf::SoundBuffer soundshoot;
	soundshoot.loadFromFile("Resource/Sound/shoots.wav");
	sf::Sound soundshoots;
	soundshoots.setBuffer(soundshoot);

	//hp up
	sf::SoundBuffer soundhpup;
	soundhpup.loadFromFile("Resource/Sound/hpup.wav");
	sf::Sound soundhpups;
	soundhpups.setBuffer(soundhpup);

	//score up
	sf::SoundBuffer soundscoreup;
	soundscoreup.loadFromFile("Resource/Sound/scoreup.wav");
	sf::Sound soundscoreups;
	soundscoreups.setBuffer(soundscoreup);

	//ชน + score down
	sf::SoundBuffer soundscoredown;
	soundscoredown.loadFromFile("Resource/Sound/colider+scoredown.wav");
	sf::Sound soundscoredowns;
	soundscoredowns.setBuffer(soundscoredown);

	//game over
	sf::SoundBuffer soundgameover;
	soundgameover.loadFromFile("Resource/Sound/gameover.wav");
	sf::Sound soundgameovers;
	soundgameovers.setBuffer(soundgameover);


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

	Menu bgmenu(&bgmenuTexture, sf::Vector2f(1.0f, 1.0f), sf::Vector2f(0.0f, 0.0f));
	Menu bgscore(&bgscoreTexture, sf::Vector2f(1.0f, 1.0f), sf::Vector2f(0.0f, 0.0f));
	Menu start(&startTexture, sf::Vector2f(0.5f, 0.5f), sf::Vector2f(710.0f, 350.0f));
	Menu score(&scoreTexture, sf::Vector2f(0.5f, 0.5f), sf::Vector2f(720.0f, 500.0f));
	Menu quit(&quitTexture, sf::Vector2f(0.5f, 0.5f), sf::Vector2f(720.0f, 650.0f));
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
	sf::RectangleShape itemheart(sf::Vector2f(50.0f, 50.0f));
	sf::Texture itemheartTexture;
	itemheartTexture.loadFromFile("Resource/Item/itemheart.png");
	itemheart.setTexture(&itemheartTexture);
	itemheart.setPosition(1000, 450);
	//
	sf::Clock Itemclock;

	//sf::RectangleShape item(sf::Vector2f(20.0f, 30.0f));  // ของ monepic
	sf::Texture textureItem1;
	textureItem1.loadFromFile("Resource/Item/itemheart.png");
	sf::Texture textureItem2;
	textureItem2.loadFromFile("Resource/Item/itemdead.png");
	sf::Texture textureItem3;
	textureItem3.loadFromFile("Resource/Item/itemscoreup.png");
	sf::Texture textureItem4;
	textureItem4.loadFromFile("Resource/Item/itemscoredown.png");

	//item.setTexture(&allitem);
	//item.setPosition({ 1000, 450 });

	std::vector<Item> items; //items
	//items.push_back(RectangleShape(item));

	float ItemDelay = Itemclock.getElapsedTime().asSeconds();
	int itemSpawnTimer = 0;

	/////////////////////////////////input name/////////////////////////////
	sf::RectangleShape input_name(sf::Vector2f(1920.0f, 1080.0f));
	sf::Texture nameplayerinput;
	nameplayerinput.loadFromFile("Resource/Username/bglogin.png");
	input_name.setTexture(&nameplayerinput);
	input_name.setPosition({ 0,0 });  // ตำแหน่งวงกลม (x,y)


	/////////////////////////////////Highscore/////////////////////////////////
	std::vector<std::pair<int, std::string>> highScore;
	bool collectHS = false;        /// r คือ read file
	LoadHighscoreList(&highScore);

	//ใส่ username
	TextBox input(70, Color::Black , &word, Vector2f(850.0f, 600.0f), true, true, 7);
	string user_name;


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

	auto _Refresh = [&]() {
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) {
					window.close();
					break;
				}
				/*if (event.type == sf::Event::TextEntered) {
					if (event.text.unicode < 128)
					{
						std::map<int, std::string, std::greater<int>>& data
					}
				}*/
			}
		}
		window.clear();
	};

	//เปิดเกมมา
	while (window.isOpen())
	{
		while (game == 0)
		{
			_Refresh();

			//วาดหน้าเมนู
			/*bgsound1.play();*/
			bgmenu.Draw(window);
			start.Draw(window);
			score.Draw(window);
			quit.Draw(window);

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
					soundclicks.play();

					scoreCount = 0;

					enemies1.clear();
					enemies2.clear();
					bullets1.clear();
					items.clear();

					hit = 0;
					hitagain = 0;
					hpplay.loadFromFile("Resource/Hp/hp1.png");
					hp.setTexture(&hpplay);

					player = Player(&playerTexture, sf::Vector2u(5, 6), 0.3f, 400.0f, 200.0f);
					game = 5;
				}
				else if (score.getGlobalBounds(window)) {
					soundclicks.play();
					game = 2;
				}
				else if (quit.getGlobalBounds(window)) {
					soundclicks.play();
					game = 3;
				}
			}

			window.display();
		}

		//score 
		while (game == 2)
		{
			_Refresh();
			/*bgsound.pause();*/
			highScore.clear();
			LoadHighscoreList(&highScore);

			bgscore.Draw(window);
			back.Draw(window);

			for (int i = 0; i < highScore.size(); ++i) {
				usernames_all[i].setString(highScore[i].second);
				scoreplayers_all[i].setString(std::to_string(highScore[i].first));

				window.draw(usernames_all[i]);
				window.draw(scoreplayers_all[i]);
			}
			

			if (back.getGlobalBounds(window)) {
				back.setScale(sf::Vector2f(0.4f, 0.4f));
			}
			else {
				back.setScale(sf::Vector2f(0.3f, 0.3f));
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && back.getGlobalBounds(window))
			{
				soundclicks.play();
				
				game = 0;
			}

			window.display();
		}

		//quit
		if (game == 3)
		{
			window.close();
		}

		//game over

		while (game == 4)
		{
			_Refresh();
			bggameover.Draw(window);
			backtomenu.Draw(window);
			input.setSelected(true);

			username.setString(user_name);   // เอาชื่อมาแสดง
			scoreplayer.setString(to_string(scoreCount));   // เอา score มาแสดง
			window.draw(username);
			window.draw(scoreplayer);

			if (!collectHS) {  // เคสเก็บสกอกับชื่อ 
				highScore.clear();
				LoadHighscoreList(&highScore);

				if (user_name == "") {
					user_name = "-"; // กรณีไม่ใส่ชื่อ 
				}
				highScore.push_back(std::make_pair(scoreCount, user_name));  // เก็บค่าตัวเลข กับชื่อ player 

				struct _greater {
					using T = std::pair<int, std::string>;
					constexpr bool operator()(const T& _Left, const T& _Right) const {
						return _Left.first > _Right.first;
					}
				};
				std::sort(highScore.begin(), highScore.end(), _greater{});   // เรียงค่าจาก น้อยไปมาก score
				
				FILE* file = fopen("score.dat", "w");
				char temp[26];
				for (int i = 0; i < highScore.size(); i++) {
					strcpy(temp, highScore[i].second.c_str());
					fprintf(file, "%s %d\n", temp, highScore[i].first);
				}
				fclose(file);
				collectHS = true;
			}

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


			window.display();
		}

		//หน้า username
		while (game == 5)
		{
			input.setSelected(true);

			while (window.pollEvent(evnt))
			{
				if (evnt.type == Event::Closed) window.close();
				if (evnt.type == Event::TextEntered) input.update(evnt);
				if (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Enter)
				{
					user_name = input.getInput();
					input.clear();
					collectHS = false;
					input.setSelected(false);

					game = 1;
				}

			}
			window.draw(input_name);
			input.draw(window);
			
			window.display();
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))   // หกแล้วไปหน้าเมนูเลย
			{
				game = 0;
			}
			cout << user_name << endl;
		}




		/////////////////////////////////////////////////////////////////////////


		//start
		while (game == 1)
		{
			
			_Refresh();
			/*bgsound2.play();*/
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				soundclicks.play();
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
			if (Keyboard::isKeyPressed(Keyboard::Space) && shootTimer1 >= 30) //กดยิงงง
			{
				soundclicks.play();
				if (bullets1.size() < 4) {
					bullet1.setPosition(playerCenter);
					bullets1.push_back(RectangleShape(bullet1));
					
					shootTimer1 = 0;
					
				}
			}

			for (size_t i = 0; i < bullets1.size(); i++)
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
					enemySpawnNext1 = getRand(130, 260);
				}
				enemySpawnTimer1++;
			}
			for (size_t i = 0; i < enemies1.size(); i++)
			{
				if (scoreCount >= 100) { enemies1[i].move(-1.5f, 0.0f); }
				if (scoreCount >= 200) { enemies1[i].move(-3.0f, 0.0f); }
				enemies1[i].move(-5.f, 0.f);
				if (player.GetGlobal().intersects(enemies1[i].getGlobalBounds()))
				{
					enemies1.erase(enemies1.begin() + i);
					soundscoredowns.play();
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
						soundgameovers.play();
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
			for (size_t i = 0; i < enemies2.size(); i++)
			{
				if (scoreCount >= 150) { enemies2[i].move(-3.0f, 0.0f); }
				if (scoreCount >= 200) { enemies2[i].move(-4.0f, 0.0f); }
				enemies2[i].move(-5.f, 0.f);
				if (player.GetGlobal().intersects(enemies2[i].getGlobalBounds()))
				{
					enemies2.erase(enemies2.begin() + i);
					soundscoredowns.play();
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
						soundgameovers.play();
						
					}

				}


				/*if (enemies1[i].getPosition().x > window.getSize().x);
					enemies1.erase(enemies1.begin() + i);*/
			}

			//Collision
			//mon 1 b.1
			for (size_t i = 0; i < bullets1.size(); i++)
			{
				for (size_t k = 0; k < enemies1.size(); k++)
				{
					if (bullets1[i].getGlobalBounds().intersects(enemies1[k].getGlobalBounds()))
					{
						soundclicks.play();
						bullets1.erase(bullets1.begin() + i);
						enemies1.erase(enemies1.begin() + k);
						scoreCount += 5;

						break;
					}
				}
			}
			//mon 2 b.2
			for (size_t i = 0; i < bullets1.size(); i++)
			{
				for (size_t k = 0; k < enemies2.size(); k++)
				{
					if (bullets1[i].getGlobalBounds().intersects(enemies2[k].getGlobalBounds()))
					{
						soundclicks.play();
						bullets1.erase(bullets1.begin() + i);
						enemies2.erase(enemies2.begin() + k);
						scoreCount += 10;

						break;
					}
				}
			}

			hit = 0;
			/////////////////////////////////Item/////////////////////////////////

			if (itemSpawnTimer >= 10 *60) //10 120
			{
				int rand_x = getRand(1490, 1920);

				sf::Texture* pTexture = &textureItem1;
				float scale = 1;

				int rand_item = rand() % 5; // สุ่มให้ไอเท็มเกิด
				if (rand_item == 0) { pTexture = &textureItem1; scale = 1; }
				else if (rand_item == 1) { pTexture = &textureItem2; }
				else if (rand_item == 2) { pTexture = &textureItem3; scale = 1; }
				else if (rand_item == 3) { pTexture = &textureItem4; }

				Item newItem;
				newItem.obj = RectangleShape(sf::Vector2f(50.0f, 50.0f));
				newItem.type = rand_item;

				newItem.obj.setTexture(pTexture);
				newItem.obj.setPosition(Vector2f(rand_x, 470.0f));
				newItem.obj.setScale(scale, scale);
				items.push_back(newItem);//วาดไอเทม

				itemSpawnTimer = 0;
				ItemDelay = Itemclock.restart().asSeconds();

			}

			itemSpawnTimer++;
			//std::printf("%f\n", ItemDelay);

			for (size_t d = 0; d < items.size(); d++)
			{
				items[d].obj.move(-5.0f, 0.0f);

				//if (items[d].getPosition().y > window.getSize().y)  // สปอน มอนให้เกิด
				//{
				//	items.erase(items.begin() + d);
				//	break;
				//}

				if (player.GetGlobal().intersects(items[d].obj.getGlobalBounds()))
				{
					
					// เก็บได้หัวใจ
					if (items[d].type == 0)
					{
						soundhpups.play();
						
						if (hitagain == 1) {
							hpplay.loadFromFile("Resource/Hp/hp1.png");
							hitagain--;
						}

						if (hitagain == 2) {   
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
							soundgameovers.play();
						}
					}
					// เก็บได้ dead  ตายเลย
					if (items[d].type == 1) {
						game = 4;
						soundgameovers.play();
					}
					// เก็บได้ score++
					if (items[d].type == 2) {
						scoreCount += 20;
						soundscoreups.play();
					}
					// เก็บได้ score--
					if (items[d].type == 3) {
						scoreCount -= 10;
						soundscoredowns.play();
					}

					items.erase(items.begin() + d);

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

			std::map<int, std::string, std::greater<int>> scoreCount;
			window.draw(score1);
			window.draw(scoreText);

			///////////////////////////////// Draw shoots & enemies1 /////////////////////////////////
			for (size_t i = 0; i < enemies1.size(); i++)
			{
				window.draw(enemies1[i]);
			}
			for (size_t i = 0; i < enemies2.size(); i++)
			{
				window.draw(enemies2[i]);
			}
			for (size_t d = 0; d < items.size(); d++)
			{
				window.draw(items[d].obj);
			}
			for (size_t i = 0; i < bullets1.size(); i++)
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
			if (player.GetPosition().y > 1080) {
				sf::Vector2f pos = player.GetPosition();
				pos.y = 1080;
				player.setPosition(pos);
			}

			for (Platform& platform : platforms)
				platform.Draw(window);

			window.display();
		}

		
	}
	return 0;
}
