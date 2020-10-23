#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <map>
#include "main.h"
#include <windows.h>
#include <string.h>
#include <ctime>

using namespace sf;

float offsetX = 0, offsetY = 0;
int hp = 100;
Font font;

const int H = 30, W = 50;
int score = 0, positionJump1 = 0, positionJump2 = 0, counter = 0;

String TileMap[H];
std::map <char, IntRect> spriteMap = {
	{'B', IntRect(1, 0, 32, 32)},
	{'b', IntRect(384, 0, 32, 32)},
	{'+', IntRect(97, 32, 32, 32)},
	{'0', IntRect(347, 120, 32, 32)},
	{'1', IntRect(65, 0, 32, 32)},
	{'2', IntRect(65, 32, 32, 32)},
	{'3', IntRect(33, 0, 32, 32)},
	{'4', IntRect(33, 32, 32, 32)},
	{'5', IntRect(97, 0, 32, 32)},
	{'R', IntRect(161, 0, 32, 32)},
	{'r', IntRect(161, 32, 32, 32)},
	{'K', IntRect(257, 33, 32, 32)},
	{'D', IntRect(129, 0, 32, 32)},
	{'d', IntRect(129, 32, 32, 32)},
	{'Q', IntRect(321, 0, 32, 64)},
	{'q', IntRect(353, 0, 32, 64)}
};

class PLAYER {

public:

	double dx, dy, currentFrame;
	FloatRect rect;
	bool onGround, isCheckRight = true;
	Sprite sprite;

	PLAYER(Texture& image) {
		sprite.setTexture(image);
		rect = FloatRect(7 * 32, 22 * 32, 32, 60);
		dx = dy = 0.1;
		currentFrame = 0;
	}

	void update(float time) {
		if (time > 90) { time = 90; }
		rect.left += dx * time;
		Collision(0);
		if (!onGround) dy = dy + 0.0005 * time;
		rect.top += dy * time;
		onGround = false;
		Collision(1);

		currentFrame += 0.005 * time;
		if (currentFrame > 5) currentFrame -= 5; // обнуление времени
		if (currentFrame > 2) { positionHero(currentFrame, 44); }
		else { positionHero(currentFrame, 41); }
		if ((!Keyboard::isKeyPressed(Keyboard::Right)) && (!Keyboard::isKeyPressed(Keyboard::Left)) && (!Keyboard::isKeyPressed(Keyboard::Up)))    //стойка персонажа
		{
			if (isCheckRight) { sprite.setTextureRect(IntRect(39 * int(currentFrame), 1, 32, 60)); }
			else { sprite.setTextureRect(IntRect(39 * int(currentFrame) + 32, 1, -32, 60)); }
		}
		if (isCheckRight) { jump(1); }
		else { jump(0); }
		if (Keyboard::isKeyPressed(Keyboard::Left)) { isCheckRight = false; }
		if (Keyboard::isKeyPressed(Keyboard::Right)) { isCheckRight = true; }
		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
		dx = 0;
	}

	void positionHero(float currentFrame, int pixel) {
		if (dx > 0) sprite.setTextureRect(IntRect(pixel * int(currentFrame), 74, pixel, 60));//изменение текстуры
		if (dx < 0) sprite.setTextureRect(IntRect(pixel * int(currentFrame) + pixel, 74, -pixel, 60));
	}

	void jump(int i) {
		int x = rect.left / 32;    int y = (rect.top / 32) + 2;
		if ((TileMap[y][x + i] != 'B') && (TileMap[y][x + i] != 'b') && (TileMap[y - 1][x + i] != 'B') && (TileMap[y - 1][x + i] != 'b') && (TileMap[y - 2][x + i] != 'B') && (TileMap[y - 2][x + i] != 'b')) {
			if (isCheckRight) { sprite.setTextureRect(IntRect(335, 273, 32, 60)); }
			else { sprite.setTextureRect(IntRect(367, 273, -32, 60)); }
		}
		checkHp(i);
	}

	void checkHp(int i) {
		int x, y = (rect.top / 32) + 2;
		if (isCheckRight) { x = (rect.left - 29) / 32; }
		else { x = (rect.left + 29) / 32; }
		if ((TileMap[y][x + i] != 'B') && (TileMap[y][x + i] != 'b'))  //право
		{
			if (counter == 0) { positionJump1 = rect.top; }
			positionJump2 = rect.top;
			counter++;
		}
		else {
			if (positionJump1 != 0) {
				int n = positionJump2 - positionJump1;
				int num = (positionJump2 - positionJump1 + 10) / 32;
				switch (num) {
				case 5: {hp -= 20; break; }
				case 6: {hp -= 28; break; }
				default: {
					if (num > 6) {
						int i = 5 + rand() % 3;
						hp -= num * i - rand() % 8;
					}
					break;
				}
				}
				positionJump1 = positionJump2 = counter = 0;
			}
		}
	}

