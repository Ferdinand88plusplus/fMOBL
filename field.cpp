#include "field.h"
#include "frule.h"


void Field::collideBreed(Ball* ball, Ball* collider)
{
	Ball* nball = new Ball;
	//if (currRule->type1 == currRule->type2 - DOWN_R) break;

	

	srand((int)sqrt(degree(RBalls*WBalls,BBalls*GBalls)));
	nball->pos = (ball->pos + collider->pos) / 2;
	if (rand() % 2) {
		nball->type = currRule->type1;
	}
	else {
		nball->type = currRule->type2 - DOWN_R;
	}
	nball->vspeed = (ball->vspeed + collider->vspeed) / 2;

	nball->mainBody.setFillColor(palette[nball->type + BALLOFFS]);
	nball->mainBody.setRadius(ballSize);
	nball->mainBody.setOrigin(ballSize, ballSize);
	nball->outBody.setRadius(ballSize * 1.4);
	nball->outBody.setOrigin(ballSize * 1.4, ballSize * 1.4);
	nball->outBody.setFillColor(sf::Color::Black);

	ball->pos = stepVec(ball->pos, vecAng(nball->pos, ball->pos), vecDist(ball->pos, nball->pos) / 2);
	collider->pos = stepVec(collider->pos, vecAng(nball->pos, collider->pos), vecDist(collider->pos, nball->pos) / 2);

	//ball->vspeed = stepVec(ball->vspeed, vecAng(nball->pos, ball->pos), ballSize);
	//collider->vspeed = stepVec(collider->vspeed, vecAng(nball->pos, collider->pos), ballSize);


	balls.push_back(nball);
	if (balls.size() > 5000) {
		std::cout << "BALLS ARRAY OVERLOADED AFTER 'BREED' RULE\n";
		exit(1);
	}
}

int Field::minTypeBalls()
{
	
	if (RBalls < GBalls && RBalls < BBalls ) {
		return BALLR;
	}
	if (GBalls < RBalls && GBalls < BBalls ) {
		return BALLG;
	}
	if (BBalls < RBalls && BBalls < GBalls) {
		return BALLB;
	}
	srand(balls.size());
	return rand() % BALLW;
}

void Field::vacantBreed(Ball* ball)
{
	Ball* nball = new Ball;
	nball->pos = ball->pos + ball->vspeed;
	float dist = vecDist(ball->pos, nball->pos);
	nball->pos = stepVec(nball->pos, vecAng(ball->pos, nball->pos), dist / 2);
	ball->pos = stepVec(ball->pos, vecAng(nball->pos, ball->pos), dist / 2);
	nball->vspeed = -ball->vspeed;
	nball->type = ball->type;
	nball->vacant = 1;
	nball->nCollTime = ballVacantTime + 1;
	nball->mainBody.setFillColor(palette[nball->type + BALLOFFS]);
	nball->mainBody.setRadius(ballSize);
	nball->mainBody.setOrigin(ballSize, ballSize);
	nball->outBody.setRadius(ballSize * 1.4);
	nball->outBody.setOrigin(ballSize * 1.4, ballSize * 1.4);
	nball->outBody.setFillColor(sf::Color::Black);
	balls.push_back(nball);
}

//#define __dbg

Field::Field(sf::RenderWindow* renderWin, fieldRule* rules)
{
	window = renderWin;
	currRule = rules;

	
	Ffg* consts = new Ffg("menu/gameprops.ffg");

	consts->prepare();

	size.x = consts->intGet("mapSizeX");
	size.y = consts->intGet("mapSizeY");

	tileSize.x = consts->intGet("tileSizeX");
	tileSize.y = consts->intGet("tileSizeY");

	coolingPwr = consts->intGet("coolingPower");
	ballVacantTime = consts->intGet("ballVacantTime");
	ballSize = consts->intGet("ballSize");
	maxBreedCD = consts->intGet("ballBreedCD");

	RBalls = consts->intGet("RBalls");
	GBalls = consts->intGet("GBalls");
	BBalls = consts->intGet("BBalls");
	WBalls = consts->intGet("WBalls");
	killCaptures = consts->intGet("deathAfterCapture");
	captureKx = (tileSize.x - tileSize.x / 10) / 255;

	delete consts;
	

	

}


