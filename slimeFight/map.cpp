#include "map.h"

const int map::HUGE = 665544;

map::map(int wid, int hei)
{
	int pixX = 0;
	int pixY = 0;



	height = hei;
	width = wid;
	for (int i = 0; i < height; i++)
	{
		std::vector<brick*>tkp;
		for (int j = 0; j < width; j++)
		{
			brick* tmp = new brick(true, pixX, pixY);
			
			pixX += tmp->Width;
			tkp.emplace_back(tmp);
			
		}
		space.emplace_back(tkp);
		pixY += tkp[0]->Height;
		pixX = 0;
	}
	
}

void map::setRandomRoom(int trytimes)
{
	Vector2i averageRoomSize(15,10);
	Vector2i stdDeviationd(2,3);
	srand(time(0));
	Vector2i roomPos;
	int num=trytimes/50+1;
	int j = 0;
	int failtime = 0;
	for (int i = 0; i < num;)
	{
		
		Vector2i roomSize;
		roomSize.x = abs((int)Mathmatic::Box_Muller_Random(averageRoomSize.x, stdDeviationd.x));
		roomSize.y = abs((int)Mathmatic::Box_Muller_Random(averageRoomSize.y, stdDeviationd.y));
		roomPos.x += rand() % (averageRoomSize.x*2);
	    roomPos.x = roomPos.x % width;
		roomPos.y += rand() % (averageRoomSize.y * 2);
		roomPos.y = roomPos.y % height;
		room* tmp=new room(roomSize.y,roomSize.x,roomPos.x,roomPos.y);
		j++;
		if (j > trytimes)
		{
			i++;
			j = 0;
			failtime++;
		}
			//i++;
		if (ifPutAble(tmp)&&tmp->Width>5&&tmp->Height>5)
		{
			int k = 0;
			for (int i = 0; i < tmp->Height; i++)
				for (int j = 0; j < tmp->Width; j++)
				{
					space[i + tmp->Y][j + tmp->X]->ifCollusible = false;
					space[i + tmp->Y][j + tmp->X]->needupdate = true;
						
				}
			i++;
			rooms.emplace_back(tmp);
			std::cout << "/////" << std::endl;
		}


	}
	std::cout << failtime << "\n";
}

bool map::ifPutAble(room* tmp)
{
	int count=0;
	double sp = tmp->Height * width;
	bool ifSPCover = false;

	if (tmp->X + tmp->Width >= width-10 || tmp->Y + tmp->Height >= height-10||tmp->X<=10||tmp->Y<=10)
		return false;
	else
	{
		for (int i = -11; i < tmp->Height+11; i++)
		{
			for (int j = -11; j < tmp->Width+11; j++)
			{
				if (!space[i + tmp->Y][j + tmp->X]->ifCollusible)
				{
					count++;
				}
				//count++;
			}
		}
		if (count==0)
		{
   			return true;
		}
		else
		{
			return false;
		}
	}
}

void map::beforeCreateMaze()
{
	bool flag = true;
	bool flag2 = true;
	std::vector<Vector2i> changepoint;
	for (int i = 10; i < space.size()-10; i++)
	{
		for (int j = 10; j < space[i].size()-10; j++)
		{
			if ( flag&&flag2 && space[i][j]->ifCollusible)
			{
				if (space[i + 1][j]->ifCollusible && space[i - 1][j]->ifCollusible
					&& space[i + 1][j + 1]->ifCollusible && space[i][j + 1]->ifCollusible && space[i - 1][j + 1]->ifCollusible
					&& space[i + 1][j - 1]->ifCollusible && space[i][j - 1]->ifCollusible && space[i - 1][j - 1]->ifCollusible)
					changepoint.emplace_back(Vector2i(i, j));
				
			}
			flag = !flag;
		}
		
			flag2 = !flag2;
	}
	for (int i = 0; i < changepoint.size(); i++)
	{
		space[changepoint[i].x][changepoint[i].y]->ifCollusible = false;
		space[changepoint[i].x][changepoint[i].y]->needupdate = true;
	}
	for (int i = 10; i < space.size() - 10; i++)
	{
		for (int j = 10; j < space[i].size() - 10; j++)
		{

			
			if (space[i + 1][j]->ifCollusible && space[i - 1][j]->ifCollusible && space[i][j]->ifCollusible
				&& space[i + 1][j + 1]->ifCollusible && space[i][j + 1]->ifCollusible && space[i - 1][j + 1]->ifCollusible
				&& space[i + 1][j - 1]->ifCollusible && space[i][j - 1]->ifCollusible && space[i - 1][j - 1]->ifCollusible)
			{
				space[i][j]->ifCollusible = false;
				space[i][j]->needupdate = true;
			}
				

		}

	}
	for (int i = 10; i < space.size() - 10; i++)
	{
		for (int j = 10; j < space[i].size() - 10; j++)
		{
			int cnt = 0;
			for(int a=-1;a<2;a++)
				for (int b = -1; b < 2; b++)
				{
					if (space[i + a][j + b]->ifCollusible)
					{
						cnt++;
					}
				}

			if (cnt>=8)
			{
				space[i][j]->ifCollusible = false;
				space[i][j]->needupdate = true;
			}


		}

	}

	
}

