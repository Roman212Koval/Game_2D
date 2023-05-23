#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <string>
#include <fstream>
//#include <iomanip>
//#include <conio.h>
//#include <iostream>

using namespace sf;
using namespace std;

const int H = 20;
const int W = 109;


class screen 
{

public:
	void next_level(RenderWindow& window, int lv )
{
	
	Texture winlevel, losslevel;
	winlevel.loadFromFile("level.png");
	losslevel.loadFromFile("game_over.png");
	Sprite win(winlevel), loss(losslevel);
	win.setPosition(0, 0);
	loss.setPosition(0, 0);

	Music music1,music2;
	music1.openFromFile("win.ogg");
	music2.openFromFile("gameover.ogg");

	Clock clock1;
	int time = clock1.getElapsedTime().asSeconds();
	bool next_boll = 1;
	if(lv == 1)music1.play();
	if (lv == 2) music2.play();
	while(next_boll)
	{
		time = clock1.getElapsedTime().asSeconds();
		if(lv==1)
		{
			win.setColor(Color::Yellow);
			if(time%2==0) win.setColor(Color::Red);
			window.draw(win);
			window.display();
		}
		else
		{
		
			window.draw(loss);
			window.display();
		}
		if (Keyboard::isKeyPressed(Keyboard::Enter)) next_boll = 0;
		window.clear(Color(107, 140, 255));
	}
	
}
void menu(RenderWindow& window) {
	

	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("play.png");
	menuTexture2.loadFromFile("about.png");
	menuTexture3.loadFromFile("exit.png");
	aboutTexture.loadFromFile("about_game.png");
	menuBackground.loadFromFile("megaman.jpg");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(10, 30);
	menu2.setPosition(10, 90);
	menu3.setPosition(10, 150);
	menuBg.setPosition(245, 10);

	//////////////////////////////МЕНЮ///////////////////
	while (isMenu)
	{
		
		//window.draw(text);
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);
		menuNum = 0;
		window.clear(Color(129, 181, 221));

		if (IntRect(100, 30, 180, 50).contains(Mouse::getPosition(window))) {  menu1.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(100, 90, 180, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menuNum = 2; }
		if (IntRect(100, 150, 180, 50).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Blue); menuNum = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1)  isMenu = false;//если нажали первую кнопку, то выходим из меню 
			if (menuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
			if (menuNum == 3) { window.close(); isMenu = false; }

		}
		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);
		
		window.display();
		
	}
	////////////////////////////////////////////////////
	window.clear(Color(107, 140, 255));
	

}
};

float offsetX = 0, offsetY = 0;





class PLAYER {

public:
	int m = 0;
	string *TileMap =new string [W];


	float dx, dy;
	FloatRect rect;
	bool onGround;
	Sprite sprite;
	float currentFrame;

	PLAYER(sf::Texture& image)
	{
		sprite.setTexture(image);
		rect = FloatRect(7 * 32, 9 * 32, 22, 38);

		dx = dy = 0.1;
		currentFrame = 0;
	}


	void update(float time)
	{
		
		rect.left += dx * time;
		Collision(0);

		if (!onGround) dy = dy + 0.0005 * time;
		rect.top += dy * time;
		onGround = false;
		Collision(1);


		currentFrame += 0.005 * time;
		if (currentFrame > 6) currentFrame -= 6;

		currentFrame += 0.009 * time;
		if (currentFrame > 9) currentFrame -= 9;
		if (dx > 0)
		{
			if (currentFrame < 2) { sprite.setTextureRect(IntRect(111 + 22 * int(currentFrame), 70, 22, 38)); }                               //1-2
			if (currentFrame <= 3.5 && currentFrame > 2) { sprite.setTextureRect(IntRect(155 + 34 * (int(currentFrame) - 2), 70, 34, 38)); }       //3-4
			if (currentFrame <= 6 && currentFrame > 3.5) { sprite.setTextureRect(IntRect(222 + 26 * (int(currentFrame) - 3), 70, 23, 38)); }     //5-7
			if (currentFrame < 9 && currentFrame > 6) { sprite.setTextureRect(IntRect(296 + 32 * (int(currentFrame) - 6), 70, 32, 38)); }    //8-10
		}
		if (dx < 0)
		{
			if (currentFrame < 2) { sprite.setTextureRect(IntRect(111 + 22 * int(currentFrame) + 22, 70, -22, 38)); }
			if (currentFrame <= 3 && currentFrame >= 2) { sprite.setTextureRect(IntRect(155 + 34 * (int(currentFrame) - 2) + 34, 70, -34, 38)); }
			if (currentFrame <= 6 && currentFrame > 3) { sprite.setTextureRect(IntRect(222 + 26 * (int(currentFrame) - 3) + 23, 70, -23, 38)); }
			if (currentFrame < 9 && currentFrame > 7) { sprite.setTextureRect(IntRect(296 + 32 * (int(currentFrame) - 7) + 32, 70, -32, 38)); }
		}



		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

		dx = 0;
	}



