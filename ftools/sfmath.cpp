#include "sfmath.h"

void operator*=(sf::Vector2i& vec1, sf::Vector2i vec2)
{
	vec1.x *= vec2.x;
	vec1.y *= vec2.y;
}

void operator/=(sf::Vector2i& vec1, sf::Vector2i vec2)
{
	vec1.x /= vec2.x;
	vec1.y /= vec2.y;
}

sf::Vector2i operator/(sf::Vector2i vec1, sf::Vector2i vec2)
{
	return sf::Vector2i(vec1.x / vec2.x, vec1.y / vec2.y);
}

sf::Vector2i operator*(sf::Vector2i vec1, sf::Vector2i vec2)
{
	return sf::Vector2i(vec1.x * vec2.x, vec1.y * vec2.y);
}

sf::Vector2f operator/(sf::Vector2f vec1, sf::Vector2f vec2)
{
	return sf::Vector2f(vec1.x / vec2.x, vec1.y / vec2.y);
}

sf::Vector2f operator*(sf::Vector2f vec1, sf::Vector2f vec2)
{
	return sf::Vector2f(vec1.x * vec2.x, vec1.y * vec2.y);
}

sf::Vector2f operator*(sf::Vector2f vec, float num)
{
	return sf::Vector2f(vec.x * num, vec.y * num);
}

sf::Vector2f operator/(sf::Vector2f vec, float num)
{
	return sf::Vector2f(vec.x / num, vec.y / num);
}

sf::Vector2f itof(sf::Vector2i ivec) {
	return sf::Vector2f(float(ivec.x), float(ivec.y));
}

sf::Color operator/(sf::Color col, float num)
{
	return sf::Color(col.r/num, col.g/num, col.b/num);
}

sf::Color operator*(sf::Color col, float num)
{
	return sf::Color(col.r * num, col.g * num, col.b * num);
}

sf::Color operator/(sf::Color col1, sf::Color col2)
{
	sf::Uint8 red;
	sf::Uint8 green;
	sf::Uint8 blue;
	if (col2.r == 0) red = 0;
	else red = col1.r / col2.r;

	if (col2.g == 0) green = 0;
	else green = col1.g / col2.g;

	if (col2.b == 0) blue = 0;
	else blue = col1.b / col2.b;

	return sf::Color(red, green, blue);
}

sf::Color boolean(sf::Color col)
{
	return sf::Color((col.r>0),(col.g>0), (col.b>0));
}

void operator*=(sf::Vector2f& vec, float num)
{
	vec.x *= num;
	vec.y *= num;
}

void operator/=(sf::Vector2f& vec, float num)
{
	vec.x /= num;
	vec.y /= num;
}

float rad2Deg(float rads)
{
	return rads*180/PI;
}

float deg2Rad(float degs)
{
	//check doesnt it shitty
	return degs * PI / 180;
}

float degree(float num, int per)
{
	float raw = num;
	for (int i = 1; i < per; i++) num *= raw;

	return num;
}

float vecAng(sf::Vector2f vec1, sf::Vector2f vec2)
{
	return rad2Deg(atan2(vec2.y - vec1.y, vec2.x - vec1.x));
}

float vecDist(sf::Vector2f vec1, sf::Vector2f vec2)
{
	return sqrt((vec2.x-vec1.x)*(vec2.x-vec1.x)+
				(vec2.y-vec1.y)*(vec2.y-vec1.y));
}

sf::Vector2f stepVec(sf::Vector2f vec, float angle, float distance)
{
	float x = vec.x + distance * cos(deg2Rad(angle));
	float y = vec.y + distance * sin(deg2Rad(angle));

	return sf::Vector2f(x, y);
}