int map::findBrick(brick* bb)
{
	int tmp=-1;
	brick* ttp = NULL;
	for (int i = 0; i < throughBrick.size(); i++)
	{
		if (bb == throughBrick[i].first)
		{
			tmp = i;
		}
	}

	return tmp;
}

int map::findBrickFather(int index)
{
	int tmp = index;
	if (index != throughBrick[index].second)
	{
		tmp = findBrickFather(throughBrick[index].second);
	}
	throughBrick[index].second = tmp;
	return tmp;
}

int map::numOfNotThrough()
{
	int cnt=0;
	int tmp=-1;
	for (int i = 0; i < throughBrick.size(); i++)
	{
		if (tmp != findBrickFather(throughBrick[i].second))
		{
			cnt++;
			tmp = throughBrick[i].second;
		}
	}
	return cnt;
}

void map::resize(int a,int b)
{
	int pixX = 0;
	int pixY = 0;
	width = a;
	height = b;
	std::vector<std::vector<brick*>>().swap(space);
	std::vector<room*>().swap(rooms);
	std::vector<RectangleShape>().swap( visableBrick);
	std::vector<std::pair<brick*, int>>().swap( throughBrick);
	std::vector < std::tuple <brick*, std::pair<int, int>, bool> >().swap( walls);
	for (int i = 0; i < b; i++)
	{
		std::vector<brick*>tkp;
		for (int j = 0; j < a; j++)
		{
			brick* tmp = new brick(true, pixX, pixY);

			pixX += tmp->Width;
			tkp.emplace_back(tmp);

		}
		space.emplace_back(tkp);
		pixY += tkp[0]->Height;
		pixX = 0;
	}

	
}

void map::testShow(Texture *collusiable, Texture *uncollusiable)
{
	Image red;
	red.create(10, 10, Color::Red);
	Image blue;
	blue.create(10, 10, Color::Blue);
	Image yellow;
	yellow.create(10, 10, Color::Yellow);

	Texture styleA;
	Texture styleB;
	Texture styleC;
	styleC.loadFromImage(yellow);
	styleA.loadFromImage(red);
	styleB.loadFromImage(blue);
	int sz = space.size() * space[0].size();
	std::vector<RectangleShape>().swap(visableBrick);
	for (int i = 0; i < space.size(); i++)
	{
		for (int j = 0; j < space[i].size(); j++)
		{
			
				if (space[i][j]->ifCollusible)
				{
					//tmp.setTexture(collusiable);
				}
				else
				{
					space[i][j]->self.setSize(Vector2f(space[i][j]->Width, space[i][j]->Height));
					space[i][j]->self.setPosition(space[i][j]->X, space[i][j]->Y);
					space[i][j]->self.setTexture(uncollusiable);
					//space[i][j]->self.setFillColor(Color(120, 120, 120));
					RectangleShape tno;
					tno.setSize(Vector2f(space[i][j]->Width, space[i][j]->Height));
					tno.setPosition(space[i][j]->X, space[i][j]->Y);
					if (space[i][j]->texturetype == usoftexture::middle)
						tno.setTexture(uncollusiable);
					else if (space[i][j]->texturetype == usoftexture::hallway)
						tno.setTexture(collusiable);
					else if (space[i][j]->texturetype == usoftexture::wall)
						tno.setFillColor(Color(3, 230, 230));
					visableBrick.emplace_back(tno);
				}
				
			
			/*else if (space[i][j]->needupdate)
			{
				std::vector<RectangleShape>::iterator tmp;
				tmp = std::find(visableBrick.begin(), visableBrick.end(), space[i][j]);
				if (space[i][j]->ifCollusible)
				{
					
					if (tmp != visableBrick.end())
					{
						visableBrick.erase(tmp, visableBrick.end());

					}
				}
				else
				{
					space[i][j]->self.setSize(Vector2f(space[i][j]->Width, space[i][j]->Height));
					space[i][j]->self.setPosition(space[i][j]->X, space[i][j]->Y);
					space[i][j]->self.setTexture(uncollusiable);
					space[i][j]->self.setFillColor(Color(120, 120, 120));
					visableBrick.emplace_back(space[i][j]->self);
				}
				space[i][j]->needupdate = false;
				
				
			}*/
		}
	}
}