	void Collision(int dir)
	{
		
		
		for (int i = rect.top / 32; i < (rect.top + rect.height) / 32; i++)
			for (int j = rect.left / 32; j < (rect.left + rect.width) / 32; j++)
			{
				if (TileMap[i][j] == 'B' || TileMap[i][j] == 'H')
				{
					if ((dx > 0) && (dir == 0)) rect.left = j * 32 - rect.width;
					if ((dx < 0) && (dir == 0)) rect.left = j * 32 + 32;
					if ((dy > 0) && (dir == 1)) { rect.top = i * 32 - rect.height;  dy = 0;   onGround = true; }
					if ((dy < 0) && (dir == 1)) { rect.top = i * 32 + 32;   dy = 0; }
				}

				if (TileMap[i][j] == '0')
				{
					TileMap[i][j] = ' '; m = 1;
				}

			}

	}
};

int main()
{	
	
	RenderWindow window(VideoMode(900,400), "game");
	screen my;
	my.menu(window);
	
	SoundBuffer  shotbooffer; 
	SoundBuffer jumpbooffer;
	shotbooffer.loadFromFile("pop.ogg");
	jumpbooffer.loadFromFile("jump1.ogg");
	Sound shoot(shotbooffer);
	Sound jump (jumpbooffer);
	shoot.setVolume(25);

	Music music;
	music.openFromFile("music.ogg");
	
	music.setVolume(25);

	bool end = true;
	int ball = 0;
	Font font;
	font.loadFromFile("kyrill.ttf");
	Text text("", font, 20);
	text.setFillColor(Color::Red);
	text.setStyle(Text::Bold);

	Texture t;
	t.loadFromFile("game.png");
	Sprite tile(t);
	PLAYER p(t);


	Clock clock;
	Clock gameTimecl;
	int gameTime =1;
	int min=1, sec=1;

	int c = 0;
	int mapnum = 0;
	

	while (end)
	{	
		if (ball == 0) { mapnum += 1; }
		if (!window.isOpen()  ) end = false;
		if (mapnum == 5)  mapnum = 1;
	
		ball = 10; gameTime = 0; c=0;
		gameTimecl.restart(); p.rect = FloatRect(7 * 32, 9 * 32, 22, 38);
		 offsetX = 2, offsetY = 0;
		p.rect.left = 7 * 32; p.rect.top = 9 * 32; p.sprite.setPosition(7 * 32, 9 * 32);
		window.draw(tile);
		window.display();
		
		std::string str = {"ffffff"};
		std::ifstream file;
		if (mapnum == 1) file.open("map1.txt");
		if (mapnum == 2) file.open("map2.txt");
		if (mapnum == 3) file.open("map3.txt");
		if (mapnum == 4) file.open("map4.txt");
		music.play();
		
		
		while (!file.eof())
		{
			getline(file, str);
			p.TileMap[c] = str;
			c += 1 ;
		}
		
		file.close();

		while (window.isOpen() && gameTime != 179)
		{
			gameTime = gameTimecl.getElapsedTime().asSeconds();
			min = 2 - (gameTime - (gameTime % 60)) / 60;
			sec = 59 - gameTime + 60 * (2 - min);

			std::ostringstream ScoreString, Time;
			ScoreString << ball;
			Time << min << ":" << sec;
			text.setString("ball:" + ScoreString.str() + "\nTime:" + Time.str());
			text.setPosition(50, 350);

			float currentFrame = 0;


			if (p.m == 1) { shoot.play(); ball -= 1; }p.m = 0;
			if (Keyboard::isKeyPressed(Keyboard::Escape)) { music.pause(); my.menu(window); music.play(); }
			float time = clock.getElapsedTime().asMicroseconds();
			clock.restart();



			time = time / 10;

			if (time > 20) time = 20;

			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					window.close();
			}

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				p.dx = -0.2;
			}

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				p.dx = 0.2;
			}

			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				if (p.onGround) {
					p.dy = -0.35;
					p.onGround = false;
					jump.play();
				}
			}

			p.update(time);

			if (p.rect.left > 450) { offsetX = p.rect.left - 450; }
			if(p.rect.top >200 && p.rect.top < 450){ offsetY = p.rect.top - 200;}

			window.clear(Color(107, 140, 255));


			for (int i = 0; i < H; i++) {
				for (int j = 0; j < W; j++)
				{
					if (p.TileMap[i][j] == 'B') { tile.setTextureRect(IntRect(481, 129, 32, 32)); }

					if (p.TileMap[i][j] == '0')  tile.setTextureRect(IntRect(327, 161, 32, 32));

					if (p.TileMap[i][j] == 'W')  tile.setTextureRect(IntRect(336, 275, 32, 32));

					if (p.TileMap[i][j] == 'H')  tile.setTextureRect(IntRect(433, 113, 32, 32));

					if (p.TileMap[i][j] == ' ') { continue; }

					tile.setPosition(j * 32 - offsetX, i * 32 - offsetY);
					window.draw(tile);
				}
			}
			window.draw(p.sprite);
			window.draw(text);
			window.display();
			if (ball == 0) { gameTime = 179; }
		}
		if (ball == 0) my.next_level(window,1);
		if (ball != 0 && gameTime ==179) my.next_level(window, 2);
	}
	return 0;
}