	void Collision(int dir)  //обработка положения игрока
	{
		for (int i = rect.top / 32; i < (rect.top + rect.height) / 32; i++)
			for (int j = rect.left / 32; j < (rect.left + rect.width) / 32; j++) {
				if ((TileMap[i][j] == 'B') || (TileMap[i][j] == 'b') || (TileMap[i][j] == 'R') || (TileMap[i][j] == 'r') || (TileMap[i][j] == '0')) {
					if ((dx > 0) && (dir == 0)) rect.left = j * 32 - rect.width;
					if ((dx < 0) && (dir == 0)) rect.left = j * 32 + 32;
					if ((dy > 0) && (dir == 1)) { rect.top = i * 32 - rect.height;  dy = 0;   onGround = true; }
					if ((dy < 0) && (dir == 1)) { rect.top = i * 32 + 32;   dy = 0; }
				}
				if (TileMap[i][j] == 'K') {
					TileMap[i][j] = '+';
					score++;
				}
			}
	}
};

void printInfo(RenderWindow& window) {
	font.loadFromFile("Fonts\\3.ttf");
	Text text("", font, 30);
	text.setStyle(Text::Bold);
	Text textHp("", font, 35);
	textHp.setStyle(Text::Bold);
	if (hp <= 100 && hp > 80) { textHp.setFillColor(sf::Color(255, 255, 0)); }
	if (hp <= 80 && hp > 60) { textHp.setFillColor(sf::Color(255, 215, 0)); }
	if (hp <= 60 && hp > 45) { textHp.setFillColor(sf::Color(255, 265, 0)); }
	if (hp <= 45 && hp > 30) { textHp.setFillColor(sf::Color(255, 140, 0)); }
	if (hp <= 30 && hp > 15) { textHp.setFillColor(sf::Color(255, 69, 0)); }
	if (hp <= 15) { textHp.setFillColor(sf::Color(255, 0, 0)); }
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

char positionKey(int& keys, int& noKey, char TileMap, int vib) {
	if ((keys == 2 && noKey == 0) || (keys == 1 && noKey == 1)) { TileMap = '+'; }
	else {
		if (noKey != 2 && vib == 0) {
			TileMap = '+';
			noKey++;
		}
	}
	keys--;
	return TileMap;
}

void printMap(RenderWindow& window, Sprite sp1, PLAYER p) {
	int startHorisontal = (p.rect.left / 32) - 13;
	int stopHorisontal = startHorisontal + 2 * 13;
	if (startHorisontal < 0) { startHorisontal = 0; }
	if (startHorisontal >= W - 2 * 13) { startHorisontal = W - 2 * 13; stopHorisontal = W; }
	int startVertical = (p.rect.top / 32) - 8;
	if (startVertical < 9) { startVertical = 0; }
	int stopVeertical = (p.rect.top / 32) + 9;
	if (stopVeertical > H) { stopVeertical = H; };
	for (int i = startVertical; i < stopVeertical; i++) {
		for (int j = startHorisontal; j < W; j++) {
			if (TileMap[i][j] == ' ') { continue; }
			else { sp1.setTextureRect(spriteMap[TileMap[i][j]]); }
			sp1.setPosition(j * 32 - offsetX, i * 32 - offsetY);
			window.draw(sp1);
		}
	}
}

RenderWindow window(VideoMode(800, 500), "Assassin's Creed: in the search for keys");

void viewInfo(Sprite sprite) {
	Font font;
	font.loadFromFile("Fonts\\2.ttf");
	Text text("\n  Целью данного игрового средства является сбор всех \nключей на игровом поле. Управление осуществляется \nпо сдедствам стрелок. Игровой процесс продолжается,\nпока не кончатся очки hp, или не будет выполнена цель.\nЭто ПО разработано судентом группы 951006 БГУИР  \nКурбацким Ильёй Дмитриевичем в 2020 году.\n\n\n\n\n                    - ссылка для обратной связи", font, 30);
	text.setStyle(Text::Bold);
	text.setPosition(20, 90);
	window.clear(Color::Black);
	window.draw(sprite);
	window.draw(text);
	window.display();
}

void eventWindow(int& menu) {
	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) {
			menu = 3;
			window.close();
		}
	}
}