int map::numofVisable()
{
	int ret = 0;
	for (int i = 0; i < space.size(); i++)
		for (int j = 0; j < space[i].size(); j++)
			if (space[i][j]->ifCollusible == false)
				ret++;
	return ret;
}

void map::setbrick()
{
	for (int i = 0; i < rooms.size(); i++)
	{
		for (int j = rooms[i]->Y; j < rooms[i]->Height + rooms[i]->Y; j++)
			for (int k = rooms[i]->X; k < rooms[i]->Width + rooms[i]->X; k++)
			{
				if (j == rooms[i]->Y && k == rooms[i]->X)
				{
					space[j][k]->texturetype = usoftexture::leftup;
				}
				else if (j == rooms[i]->Y && k == rooms[i]->Width + rooms[i]->X - 1)
				{
					space[j][k]->texturetype = usoftexture::rightup;
				}
				else if (j == rooms[i]->Y)
				{
					space[j][k]->texturetype = usoftexture::up;
				}
				else if (j == rooms[i]->Height + rooms[i]->Y - 1 && k == rooms[i]->X)
				{
					space[j][k]->texturetype = usoftexture::leftdown;
				}
				else if (j == rooms[i]->Height + rooms[i]->Y - 1 && k == rooms[i]->Width + rooms[i]->X - 1)
				{
					space[j][k]->texturetype = usoftexture::rightdown;
				}
				else if (j == rooms[i]->Height + rooms[i]->Y - 1)
				{
					space[j][k]->texturetype = usoftexture::down;
				}
				else if (k == rooms[i]->X)
				{
					space[j][k]->texturetype = usoftexture::left;
				}
				else if (k == rooms[i]->X + rooms[i]->Width - 1)
				{
					space[j][k]->texturetype = usoftexture::right;
				}
				else
				{
					space[j][k]->texturetype = usoftexture::middle;
				}
			}
	}
		std::vector< Vector2i> hallwaypos;
		/*
		for (int k = rooms[i]->X; k < rooms[i]->Width + rooms[i]->X; k++)
		{
			int j = rooms[i]->Y;
			bool ifhallway = true;
			if (space[k - 1][j]->texturetype != usoftexture::hallway
				&& space[k][j - 1]->texturetype != usoftexture::hallway
				&& space[k + 1][j]->texturetype != usoftexture::hallway
				&& space[k][j + 1]->texturetype != usoftexture::hallway)
				ifhallway = false;
			if (ifhallway)
				hallwaypos.emplace_back(Vector2i(j, k));
			 j = rooms[i]->Y+rooms[i]->Height-1;
			 ifhallway = true;
			 if (space[k - 1][j]->texturetype != usoftexture::hallway
				 && space[k][j - 1]->texturetype != usoftexture::hallway
				 && space[k + 1][j]->texturetype != usoftexture::hallway
				 && space[k][j + 1]->texturetype != usoftexture::hallway)
				 ifhallway = false;
			if (ifhallway)
				hallwaypos.emplace_back(Vector2i(j, k));
		}
		for (int j = rooms[i]->Y; j < rooms[i]->Height + rooms[i]->Y; j++)
		{
			int k = rooms[i]->X;
			bool ifhallway = true;
			if (space[k - 1][j]->texturetype != usoftexture::hallway
				&& space[k][j - 1]->texturetype != usoftexture::hallway
				&& space[k + 1][j]->texturetype != usoftexture::hallway
				&& space[k][j + 1]->texturetype != usoftexture::hallway)
				ifhallway = false;
			if (ifhallway)
				hallwaypos.emplace_back(Vector2i(j, k));
			k = rooms[i]->X + rooms[i]->Width - 1;
			ifhallway = true;
			if (space[k - 1][j]->texturetype != usoftexture::hallway
				&& space[k][j - 1]->texturetype != usoftexture::hallway
				&& space[k + 1][j]->texturetype != usoftexture::hallway
				&& space[k][j + 1]->texturetype != usoftexture::hallway)
				ifhallway = false;
			if (ifhallway)
				hallwaypos.emplace_back(Vector2i(j, k));
		}
		for (int j = 0; j < hallwaypos.size(); j++)
		{
			//space[hallwaypos[j].y][hallwaypos[j].x]->texturetype = usoftexture::hallway;
		}
		}*/
	std::vector<Vector2i> upPoint;
	for (int i = 0; i < space.size() ; i++)
	{
		for (int j = 0; j < space[i].size() ; j++)
		{
			if (space[i][j]->ifCollusible)
			{
				int cnt = 0;
				for(int a=i-1;a<space.size()&&a<=i+1;a++)
					for (int b = j - 1; b<space[i].size()&&b <= j + 1; b++)
					{
						if (a < 0)
							a = 0;
						if (b < 0)
							b = 0;
						if (!space[a][b]->ifCollusible)
							cnt++;
					}
				if (cnt != 0)
				{
					upPoint.emplace_back(Vector2i(i, j));
					
				}
			}
		}
	}
	for (int k = 0; k < upPoint.size(); k++)
	{
		int i = upPoint[k].x;
		int j = upPoint[k].y;
		space[i][j]->ifCollusible = false;
		space[i][j]->texturetype = usoftexture::wall;
	}

}



