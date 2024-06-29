#include "menu.h"

Menu::Menu(sf::RenderWindow* renderWin)
{	
	window = renderWin;
}

void Menu::tick(sf::Vector2i pos)
{
	for (Button *button : buttons) {
		if (vecDist(itof(pos), button->offset) < button->radius) {
			if (button->unpressable) button->pressed = !button->pressed;
			else button->pressed = 1;
			currEv.hasInfo = 1;
			currEv.subData = button->pressEv;

			if(button->pressed) currEv.dataMain = BUTTONPRESS;
			else currEv.dataMain = BUTTONUNPRESS;
			
			break;
		}
	}
}

void Menu::draw()
{
	for (Button *button : buttons) {
		if (button->pressed) window->draw(button->sprOn);
		else window->draw(button->sprOff);
	}
}

void Menu::addBtn(int pressevent, float x, float y, float size, bool unpressable, std::string btnImg)
{
	Button* button = new Button;
	button->offset.x = x;
	button->offset.y = y;
	button->radius = size;
	button->pressEv = pressevent;
	button->unpressable = unpressable;
	
	button->txrOff.loadFromFile(btnImg + "_off.png");
	button->txrOn.loadFromFile(btnImg + "_on.png");
	button->sprOff.setTexture(button->txrOff);
	button->sprOn.setTexture(button->txrOn);

	button->sprOn.setPosition(x - size, y - size);
	button->sprOff.setPosition(x - size, y - size);

	buttons.push_back(button);
}

