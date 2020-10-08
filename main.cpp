#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <map>
#include "main.h"

using namespace sf;

float offsetX = 0, offsetY = 0;
double hp = 100;
Font font;

const int H = 30, W = 50;
int score = 0, positionJump1 = 0, positionJump2 = 0, counter = 0;

String TileMap[H];

class PLAYER {

public:

	float dx, dy, currentFrame;
	FloatRect rect;
	bool onGround, isCheckRight = true;
	Sprite sprite;


	PLAYER(Texture& image)
	{
		sprite.setTexture(image);
		rect = FloatRect(7 * 32, 22 * 32, 32, 60);
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
		if (currentFrame > 5) currentFrame -= 5; // ��������� �������
		if (currentFrame > 2) {
			positionHero(currentFrame, 44);   // ����������� ����� ���������� �������� ��� ���� 
		}
		else { positionHero(currentFrame, 41); }
		if ((!Keyboard::isKeyPressed(Keyboard::Right)) && (!Keyboard::isKeyPressed(Keyboard::Left)) && (!Keyboard::isKeyPressed(Keyboard::Up)))
		{
			if (isCheckRight) {
				sprite.setTextureRect(IntRect(39 * int(currentFrame), 1, 32, 60));
			}
			else {
				sprite.setTextureRect(IntRect(39 * int(currentFrame) + 32, 1, -32, 60));
			}
		}
		if (isCheckRight)
		{
			jump(1);
		}
		else {
			jump(0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			isCheckRight = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			isCheckRight = true;
		}
		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
		dx = 0;
	}

	void positionHero(float currentFrame, int pixel) {
		if (dx > 0) sprite.setTextureRect(IntRect(pixel * int(currentFrame), 74, pixel, 60));//��������� ��������000
		if (dx < 0) sprite.setTextureRect(IntRect(pixel * int(currentFrame) + pixel, 74, -pixel, 60));
	}

	void jump(int i) {
		int x = rect.left / 32;    int y = (rect.top / 32) + 2;
		if ((TileMap[y][x + i] != 'B') && (TileMap[y][x + i] != 'b'))
		{
			if (isCheckRight) {
				sprite.setTextureRect(IntRect(335, 273, 32, 60));
			}
			else {
				sprite.setTextureRect(IntRect(367, 273, -32, 60));
			}
		}
		checkHp(i);
	}

	void checkHp(int i) {
		int x, y = (rect.top / 32) + 2;
		if (isCheckRight) {x = (rect.left - 29) / 32;}
		else {x = (rect.left + 29) / 32;}
		if ((TileMap[y][x + i] != 'B') && (TileMap[y][x + i] != 'b'))  //�����
		{
			if (counter == 0){positionJump1 = rect.top;}
			positionJump2 = rect.top;
			counter++;
		}
		else {
			if (positionJump1 != 0)
			{
				int n = positionJump2 - positionJump1;
				int num = (positionJump2 - positionJump1 + 10) / 32;
				switch (num)
				{
				case -4: {hp -= 13;	break;}
				case 5: {hp -= 20; break;}
				case 6: {hp -= 28; break;}
				default: {
					if (num > 6) {
						int i = 5 + rand() % 3;
						hp -= num * i - rand() % 8;
					}
					break;
				}	   
				}
				positionJump1=positionJump2=counter= 0;
			}
		}
	}

	void Collision(int dir)  //��������� ��������� ������
	{
		for (int i = rect.top / 32; i < (rect.top + rect.height) / 32; i++)
			for (int j = rect.left / 32; j < (rect.left + rect.width) / 32; j++)
			{
				if ((TileMap[i][j] == 'B') || (TileMap[i][j] == 'b') || (TileMap[i][j] == 'R') || (TileMap[i][j] == 'r') || (TileMap[i][j] == '0'))
				{
					if ((dx > 0) && (dir == 0)) rect.left = j * 32 - rect.width;
					if ((dx < 0) && (dir == 0)) rect.left = j * 32 + 32;
					if ((dy > 0) && (dir == 1)) { rect.top = i * 32 - rect.height;  dy = 0;   onGround = true; }
					if ((dy < 0) && (dir == 1)) { rect.top = i * 32 + 32;   dy = 0; }
				}
				if (TileMap[i][j] == 'K')
				{
					TileMap[i][j] = '+';
					score++;
				}
			}
	}
};

Sprite dr(Sprite sp1, char TileMap) {
	switch (TileMap)
	{
	case 'B': {sp1.setTextureRect(IntRect(1, 0, 32, 32)); break; }
	case 'b': {sp1.setTextureRect(IntRect(1, 32, 32, 32)); break; }
	case '+': {sp1.setTextureRect(IntRect(97, 32, 32, 32)); break; }
	case '0': {sp1.setTextureRect(IntRect(97, 32, 32, 32)); break; }
	case '1': {sp1.setTextureRect(IntRect(65, 0, 32, 32)); break; }
	case '2': {sp1.setTextureRect(IntRect(65, 32, 32, 32)); break; }
	case '3': {sp1.setTextureRect(IntRect(33, 0, 32, 32)); break; }
	case '4': {sp1.setTextureRect(IntRect(33, 32, 32, 32)); break; }
	case '5': {sp1.setTextureRect(IntRect(97, 0, 32, 32)); break; }
	case 'R': {sp1.setTextureRect(IntRect(161, 0, 32, 32)); break; }
	case 'r': {sp1.setTextureRect(IntRect(161, 32, 32, 32)); break; }
	case 'K': {sp1.setTextureRect(IntRect(257, 33, 32, 32)); break; }
	case 'D': {sp1.setTextureRect(IntRect(129, 0, 32, 32)); break; }
	case 'd': {sp1.setTextureRect(IntRect(129, 32, 32, 32)); break; }
	case 'Q': {sp1.setTextureRect(IntRect(321, 0, 32, 64)); break; }
	case 'q': {sp1.setTextureRect(IntRect(353, 0, 32, 64)); break; }
	}
	return sp1;
}

void printInfo(RenderWindow& window) {
	font.loadFromFile("3.ttf");
	Text text("", font, 30);
	text.setStyle(Text::Bold);
	Text textHp("", font, 35);
	textHp.setStyle(Text::Bold);
	textHp.setFillColor(Color::Yellow);
	std::ostringstream scorePlayer, hpPlayer;
	scorePlayer << score;
	hpPlayer << hp;
	text.setString("Score: " + scorePlayer.str() + "/3");
	text.setPosition(650, 10);
	window.draw(text);
	textHp.setString(hpPlayer.str() + " hp");
	textHp.setPosition(700, 40);
	window.draw(textHp);
}

void printMap(RenderWindow& window, Sprite sp1, RectangleShape rectangle) {
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++)
		{
			if (TileMap[i][j] == ' ') { continue; }
			else{sp1 = dr(sp1, TileMap[i][j]);}
			rectangle.setPosition(j * 32 - offsetX, i * 32 - offsetY);
			sp1.setPosition(j * 32 - offsetX, i * 32 - offsetY);
			window.draw(rectangle);
			window.draw(sp1);
		}
	}
}