void Field::draw(bool tick)
{
	RBalls = 0;
	GBalls = 0;
	BBalls = 0;
	
	float sz;
	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			/*tileRect.setPosition(x * tileSize.x, y * tileSize.y);
			tileRect.setFillColor(palette[GRAYTILE]/1.5);
			tileRect.setSize(tileSize);
			window->draw(tileRect);

			tileRect.move((tileSize / 10) / 2);
			tileRect.setSize(tileSize - (tileSize / 10));
			tileRect.setFillColor(palette[GRAYTILE]);*/
			
			window->draw(TL(x,y).mainBody);
			window->draw(TL(x, y).outBody);


			
			sz = (TL(x, y).cooling.r+TL(x,y).cooling.g+TL(x,y).cooling.b) * captureKx;
			TL(x,y).colBody.setSize(sf::Vector2f(sz, sz));
			TL(x, y).colBody.setPosition(x * tileSize.x, y * tileSize.y);
			TL(x, y).colBody.move(tileSize / 2);
			TL(x, y).colBody.setOrigin(sz/2, sz/2);
				
			TL(x, y).colBody.setFillColor(boolean(TL(x,y).cooling) * 255);
			window->draw(TL(x, y).colBody);

			
		}
	}

	
	for (int b = 0; b < balls.size(); b++) {
		Ball* ball = balls[b];
		/*		ballCircle.setPosition(ball->pos);
		ballCircle.setRadius(ballSize * 1.4);
		ballCircle.setOrigin(ballSize * 1.4, ballSize * 1.4);
		if(!ball->vacant) ballCircle.setFillColor(sf::Color::Black);
		else ballCircle.setFillColor(sf::Color::White);
		window->draw(ballCircle);

		ballCircle.setRadius(ballSize );
		ballCircle.setOrigin(ballSize , ballSize );
		sf::Color bcol;
		bcol = palette[ball->type + BALLOFFS];
		if (!ball->vacant) bcol = bcol/1.5;
		ballCircle.setFillColor(bcol);
		*/
		ball->mainBody.setPosition(ball->pos);
		ball->outBody.setPosition(ball->pos);
		window->draw(ball->outBody);
		window->draw(ball->mainBody);

		if (ball->type == BALLR) RBalls++;
		else if (ball->type == BALLG) GBalls++;
		else if (ball->type == BALLB) BBalls++;

		if (tick) {
			ball->nCollTime++;
			ball->pos += ball->vspeed;

			if (ball->pos.x - ballSize < 0 || ball->pos.x + ballSize > size.x * tileSize.x) { 
				ball->vspeed.x *= -1; 
				if (ball->pos.x - ballSize < 0) {
					ball->pos.x = ballSize;
				}
				else {
					ball->pos.x = size.x * tileSize.x - ballSize;
				}
			}
			if (ball->pos.y - ballSize < 0 || ball->pos.y + ballSize > size.y * tileSize.y) { 
				ball->vspeed.y *= -1;
				if (ball->pos.y - ballSize < 0) {
					ball->pos.y = ballSize;
				}
				else {
					ball->pos.y = size.y * tileSize.y - ballSize;
				}
			}
			float dist;
			float angC2B;
			float angB2C;
			sf::Vector2f buf;
			bool terminate = 0;
			for (int c = 0; c < balls.size(); c++) {
				Ball* collider = balls[c];
				if (collider == ball) continue;

				dist = vecDist(ball->pos, collider->pos);
				if (dist < ballSize * 2) {
					angC2B = vecAng(collider->pos, ball->pos);
					angB2C = vecAng(ball->pos, collider->pos);
					buf = ball->vspeed;
					ball->vspeed = stepVec(sf::Vector2f(0,0),angC2B, sqrt(degree(collider->vspeed.x,2 )+degree(collider->vspeed.y, 2)));
					collider->vspeed = stepVec(sf::Vector2f(0, 0), angB2C, sqrt(degree(buf.x, 2) + degree(buf.y, 2)));;
					ball->pos = stepVec(ball->pos, angC2B, dist/2);
					collider->pos = stepVec(collider->pos, angB2C, dist / 2);
					ball->nCollTime = 0;
					collider->nCollTime = 0;

					if (currRule->type2 != DOWN_V && currRule->type1 != UP_V) {
						bool match = 0;
						
						
						if ((currRule->type1 == ball->type && currRule->type2 == collider->type + DOWN_R) ||
							(currRule->type1 == collider->type && currRule->type2 == ball->type + DOWN_R)) match = 1;

						if (match) {
							int mintype;
							
							switch (currRule->fevent) {
							case EVB_DESTROY:
								delete ball;
								delete collider;
								if (c > b) c--;
								balls.erase(balls.begin() + b, balls.begin() + b + 1);
								balls.erase(balls.begin() + c, balls.begin() + c + 1);
								b--;
								terminate = 1;
								break;
							case EVB_BREED:
								curBreedCD++;
								if (curBreedCD != maxBreedCD) break;
								curBreedCD = 0;
								collideBreed(ball, collider);
								break;
							case EVB_PAINT:
								mintype = minTypeBalls();
								ball->type = mintype;
								collider->type = mintype;
								ball->mainBody.setFillColor(palette[BALLOFFS + ball->type]);
								collider->mainBody.setFillColor(palette[BALLOFFS + collider->type]);

								break;
							case EVB_STOP:
								ball->vspeed = sf::Vector2f(0, 0);
								collider->vspeed = sf::Vector2f(0, 0);
								ball->pos = stepVec(ball->pos, angC2B, dist/2);
								collider->pos = stepVec(collider->pos, angB2C, dist / 2);	
								break;
							}

							if (terminate) break;
							
						}
					}
				}
			}
			if (terminate || !subTick) continue;
			Tile *tile = &TL(int(ball->pos.x / tileSize.x), int(ball->pos.y / tileSize.y));
			if (!tile->captured) {
				switch (ball->type) {
				case BALLW:
					if (tile->cooling.r != 0) {
						tile->cooling.r -= coolingPwr;
						break;
					}
					if (tile->cooling.g != 0) {
						tile->cooling.g -= coolingPwr;
						break;
					}
					if (tile->cooling.b != 0) {
						tile->cooling.b -= coolingPwr;
						break;
					}
					break;
				case BALLR:
					if (tile->cooling.g != 0) {
						tile->cooling.g -= coolingPwr;
						break;
					}
					if (tile->cooling.b != 0) {
						tile->cooling.b -= coolingPwr;
						break;
					}
					tile->cooling.r += coolingPwr;
					if (tile->cooling.r == 255) tile->captured = 1;
					break;
				case BALLG:
					if (tile->cooling.r != 0) {
						tile->cooling.r -= coolingPwr;
						break;
					}
					if (tile->cooling.b != 0) {
						tile->cooling.b -= coolingPwr;
						break;
					}
					tile->cooling.g += coolingPwr;
					if (tile->cooling.g == 255) tile->captured = 1;
					break;
				case BALLB:
					if (tile->cooling.g != 0) {
						tile->cooling.g -= coolingPwr;
						break;
					}
					if (tile->cooling.r != 0) {
						tile->cooling.r -= coolingPwr;
						break;
					}
					tile->cooling.b += coolingPwr;
					if (tile->cooling.b == 255) tile->captured = 1;
					break;
				}
				if (tile->captured) {
					capturedTiles++;
					if (killCaptures) {
						delete ball;
						balls.erase(balls.begin() + b, balls.begin() + b + 1);
						b--;

						continue;
					}
				}
			}

			
			
			if (ball->nCollTime == ballVacantTime) {
				ball->vacant = 1;
				ball->outBody.setFillColor(sf::Color::White);
				if (currRule->type1 == UP_V && currRule->type2 == ball->type+DOWN_R) {
					switch (currRule->fevent) {
					case EVB_DESTROY:
						delete ball;
						balls.erase(balls.begin() + b, balls.begin() + b + 1);
						b--;
						break;
					case EVB_BREED:
						vacantBreed(ball);
						break;
					case EVB_PAINT:
						break;
					case EVB_STOP:
						break;
					}
				}
			}
		}
	}
}

