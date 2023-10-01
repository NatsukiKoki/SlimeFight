#pragma once

#include"brick.h"
#include"room.h"
#include<SFML/Graphics.hpp>
#include<SFML/Main.hpp>
#include<vector>
#include<iostream>
#include"mathmatic.h"


//using namespace std;
using namespace sf;

class map
{
public:
	//int roomNum;
	//int columnNum;
	//int rowNum;
	int height;
	int width;
	//int playerPositionX;
	//int playerPositionY;
	//int numofVis;
	//float playerPointX;
	//float playerPointY;
	static const int HUGE ;
	std::vector<std::vector<brick*>> space;
	std::vector<room*> rooms;
	std::vector<RectangleShape> visableBrick;
	std::vector<std::pair<brick*, int>> throughBrick;
	std::vector < std::tuple <brick*, std::pair<int, int>,bool> > walls;


	map(int wid, int hei);
	void resize(int a, int b);

	int findBrick(brick* bb);
	
	int findBrickFather(int index);
	int numOfNotThrough();

	void setRandomRoom(int trytimes);
	bool ifPutAble(room* tmp);
	
	void beforeCreateMaze();

	void makeRoute();
	
	void degenerate();
	void subDegenerate(int i,int j);
	void setbrick();

	void testShow(Texture *collusiable, Texture *uncollusiable);
	int numofVisable();
};

