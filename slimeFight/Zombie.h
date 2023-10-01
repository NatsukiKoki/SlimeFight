#pragma once
#include <SFML/Graphics.hpp>

#include"map.h"

#include"mathmatic.h"

using namespace sf;

class Zombie
{
public:
	// How fast is each zombie type?
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	// How tough is each zombie type
	const float BLOATER_HEALTH = 20;
	const float CHASER_HEALTH = 10;
	const float CRAWLER_HEALTH = 15;

	// Make each zombie vary its speed slightly
	const int MAX_VARRIANCE = 50;
	const int OFFSET = 101 - MAX_VARRIANCE;

	// Where is this zombie?
	Vector2f m_Position;

	// A sprite for the zombie
	Sprite m_Sprite;
	Sprite m_Sprite_;

	FloatRect livezone;

	float faceAngle;
	
	bool blowAway;

	Vector2f blowAwaydic;
	float blowAwaySpeed;

	int aggressive;

	int actFrec;
	int actlastcnt;
	bool catchPlayer;
	Vector2f nowdict;
	bool isAttack;
	Time lastAttack;
	int attactFrec;
	// How fast can this one run/crawl?
	float m_Speed;

	// How much health has it got?
	float m_Health;

	// Is it still alive?
	bool m_Alive;

	bool stan;
	int stantime;
	int attk;

	// Public prototypes go here	
public:

	// Handle when a bullet hits a zombie
	bool hit(int damage,Time,Vector2f);

	// Find out if the zombie is alive
	bool isAlive();
	int getattk();

	// Spawn a new zombie
	int spawn(map& arena, int type, int seed, int roomNumber);

	bool attackPlayer(Time Timehit, map arena, Vector2f playerPos);

	// Return a rectangle that is the position in the world
	FloatRect getPosition();

	// Get a copy of the sprite to draw
	Sprite getSprite();

	// Update the zombie each frame
	void update(float elapsedTime, Vector2f playerLocation);
};