void printResult() {
	font.loadFromFile("Fonts\\3.ttf");
	Text text("", font, 80);
	text.setStyle(Text::Bold);
	std::ostringstream scorePlayer;
	scorePlayer << score;
	if (hp < 1) {
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

int printPrewiev(Sprite sprite, int menu, bool checkMusic) {
	window.clear(Color::Black);
	window.draw(sprite);
	window.display();
	if (!checkMusic) {
		if (IntRect(0, 0, 1920, 1080).contains(Mouse::getPosition(window))) { menu = 0; }
		if (!Mouse::isButtonPressed(Mouse::Left)) { menu = -1; }
	}
	eventWindow(menu);
	return menu;
}

void printMenu(RectangleShape& choice, Sprite sprite1, Sprite sprite2, Clock& time, int& enterChoice) {
	window.clear(Color::Black);
	window.draw(choice);
	window.draw(sprite1);
	window.draw(sprite2);
	window.display();
	if (time.getElapsedTime().asSeconds() > 0.3) {
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			enterChoice--;
			if (enterChoice < 1) { enterChoice = 3; }
			time.restart();
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			enterChoice++;
			if (enterChoice > 3) { enterChoice = 1; }
			time.restart();
		}
	}
	switch (enterChoice) {
	case 1: {choice.setPosition(450, 149); break; }
	case 2: {choice.setPosition(450, 208); break; }
	case 3: {choice.setPosition(450, 272); break; }
	}
}

void gameplay(Sprite sprite1, Sprite sprite2, Texture t, int& menu) {
	if ((int)rand() % 2) {
		sprite1.setTextureRect(IntRect(0, 1500, 800, 500));
	}
	else { sprite1.setTextureRect(IntRect(0, 1000, 800, 500)); }
	PLAYER p(t);
	Clock clock;
	SoundBuffer buffer;
	buffer.loadFromFile("Audio\\jump.wav");
	Sound sound;
	sound.setBuffer(buffer);
	hp = 100;
	score = 0;
	int x, y;
	offsetX = p.rect.left - 224;
	while (hp > 0 && score < 3) {
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time / 600;

		eventWindow(menu);
		if (Keyboard::isKeyPressed(Keyboard::Left)) { p.dx = -0.1; }
		if (Keyboard::isKeyPressed(Keyboard::Right)) { p.dx = 0.1; }
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			if (p.onGround) {
				x = p.rect.left / 32;    y = (p.rect.top / 32) - 1;
				if ((TileMap[y][x + p.isCheckRight] != 'B') && (TileMap[y][x + p.isCheckRight] != 'b')) {
					sound.play();
					p.dy = -0.35; p.onGround = false;
				}
			}
		}

		p.update(time);

		if (p.rect.left > 400) offsetX = p.rect.left - 400;
		if (p.rect.left > 32 * W - 400) offsetX = p.rect.left - 400 - (p.rect.left - 32 * W + 400);
		offsetY = p.rect.top - 250;
		window.clear(Color::Black);
		window.draw(sprite1);   ///
		printMap(window, sprite2, p);
		printInfo(window);
		window.draw(p.sprite);
		window.display();
		while (Keyboard::isKeyPressed(Keyboard::Escape)) { hp = 0; }
	}
}

void readMap(const char name[]) {
	using namespace std;
	int num = 0;
	int noKey = 0, keys = 5;
	string s;
	ifstream file1(name);
	while (getline(file1, s)) {
		TileMap[num] = s;
		for (int i = 0; i < W; i++) {
			if (TileMap[num][i] == 'K') {
				TileMap[num][i] = positionKey(keys, noKey, TileMap[num][i], (rand() % 2));
			}
		}
		num++;
	}
	file1.close();
}

void setSprite(Sprite& sprite0, Sprite& sprite1, Sprite& sprite2, Sprite& sprite3, Sprite& sprite4, Sprite& sprite5, Sprite& sprite6, Texture& tx, Texture& pr) {
	tx.loadFromFile("Graphics\\work.png");
	pr.loadFromFile("Graphics\\2.png");
	sprite0.setTexture(pr);
	sprite0.setTextureRect(IntRect(0, 1000, 800, 500));
	sprite0.setPosition(-offsetX, -offsetY);
	sprite1.setTexture(tx);
	sprite1.setTextureRect(IntRect(0, 0, 32, 32));
	sprite2.setTexture(tx);
	sprite2.setTextureRect(IntRect(590, 0, 280, 430));
	sprite2.setPosition(50, 40);
	sprite3.setTexture(tx);
	sprite3.setTextureRect(IntRect(0, 72, 240, 185));
	sprite3.setPosition(450, 150);
	sprite4.setTexture(tx);
	sprite4.setTextureRect(IntRect(0, 271, 141, 139));
	sprite4.setPosition(20, 340);
	sprite5.setTexture(pr);
	sprite5.setPosition(0, 0);
	sprite6.setTextureRect(IntRect(0, 500, 800, 500));
	sprite6.setTexture(pr);
	sprite6.setPosition(0, 0);
}