Sprite setSprite(int rectLeft, int rectTop, int rectWidth, int rectHeight, int x, int y) {
	Sprite sprite;
	Texture tx;
	tx.loadFromFile("work.png");
	sprite.setTexture(tx);
	sprite.setTextureRect(IntRect(rectLeft, rectTop, rectWidth, rectHeight));
	sprite.setPosition(x, y);
	return sprite;
}


RenderWindow window(VideoMode(800, 500), "Assassin's Creed: in the search for keys");

void viewInfo(Sprite sprite) {
	Font font;
	font.loadFromFile("2.ttf");
	Text text("\n  ����� ������� �������� �������� �������� ���� ���� \n������ �� ������� ����. ���������� ��������������� \n�� ��������� �������. ������� ������� ������������,\n���� �� �������� ���� hp, ��� �� ����� ��������� ����.\n��� �� ����������� �������� ������ 951006 �����  \n��������� ����� ������������ � 2020 ����.\n\n\n\n\n                    - ������ ��� �������� �����", font, 30);
	text.setStyle(Text::Bold);
	text.setPosition(20, 90);
	window.clear(Color::Black);
	window.draw(sprite);
	window.draw(text);
	window.display();
}

void eventWindow() {
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			window.close();
	}
}

void printResult() {
	font.loadFromFile("3.ttf");
	Text text("", font, 80);
	text.setStyle(Text::Bold);
	std::ostringstream scorePlayer;
	scorePlayer << score;
	if (hp < 1)
	{
		window.clear(Color::Red);
		text.setString("  YOU LOSE\n Score: " + scorePlayer.str() + "/3");
	}
	else {
		window.clear(Color::Green);
		text.setString("  YOU WIN\nScore: " + scorePlayer.str() + "/3");
	}
	text.setPosition(220, 150);
	window.draw(text);
}

void returnKeys() {
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++)
		{
			if (TileMap[i][j] != '+') {
				continue;
			}
			else { TileMap[i][j] = 'K'; }
		}
	}
}

int printPrewiev(Sprite sprite, int menu) {
	window.clear(Color::Red);
	window.draw(sprite);
	window.display();
	if (IntRect(0, 0, 1920, 1080).contains(Mouse::getPosition(window))) { menu = 0; }
	if (!Mouse::isButtonPressed(Mouse::Left)) { menu = -1; }
	eventWindow();
	return menu;
}

