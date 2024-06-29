#ifndef FIELD_H
#define FIELD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>

#include "ftools/sfmath.h"
#include "frule.h"
#include "ftools/ffg.h"

//#define __dbg
#define TL(tx,ty) tiles[tx+(ty*size.x)]

enum PALOFFSETS {
	TILEOFFS = 0,

	BALLOFFS = 4,

	MAXOFFS,
};

enum PALCOLORS {
	//CAPTUREDTILE,
	GRAYTILE,
	REDTILE,
	GREENTILE,
	BLUETILE,

	REDBALL,
	GREENBALL,
	BLUEBALL,
	WHITEBALL,

	MAXPAL
};

// ninja <3
static sf::Color tileGray(50, 50, 50);
static sf::Color tileRed(150, 0, 0);
static sf::Color tileGreen(0, 150, 0);
static sf::Color tileBlue(0, 0, 150);

static sf::Color palette[MAXPAL] = {
	//sf::Color::Color(150,150, 0),
	tileGray,
	tileRed,
	tileGreen,
	tileBlue,

	sf::Color::Red,
	sf::Color::Green,
	sf::Color::Blue,
	sf::Color::White,

};

enum BALLTYPES {
	BALLR,
	BALLG,
	BALLB,
	BALLW,

	MAXBTYPE,
};


enum EVENTS {
	DESTROY,
	BREED,
	PAINT,
	STOP,
};

struct Tile {
	sf::Color cooling;
	bool captured = 0;
	sf::RectangleShape mainBody;
	sf::RectangleShape outBody;
	sf::RectangleShape colBody;
};

struct Ball {
	int type;
	sf::Vector2f pos;
	sf::Vector2f vspeed;
	bool vacant = 0;
	int nCollTime = 0;
	
	sf::CircleShape mainBody;
	sf::CircleShape outBody;
};

struct Field {
private:
	int RBalls;
	int GBalls;
	int BBalls;
	int WBalls;

	bool subTick = 1;
	int curBreedCD = 0;
	int maxBreedCD;

	float ballSize;
	int coolingPwr;
	int ballVacantTime;
	bool killCaptures;

	sf::RenderWindow* window;
	fieldRule* currRule;

	Tile* tiles;
	float captureKx;

	std::vector<Ball*> balls;


	int minTypeBalls();

	void collideBreed(Ball* ball, Ball* collider);
	void vacantBreed(Ball* ball);

public:
	int capturedTiles = 0;

	sf::Vector2i size;
	sf::Vector2f tileSize;
	Field(sf::RenderWindow* renderWin, fieldRule* rules);

	void draw(bool tick);
	bool enoughBalls();
	void slowdown();
	void randGen();
	void loadAutosave();
	void autosave();
	void mapGen();
};

#endif