void setMap(RectangleShape& choice, Sprite sprite, Clock& time, int& menu) {
	int enterChoice = 1;
	int numMap;
	while (1) {
		window.clear(Color::Black);
		window.draw(sprite);
		window.draw(choice);
		window.display();
		if (Keyboard::isKeyPressed(Keyboard::Enter) && time.getElapsedTime().asSeconds() > 0.3) { break; }
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { menu = 0; return; }
		if (time.getElapsedTime().asSeconds() > 0.3) {
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				enterChoice -= 10;
				if (enterChoice < 1) { enterChoice += 20; }
				time.restart();
			}

			if (Keyboard::isKeyPressed(Keyboard::Up)) {
				enterChoice--;
				if ((enterChoice < 1) || (enterChoice < 11 && enterChoice>9)) { enterChoice += 2; }
				time.restart();
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				enterChoice += 1;
				if ((enterChoice > 2 && enterChoice < 10) || (enterChoice > 12)) { enterChoice -= 2; }
				time.restart();
			}

			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				enterChoice += 10;
				if (enterChoice > 20) { enterChoice -= 20; }
				time.restart();
			}

			eventWindow(menu);
		}
		switch (enterChoice) {
		case 1: {choice.setPosition(0, 75); break; }
		case 11: {choice.setPosition(753, 75); break; }
		case 2: {choice.setPosition(0, 282); break; }
		case 12: {choice.setPosition(753, 282); break; }
		}
	}
	switch (enterChoice) {
	case 1: readMap("Maps\\Map2.txt"); break;
	case 11: readMap("Maps\\Map1.txt"); break;
	case 2: readMap("Maps\\Map3.txt"); break;
	case 12: readMap("Maps\\Map4.txt"); break;
	}
}

int main() {
	time_t seconds;
	seconds = time(NULL);
	srand(seconds);
	Music soundPrev, musicMenu, musicGame;
	soundPrev.openFromFile("Audio\\preview.wav");
	musicMenu.openFromFile("Audio\\menu.wav");
	musicMenu.setVolume(2.5);
	musicGame.openFromFile("Audio\\game.wav");
	musicGame.setVolume(2.5);
	soundPrev.play();
	bool isMenu = true;
	RectangleShape choice(Vector2f(350, 60));
	choice.setFillColor((sf::Color(255, 0, 0, 250)));
	choice.setPosition(450, 272);
	RectangleShape choiceMap(Vector2f(47, 181));
	choiceMap.setFillColor((sf::Color(255, 0, 0, 250)));
	Texture t, tx, pr;
	int menu = -1, enterChoice = 1;
	t.loadFromFile("Graphics\\Asss.png");
	PLAYER p(t);
	Sprite sprite[7];
	setSprite(sprite[0], sprite[1], sprite[2], sprite[3], sprite[4], sprite[5], sprite[6], tx, pr);
	Clock time;

	while (menu != 3) {
		if (menu == -1) { menu = printPrewiev(sprite[5], menu, soundPrev.getStatus()); }
		if (menu == 0) {
			musicGame.stop();
			musicMenu.setLoop(true);
			if (!musicMenu.getStatus()) { musicMenu.play(); }
			printMenu(choice, sprite[2], sprite[3], time, enterChoice);

			if (Keyboard::isKeyPressed(Keyboard::Enter)) { menu = enterChoice; time.restart(); }
			eventWindow(menu);
		}

		if (menu == 2) {
			viewInfo(sprite[4]);
			if ((Keyboard::isKeyPressed(Keyboard::Escape))) { menu = 0; }
			eventWindow(menu);
		}

		if (menu == 1) {
			setMap(choiceMap, sprite[6], time, menu);
			if (menu != 0) {
				musicMenu.stop();
				musicGame.setLoop(true);
				if (!musicGame.getStatus()) { musicGame.play(); }
				float currentFrame = 0;
				gameplay(sprite[0], sprite[1], t, menu);
				eventWindow(menu);
				printResult();
				window.display();
				while ((!Keyboard::isKeyPressed(Keyboard::Escape))) { menu = 0; }
			}
		}
	}
	musicGame.stop();
	musicMenu.stop();
	soundPrev.stop();
	return 0;
}