void printMenu(RectangleShape& choice, Sprite sprite1, Sprite sprite2, Clock& time, int& enterChoice) {
	window.clear(Color::Black);
	window.draw(choice);
	window.draw(sprite1);
	window.draw(sprite2);
	window.display();

	if (time.getElapsedTime().asSeconds() > 0.3)
	{

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			enterChoice--;
			if (enterChoice < 1)
			{
				enterChoice = 3;
			}
			time.restart();
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			enterChoice++;
			if (enterChoice > 3)
			{
				enterChoice = 1;
			}
			time.restart();
		}
	}
	switch (enterChoice)
	{
	case 1: {
		choice.setPosition(450, 149);
		break;
	}
	case 2: {
		choice.setPosition(450, 208);
		break;
	}
	case 3: {
		choice.setPosition(450, 272);
		break;
	}
	}
	window.setKeyRepeatEnabled(false);
}
	
void gamrplay(Sprite sprite1, Sprite sprite2, Texture t) {
	PLAYER p(t);
	Clock clock;

	RectangleShape rectangle(Vector2f(32, 32));
	rectangle.setFillColor((sf::Color(255, 255, 255, 0)));
	hp = 100;
	score = 0;
	offsetX = p.rect.left - 224;
	while (hp > 0 && score < 3)
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time / 600;

		eventWindow();
		if (Keyboard::isKeyPressed(Keyboard::Left)) { p.dx = -0.1; }
		if (Keyboard::isKeyPressed(Keyboard::Right)) { p.dx = 0.1; }
		if (Keyboard::isKeyPressed(Keyboard::Up)) { if (p.onGround) { p.dy = -0.35; p.onGround = false; } }

		p.update(time);

		if (p.rect.left > 400) offsetX = p.rect.left - 400;
		if (p.rect.left > 32 * W - 400) offsetX = p.rect.left - 400 - (p.rect.left - 32 * W + 400);
		offsetY = p.rect.top - 250;

		window.clear(Color::Blue);

		window.draw(sprite1);
		printMap(window, sprite2, rectangle);
		printInfo(window);
		window.draw(p.sprite);
		window.display();
	}
}

void readMap() {
	using namespace std;
	int num = 0;
	string s; // ���� ����� ������ ��������� ������
	ifstream file("Map.txt"); // ���� �� �������� ������ (��� ������ ���� ����� ��������� �� �������)

	while (getline(file, s)) { // ���� �� ��������� ����� ����� ������ ��������� ������ � ���������� (s)
		TileMap[num] = s;
		num++;
	}

	file.close(); // ����������� ��������� ���� ��� �� �� ��������� ���
}


int main()
{
	readMap();
	bool isMenu = true;
	RectangleShape choice(Vector2f(350, 60));
	choice.setFillColor((sf::Color(255, 0, 0, 250)));
	choice.setPosition(450, 272);
	Texture t, tx, pr;
	int menu = -1, enterChoice = 1;
	t.loadFromFile("Asss.png");
	PLAYER p(t);
	tx.loadFromFile("work.png");
	pr.loadFromFile("1.png");
	Sprite sprite[6];
	sprite[0].setTexture(tx);
	sprite[0].setTextureRect(IntRect(1, 430, 800, 500));
	sprite[0].setPosition(-offsetX, -offsetY);
	sprite[1].setTexture(tx);
	sprite[1].setTextureRect(IntRect(0, 0, 32, 32));
	sprite[2].setTexture(tx);
	sprite[2].setTextureRect(IntRect(590, 0, 280, 430));
	sprite[2].setPosition(50, 40);
	sprite[3].setTexture(tx);
	sprite[3].setTextureRect(IntRect(0, 72, 240, 185));
	sprite[3].setPosition(450, 150);
	sprite[4].setTexture(tx);
	sprite[4].setTextureRect(IntRect(0, 271, 141, 139));
	sprite[4].setPosition(20, 340);
	sprite[5].setTexture(pr);
	sprite[5].setPosition(0, 0);
	Clock time;

	while (menu != 3)
	{
		if (menu == -1){menu = printPrewiev(sprite[5], menu);}
		if (menu == 0)
		{
			printMenu(choice, sprite[2], sprite[3], time, enterChoice);
			
			if (Keyboard::isKeyPressed(Keyboard::Enter)) { menu = enterChoice; }
			eventWindow();
		}

		if (menu == 2)
		{
			viewInfo(sprite[4]);
			if ((Keyboard::isKeyPressed(Keyboard::Escape))){menu = 0;}
			eventWindow();
		}

		if (menu == 1)
		{
			float currentFrame = 0;
			gamrplay(sprite[0], sprite[1], t);
			eventWindow();
			printResult();
			window.display();
			returnKeys();
			while ((!Keyboard::isKeyPressed(Keyboard::Escape))) { menu = 0; }
		}
	}
	return 0;
}