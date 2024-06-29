#ifndef ISCR_H
#define ISCR_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "ftools/ffg.h"

extern sf::Font globalFont;

enum ISCR_TEXTS {
	MOBL_WIN,
	DEFEAT_N_ENOUGH_BALLS,

	MAXTEXT,
};

struct LayerText {
	std::string layer1;
	std::string layer2;
	std::string layer3;
	void loadFromFile(std::ifstream* stream);
};

struct infScreen {
private:
	sf::Texture txrBg;
	sf::Sprite bg;
	std::vector<sf::Vector2f> textBinds;
	std::vector<LayerText> texts;
	sf::Text* sftext;
	int bindCount;
	sf::Vector2f bgBind;
	sf::RenderWindow* window;
public:
	infScreen(sf::RenderWindow* rwin);
	void draw(int textId);

};

#endif