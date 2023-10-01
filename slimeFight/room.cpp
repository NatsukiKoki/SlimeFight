#include "room.h"

room::room(int h, int w, int x, int y)
{
	Height = h;
	Width = w;
	X = x;
	Y = y;
	needupdate = false;
	isAlive = true;
}