void map::makeRoute()
{
	/*srand(time(0));
	srand(rand());
	std::vector<room*> reached;
	std::vector<std::pair<room*, bool>> allRooms;//bool is for reached or not,true reference reached
	for (int i = 0; i < rooms.size(); i++)
	{
		allRooms.emplace_back(std::make_pair(rooms[i], false));
	}
	allRooms[0].second = true;
	reached.emplace_back(allRooms[0].first);
	while (reached.size() < rooms.size())
	{
		int index = rand() % allRooms.size();
		for (; allRooms[index].second; index++)
		{
			index = index % allRooms.size();
		}
		int startRoom = rand() % reached.size();

		int tkpr = rand() % 4;
		Vector2i startPoint(0,0);
		if (tkpr == 0)
		{
			startPoint.y = reached[startRoom]->Y-1;
			startPoint.x = reached[startRoom]->X + rand() % reached[startRoom]->Width;
		}
		else if (tkpr == 1)
		{
			startPoint.x = reached[startRoom]->X - 1;
			startPoint.y = reached[startRoom]->Y + rand() % reached[startRoom]->Height;
		}
		else if (tkpr == 2)
		{
			startPoint.y = reached[startRoom]->Y + reached[startRoom]->Height;
			startPoint.x = reached[startRoom]->X + rand() % reached[startRoom]->Width;
		}
		else if (tkpr == 3)
		{
			startPoint.x = reached[startRoom]->X +reached[startRoom]->Width;
			startPoint.y = reached[startRoom]->Y + rand() % reached[startRoom]->Height;
		}

		FloatRect endroom(allRooms[index].first->X, allRooms[index].first->Y, allRooms[index].first->Width, allRooms[index].first->Height);



	}*/

	
	int tkkp = 0;
	for (int i = 1; i < space.size() - 1; i++)
	{
		for (int j = 1; j < space[i].size() - 1; j++)
		{
			if (space[i][j]->ifCollusible == false)
			{
				throughBrick.emplace_back(std::make_pair(space[i][j], tkkp));
				tkkp++;
			}
		}
	}

	for (int i = 1; i < space.size() - 1; i++)
	{
		for (int j = 1; j < space[i].size() - 1; j++)
		{


			 if (space[i][j]->ifCollusible == false && space[i - 1][j]->ifCollusible == false)
			{
				 int indexA = findBrick(space[i - 1][j]);
				 int indexB = findBrick(space[i][j]);
				 int fatA = findBrickFather(indexA);
				 int fatB = findBrickFather(indexB);
				 int min = fatA > fatB ? fatB : fatA;
				 throughBrick[fatA].second = min;
				 throughBrick[fatB].second = min;
				 findBrickFather(indexA);
				 findBrickFather(indexB);
			}
			else if (space[i][j]->ifCollusible == false && space[i][j - 1]->ifCollusible == false)
			{
				 int indexA = findBrick(space[i][j-1]);
				 int indexB = findBrick(space[i][j]);
				 int fatA = findBrickFather(indexA);
				 int fatB = findBrickFather(indexB);
				 int min = fatA > fatB ? fatB : fatA;
				 throughBrick[fatA].second = min;
				 throughBrick[fatB].second = min;
				 findBrickFather(indexA);
				 findBrickFather(indexB);
			}

		}

	}

	for (int i = 1; i < space.size() - 1; i++)
	{
		for (int j = 1; j < space[i].size() - 1; j++)
		{
			if (space[i][j]->ifCollusible)
			{
				if (!space[i + 1][j]->ifCollusible && !space[i - 1][j]->ifCollusible
					&& space[i][j - 1]->ifCollusible && space[i][j + 1]->ifCollusible)
				{
					int a = findBrickFather(findBrick(space[i + 1][j])), b = findBrickFather(findBrick(space[i - 1][j]));
					if (a != b)
						walls.emplace_back(std::make_tuple(space[i][j], std::make_pair(a, b), true));
				}
				else if (space[i + 1][j]->ifCollusible && space[i - 1][j]->ifCollusible
					&& !space[i][j - 1]->ifCollusible && !space[i][j + 1]->ifCollusible)
				{
					int a = findBrickFather(findBrick(space[i][j + 1])), b = findBrickFather(findBrick(space[i][j - 1]));
					if (a != b)
						walls.emplace_back(std::make_tuple(space[i][j], std::make_pair(a, b), true));
				}

			}
		}
	}

	int kkp = numOfNotThrough();

	while (kkp > 1)
	{
		int index = rand() % walls.size();
		int counter = 0;
		for (; !std::get<2>(walls[index]); )
		{
			index++;
			counter++;
			if (counter > walls.size())
				break;
			index = index % walls.size();
		}
		if (counter > walls.size())
			break;
		int a = findBrickFather(std::get<1>(walls[index]).first);
		int b = findBrickFather(std::get<1>(walls[index]).second);
		if (a == b)
		{
			if (rand() % 10000<2&&rand()>0)
			{
				throughBrick.emplace_back(std::make_pair(std::get<0>(walls[index]), a));
				std::get<0>(walls[index])->ifCollusible = false;
				std::get<0>(walls[index])->needupdate = true;
				
			}
			std::get<2>(walls[index]) = false;
			kkp = numOfNotThrough();
		}
		else
		{
			int tmp = a > b ? b : a;
			
			throughBrick.emplace_back(std::make_pair(std::get<0>(walls[index]), tmp));
			throughBrick[a].second = throughBrick[b].second = tmp;
			std::get<0>(walls[index])->ifCollusible = false;
			std::get<0>(walls[index])->needupdate = true;
			std::get<2>(walls[index]) = false;

			kkp = numOfNotThrough();
		}
	}
	return;

}

void map::degenerate()
{

	for (int i = 1; i < space.size() - 1; i++)
	{
		for (int j = 1; j < space[i].size() - 1; j++)
		{
			if (space[i][j]->ifCollusible == false)
			{
				
				subDegenerate(i, j);

			}
		}
	}

}
void map::subDegenerate(int i, int j)
{
	int con = 0;
	Vector2i nextpoint;
	if (!space[i][j]->ifCollusible)
	{
		if (space[i][j - 1]->ifCollusible)
			con++;
		else
		{
			nextpoint.x = i;
			nextpoint.y = j - 1;
		}
		if (space[i][j + 1]->ifCollusible)
			con++;
		else
		{
			nextpoint.x = i;
			nextpoint.y = j + 1;
		}
		if (space[i - 1][j]->ifCollusible)
			con++;
		else
		{
			nextpoint.x = i - 1;
			nextpoint.y = j;
		}
		if (space[i + 1][j]->ifCollusible)
			con++;
		else
		{
			nextpoint.x = i + 1;
			nextpoint.y = j;
		}
		if (con >= 3)
		{
			space[i][j]->ifCollusible = true;
			space[i][j]->needupdate = true;
			subDegenerate(nextpoint.x, nextpoint.y);
		}
	}
}

