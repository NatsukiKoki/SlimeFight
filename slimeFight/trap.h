#pragma once
#include "Zombie.h"
class trap
{
};

class Trap :
    public Zombie
{
private:
    bool isAttack;
    //Sprite m_Sprite_;
    //Sprite Sprite___;

public:
    int attackPoint;
    int spawn(map& arena, int type, int seed, int roomNumber);
    int attack(Time Timehit, map arena, Vector2f playerPos);
    void update(Time time, Vector2f playerLocation);
    Sprite getSprite__(int k);
};

