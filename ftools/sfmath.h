
#ifndef SFMATH_H
#define SFMATH_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#define PI 3.14159265359

void operator*=(sf::Vector2i& vec1, sf::Vector2i vec2);
void operator/=(sf::Vector2i& vec1, sf::Vector2i vec2);

sf::Vector2i operator/(sf::Vector2i vec1, sf::Vector2i vec2);
sf::Vector2i operator*(sf::Vector2i vec1, sf::Vector2i vec2);

sf::Vector2f operator/(sf::Vector2f vec1, sf::Vector2f vec2);
sf::Vector2f operator*(sf::Vector2f vec1, sf::Vector2f vec2);

sf::Vector2f operator*(sf::Vector2f vec, float num);
sf::Vector2f operator/(sf::Vector2f vec, float num);


sf::Vector2f itof(sf::Vector2i ivec);


sf::Color operator/(sf::Color col, float num);
sf::Color operator*(sf::Color col, float num);

sf::Color operator/(sf::Color col1, sf::Color col2);

sf::Color boolean(sf::Color col);

void operator*=(sf::Vector2f& vec, float num);
void operator/=(sf::Vector2f& vec, float num);

float rad2Deg(float rads);
float deg2Rad(float degs);

float degree(float num, int per);

float vecAng(sf::Vector2f vec1, sf::Vector2f vec2);
float vecDist(sf::Vector2f vec1, sf::Vector2f vec2);

sf::Vector2f stepVec(sf::Vector2f vec, float angle, float distance);


inline void vout(sf::Vector2f vec, const char* name) { std::cout << "Vector\t" << name << "\t= { X: " << vec.x << ", Y: " << vec.y << " }\n"; };
inline void vout(sf::Vector2i vec, const char* name) { std::cout << "Vector\t" << name << "\t= { X: " << vec.x << ", Y: " << vec.y << " }\n"; };
inline void vout(sf::Vector2u vec, const char* name) { std::cout << "Vector\t" << name << "\t= { X: " << vec.x << ", Y: " << vec.y << " }\n"; };


#endif