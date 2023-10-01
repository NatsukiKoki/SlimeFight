#pragma once

#include<SFML/Graphics.hpp>

enum class usoftexture
{
	middle,
	up,
	left,
	right,
	down,
	leftup,
	rightup,
	leftdown,
	rightdown,
	hallway,
	wall,
};

class brick
{
public:
	float X;
	float Y;
	static const float Height;
	static const float Width;
	bool ifCollusible;
	bool needupdate;
	sf::RectangleShape self;
	usoftexture texturetype;

	brick(bool ifCol,float x,float y);
	brick();
	
};

