#include "iscr.h"



void LayerText::loadFromFile(std::ifstream* stream)
{
	std::string buf;
	int offs = 0;
	while (std::getline(*stream, buf)) {
		switch (offs) {
		case 0:
			layer1 = buf;
			break;
		case 1:
			layer2 = buf;
			break;
		case 2:
			layer3 = buf;
			break;
		}
		offs++;
	}
}

infScreen::infScreen(sf::RenderWindow* rwin)
{
	window = rwin;
	sftext = new sf::Text;
	sftext->setFont(globalFont);
	

	Ffg* UI = new Ffg("menu/ui.ffg");
	UI->prepare();
	sftext->setCharacterSize(UI->intGet("textSize"));

	bindCount = UI->intGet("bindsCount");
	for (int i = 0; i < bindCount; i++) {
		sf::Vector2f buf;
		buf.x = UI->intGet("bindX");
		buf.y = UI->intGet("bindY");
		textBinds.push_back(buf);
	}

	std::string bgImg = UI->strGet("bgImg");
	bgBind.x = UI->intGet("bgBindX");
	bgBind.y = UI->intGet("bgBindY");

	txrBg.loadFromFile(bgImg);
	bg.setTexture(txrBg);
	bg.setPosition(bgBind);

	std::ifstream* winFile = new std::ifstream(UI->strGet("textWin"));
	std::ifstream* defNEballs = new std::ifstream(UI->strGet("textDefeatNEnoughBalls"));

	delete UI;

	LayerText wintext;
	wintext.loadFromFile(winFile);
	LayerText defNEtext;
	defNEtext.loadFromFile(defNEballs);

	texts.push_back(wintext);
	texts.push_back(defNEtext);

	delete winFile;
	delete defNEballs;
}

void infScreen::draw(int textId)
{
	window->draw(bg);

	sftext->setPosition(textBinds[0].x, textBinds[0].y );
	sftext->setString(texts[textId].layer1);
	window->draw(*sftext);

	sftext->setPosition(textBinds[1].x, textBinds[1].y);
	sftext->setString(texts[textId].layer2);
	window->draw(*sftext);

	sftext->setPosition(textBinds[2].x, textBinds[2].y);
	sftext->setString(texts[textId].layer3);
	window->draw(*sftext);
}
