#pragma once
#include <SFML/Graphics.hpp>
#include "map.h"

class Bullet {
private:
	sf::CircleShape m_BulletShape;

	float m_BulletSpeed;

	int attack;

	bool dead;

	float m_BulletDistanceX;
	float m_BulletDistanceY;

	float life;
	int bondetimes;
	Vector2f dic;
	int from_type;

public:
	Bullet(int type, int att, float r, sf::Color color, float speed, float startX, float startY, float targetX, float targetY);

	bool isDead();

	void stop();

	Vector2f getDic();

	sf::CircleShape getShape();

	sf::FloatRect getBound();

	sf::Vector2f getPosition();

	void update(float elapsedTime, map arena);
	int getAttack();

	int getType();
};