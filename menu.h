#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "ftools/sfmath.h"

enum MAINEVENTS {
	BUTTONPRESS,
	BUTTONUNPRESS,

	MAXEVENT,
};



struct EvPackage {
	bool hasInfo;

	int dataMain;
	int subData;
};

struct Button {
	sf::Vector2f offset;
	float radius;
	int pressEv;
	sf::Texture txrOn;
	sf::Texture txrOff;
	sf::Sprite sprOn;
	sf::Sprite sprOff;
	bool pressed = 0;
	bool unpressable;
};

struct Menu {
private:
	sf::RenderWindow* window;

public:
	std::vector<Button*> buttons;

	EvPackage currEv;
	Menu(sf::RenderWindow* renderWin);
	void tick(sf::Vector2i pos);
	void draw();
	void addBtn(int pressevent, float x, float y, float size, bool unpressable, std::string btnImg);

};


#endif