bool Field::enoughBalls()
{
	return (RBalls+GBalls+BBalls > 1);
}

void Field::slowdown()
{
	for (Ball* ball : balls) {
		ball->vspeed *= 0.99;
	}
	subTick = 0;
}



void Field::autosave()
{
	std::vector<char> buffer;
	for (Ball* ball : balls) {
		buffer.push_back(0);
		buffer.push_back(ball->type);
		buffer.push_back(ball->nCollTime);
		buffer.push_back(ball->nCollTime >> 8);
		buffer.push_back(ball->pos.x);
		buffer.push_back(int(ball->pos.x) >> 8);
		buffer.push_back(ball->pos.y);
		buffer.push_back(int(ball->pos.y) >> 8);
		buffer.push_back(ball->vacant);
		buffer.push_back((int)ball->vspeed.x*10);
		buffer.push_back((int)ball->vspeed.y*10);
		
	}
	buffer.push_back(1);
	buffer.push_back(currRule->type1);
	buffer.push_back(currRule->type2);
	buffer.push_back(currRule->fevent);
	
	std::ofstream save("autosave.msv", std::ios::binary);
	save.write(buffer.data(), buffer.size());
	buffer.clear();

}

void Field::loadAutosave()
{
	std::ifstream save("autosave.msv", std::ios::binary);
	if (!save.is_open()) {
		randGen();
		return;
	}

	save.seekg(0, std::ios::end);
	int size = save.tellg();
	save.seekg(0, std::ios::beg);

	std::vector<char> buffer;
	buffer.resize(size);
	save.read(reinterpret_cast<char*>(buffer.data()), size);

	for (int i = 0; i < buffer.size(); i++) {
		if (buffer[i]) {
			currRule->type1 = buffer[i + 1];
			currRule->type2 = buffer[i + 2];
			currRule->fevent = buffer[i + 3];
			i += 4;
		}
		else {
			Ball* nball = new Ball;
			nball->type = buffer[i + 1];
			nball->nCollTime = buffer[i + 2] + ( buffer[i + 3] << 8);
			nball->pos.x = buffer[i + 4] + ( buffer[i + 5] << 8);
			nball->pos.y = buffer[i + 6] + ( buffer[i + 7] << 8);
			nball->vacant = buffer[i + 8];
			nball->vspeed.x = (float)buffer[i + 9]/10.f;
			nball->vspeed.y = (float)buffer[i + 10]/10.f;
			std::cout << nball->vspeed.y << '\n';
			i += 11;

			nball->mainBody.setRadius(ballSize);
			nball->mainBody.setFillColor(palette[BALLOFFS + nball->type]);
			nball->outBody.setRadius(ballSize * 1.4);
			if (nball->vacant) nball->outBody.setFillColor(sf::Color::White);
			else nball->outBody.setFillColor(sf::Color::Black);
			nball->mainBody.setOrigin(ballSize, ballSize);
			nball->outBody.setOrigin(ballSize * 1.4, ballSize * 1.4);
			balls.push_back(nball);


		}
	}
	buffer.clear();
}

