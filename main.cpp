#include <SFML/Graphics.hpp>



sf::Font globalFont;

#include "frule.h"
#include "field.h"
#include "menu.h"
#include "ftools/ffg.h"


#ifdef ISCREEN
#include "iscr.h"
#endif

sf::RenderWindow* window;
sf::Vector2i RES;
sf::Event sfev;

fieldRule* currRule;
bool paused = 0;
bool lose = 0;
bool useAutosaves = 0;

int fpsDown;
int fpsStd;
int fpsUp;

int main() {

	currRule = new fieldRule;
	currRule->type1 = UP_V;
	currRule->type2 = DOWN_V;
	currRule->fevent = EVB_STOP;

	Ffg *gameprops = new Ffg("menu/gameprops.ffg");

	globalFont.loadFromFile(gameprops->strGet("font"));

	RES.x = gameprops->intGet("width");
	RES.y = gameprops->intGet("height");

	fpsDown = gameprops->intGet("spdDownFPS");
	fpsStd = gameprops->intGet("spdStdFPS");
	fpsUp = gameprops->intGet("spdUpFPS");

	window = new sf::RenderWindow;
	window->create(sf::VideoMode(RES.x, RES.y), "fMOBL");
	window->setFramerateLimit(fpsStd);

	//useAutosaves = gameprops->intGet("useAutosaves");

	

	delete gameprops;

	Field field(window, currRule);
	Menu menu(window);
	Ffg *menuBtns = new Ffg("menu/buttons.ffg");

	int btnCount = menuBtns->intGet("buttonCount");
	std::string beg = menuBtns->strGet("beg");
	std::string end = menuBtns->strGet("end");
	std::string btnsBGImg = menuBtns->strGet("btnsBGImg");
	
	sf::Texture btnsBGTexture;
	sf::Sprite btnsBG;
	btnsBGTexture.loadFromFile(btnsBGImg);
	btnsBG.setTexture(btnsBGTexture);
	btnsBG.setPosition(menuBtns->intGet("btnsBGX"), menuBtns->intGet("btnsBGY"));

	delete menuBtns;

	for (int i = 0; i < btnCount; i++) {
		Ffg btn(std::string("menu/"+beg + std::to_string(i) + end).c_str());
		btn.prepare();
		menu.addBtn(btn.intGet("event"), btn.intGet("x"), btn.intGet("y"), btn.intGet("radius"),  btn.intGet("unpressable"), btn.strGet("sprite"));
		
		menu.buttons[i]->pressed = btn.intGet("alredyPressed");
		
	}
#ifdef ISCREEN
	infScreen *iscreen = new infScreen(window);
#endif
#ifdef __dbg
	sf::Clock clock;
	int frameCount = 0;
#endif

	if (useAutosaves) {
		field.loadAutosave();
	}
	else {
		field.randGen();
	}
	field.mapGen();

	while (window->isOpen()) {
		menu.currEv.hasInfo = 0;
		while (window->pollEvent(sfev)) {
			if (sfev.type == sfev.Closed) window->close();
			else if (sfev.type == sfev.MouseButtonPressed && window->hasFocus()) {
				menu.tick(sf::Mouse::getPosition(*window));
			}
		}

		if (menu.currEv.hasInfo) {

			switch (menu.currEv.dataMain) {
			case BUTTONPRESS:
				if (menu.currEv.subData >= UP_R && menu.currEv.subData <= UP_V) {
					for (int i = UP_R; i <= UP_V; i++) {
						if (i == menu.currEv.subData) continue;

						menu.buttons[i]->pressed = 0;
					}
					currRule->type1 = menu.currEv.subData;
				}
				else if (menu.currEv.subData >= DOWN_R && menu.currEv.subData <= DOWN_V) {
					for (int i = DOWN_R; i <= DOWN_V; i++) {
						if (i == menu.currEv.subData) continue;

						menu.buttons[i]->pressed = 0;
					}
					currRule->type2 = menu.currEv.subData ;
				}
				else if (menu.currEv.subData >= EVB_DESTROY && menu.currEv.subData <= EVB_STOP) {
					for (int i = EVB_DESTROY; i <= EVB_STOP; i++) {
						if (i == menu.currEv.subData) continue;

						menu.buttons[i]->pressed = 0;
					}
					currRule->fevent = menu.currEv.subData ;
				}
				else if(menu.currEv.subData == PAUSE) {
		
					paused = 1;
				}
				else if (menu.currEv.subData == SPEEDUP && !lose) {
					window->setFramerateLimit(fpsUp);
					menu.buttons[SPEEDDOWN]->pressed = 0;
				}
				else if (menu.currEv.subData == SPEEDDOWN && !lose) {
					window->setFramerateLimit(fpsDown);
					menu.buttons[SPEEDUP]->pressed = 0;
				}
				break;
			case BUTTONUNPRESS:
				if (menu.currEv.subData == PAUSE) {
					paused = 0;
				}
				else if (menu.currEv.subData == SPEEDUP || menu.currEv.subData == SPEEDDOWN) {
					window->setFramerateLimit(fpsStd);
				}
				
			}
		}

		window->clear();

		field.draw(!(paused));
		window->draw(btnsBG);
		menu.draw();
#ifdef ISCREEN
		if (!field.enoughBalls()) {
			window->setFramerateLimit(30);
			lose = 1;
			field.slowdown();


			iscreen->draw(DEFEAT_N_ENOUGH_BALLS);
		}
		else if (field.capturedTiles == field.size.x * field.size.y) {
			window->setFramerateLimit(fpsStd);
			lose = 1;
			field.slowdown();
			iscreen->draw(MOBL_WIN);
		}
#endif
		window->display();
#ifdef __dbg
		frameCount++;
		if (clock.getElapsedTime().asSeconds() >= 1.0) {
			float fps = frameCount / clock.restart().asSeconds();
			window->setTitle(std::string("fMOBL; FPS="+std::to_string(frameCount)));
			frameCount = 0;
		}
#endif
	}
	if (useAutosaves && !lose) {
		field.autosave();
	}
	return 0;
}
