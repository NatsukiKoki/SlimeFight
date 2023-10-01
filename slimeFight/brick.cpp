#include "brick.h"

const float brick::Height = 50.0;

const float brick::Width =50.0;

brick::brick(bool ifCol, float x, float y)
{
	ifCollusible = ifCol;
	X = x;
	Y = y;
	needupdate = true;
	self.setFillColor(sf::Color(0, 0, 0));
	texturetype = usoftexture::hallway;
	//_init_();
}

brick::brick()
{
	ifCollusible = false;
	X = 0;
	Y = 0;
	needupdate = false;
	self.setFillColor(sf::Color(0, 0, 0));
	//_init_();
	texturetype = usoftexture::hallway;
}
