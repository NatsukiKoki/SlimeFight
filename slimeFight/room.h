#pragma once
#include"brick.h"
#include<vector>
//using namespace std;
class room
{
public:
	int Height;
	int Width;
	//std::vector<std::vector<brick*>> bricks;
	int  X;
	int Y;
	bool needupdate;
	bool isAlive;
	room(int h,int w,int x,int y);

};

