#include "ZombieArena.h"
#include "Zombie.h"

Vector2i createHorde(int numZombies, map& arena,int playerRoom,std::vector<Zombie> &M_container,std::vector<Trap> &T_container)
{
	int monster_num=0;
	int trap_num = 0;

	for (int i = 0; i < arena.rooms.size()-1; i++)

	{

		// Which side should the zombie spawn
		//srand((int)time(0) * i);
		int X=i;
		int point = rand() % 50 + 50;
		Zombie tmp;
		Trap tmp_;
		while (X != playerRoom&&point>0)
		{
			if (rand() % 100>10)
			{

				int type = (rand() % 3);

				point -= tmp.spawn(arena, type, i, X);
				M_container.emplace_back(tmp);
				monster_num++;
			}
			else
			{
				int type = (rand() % 3);

				point -= tmp_.spawn(arena, type, i, X)/2;
				T_container.emplace_back(tmp_);
				trap_num++;
			}
			// Spawn the new zombie into the array
			//zombies[i].spawn(arena, type, i, roomNums);
		}

	}
	return Vector2i(monster_num,trap_num);
}