void Field::randGen()
{

	for (int i = 0; i < RBalls; i++) {
		Ball* nball = new Ball;
		std::srand(sqrt(degree(i, 3)) * degree(i, 5) * (sin(i) / cos(i * 4 * -i)));

		nball->pos.x = rand() % int(size.x * tileSize.x - tileSize.x);
		nball->pos.y = rand() % int(size.y * tileSize.y - tileSize.y);

		nball->type = BALLR;

		nball->vspeed = stepVec(sf::Vector2f(0, 0), rand() % 360, rand() % 3 + 2);

		nball->mainBody.setRadius(ballSize);
		nball->mainBody.setFillColor(palette[nball->type + BALLOFFS]);
		nball->mainBody.setOrigin(ballSize, ballSize);

		nball->outBody.setRadius(ballSize * 1.4);
		nball->outBody.setOrigin(ballSize * 1.4, ballSize * 1.4);
		nball->outBody.setFillColor(sf::Color::Black);

		balls.push_back(nball);
	}
	for (int i = 0; i < GBalls; i++) {
		Ball* nball = new Ball;
		std::srand(sqrt(degree(i, 3)) * degree(i, 5) * (sin(i) / cos(i * 4 * -i)));

		nball->pos.x = rand() % int(size.x * tileSize.x - tileSize.x);
		nball->pos.y = rand() % int(size.y * tileSize.y - tileSize.y);

		nball->type = BALLG;

		nball->vspeed = stepVec(sf::Vector2f(0, 0), rand() % 360, rand() % 3 + 2);

		nball->mainBody.setRadius(ballSize);
		nball->mainBody.setFillColor(palette[nball->type + BALLOFFS]);
		nball->mainBody.setOrigin(ballSize, ballSize);

		nball->outBody.setRadius(ballSize * 1.4);
		nball->outBody.setOrigin(ballSize * 1.4, ballSize * 1.4);
		nball->outBody.setFillColor(sf::Color::Black);

		balls.push_back(nball);
	}
	for (int i = 0; i < BBalls; i++) {
		Ball* nball = new Ball;
		std::srand(sqrt(degree(i, 3)) * degree(i, 5) * (sin(i) / cos(i * 4 * -i)));

		nball->pos.x = rand() % int(size.x * tileSize.x - tileSize.x);
		nball->pos.y = rand() % int(size.y * tileSize.y - tileSize.y);

		nball->type = BALLB;

		nball->vspeed = stepVec(sf::Vector2f(0, 0), rand() % 360, rand() % 3 + 2);

		nball->mainBody.setRadius(ballSize);
		nball->mainBody.setFillColor(palette[nball->type + BALLOFFS]);
		nball->mainBody.setOrigin(ballSize, ballSize);

		nball->outBody.setRadius(ballSize * 1.4);
		nball->outBody.setOrigin(ballSize * 1.4, ballSize * 1.4);
		nball->outBody.setFillColor(sf::Color::Black);

		balls.push_back(nball);
	}
	for (int i = 0; i < WBalls; i++) {
		Ball* nball = new Ball;
		std::srand(sqrt(degree(i, 3)) * degree(i, 5) * (sin(i) / cos(i * 4 * -i)));

		nball->pos.x = rand() % int(size.x * tileSize.x - tileSize.x);
		nball->pos.y = rand() % int(size.y * tileSize.y - tileSize.y);

		nball->type = BALLW;

		nball->vspeed = stepVec(sf::Vector2f(0, 0), rand() % 360, rand() % 3 + 2);

		nball->mainBody.setRadius(ballSize);
		nball->mainBody.setFillColor(palette[nball->type + BALLOFFS]);
		nball->mainBody.setOrigin(ballSize, ballSize);

		nball->outBody.setRadius(ballSize * 1.4);
		nball->outBody.setOrigin(ballSize * 1.4, ballSize * 1.4);
		nball->outBody.setFillColor(sf::Color::Black);

		balls.push_back(nball);
	}
}

void Field::mapGen()
{
	tiles = new Tile[size.x * size.y];

	sf::Color coolCol(0, 0, 0);

	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			TL(x, y).mainBody.setSize(tileSize);
			TL(x, y).mainBody.setFillColor(palette[GRAYTILE] / 1.5);
			TL(x, y).mainBody.setPosition(x * tileSize.x, y * tileSize.y);

			TL(x, y).outBody.setPosition(sf::Vector2f(x * tileSize.x, y * tileSize.y) + ((tileSize / 10) / 2));
			TL(x, y).outBody.setSize(tileSize - (tileSize / 10));
			TL(x, y).outBody.setFillColor(palette[GRAYTILE]);

			TL(x, y).cooling = coolCol;

		}
	}
}
