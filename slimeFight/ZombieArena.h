#pragma once
#include "Zombie.h"
#include"map.h"
#include"trap.h"
using namespace sf;

int createBackground(VertexArray& rVA, map& arena);
Vector2i createHorde(int numZombies, map& arena,int playerroom,std::vector<Zombie> &container, std::vector<Trap> &T_container);

