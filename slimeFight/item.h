#pragma once
#include <SFML/Graphics.hpp>
#include "role.h"
#include "TextureHolder.h"

using namespace sf;

class Item
{
private:
	const float SECONDS_TO_LIVE = 10;

	Sprite m_Sprite;

	float  m_SecondsToLive;

protected:
	bool dead;

public:
	Item(Sprite s, Vector2f pos);

	FloatRect getPosition();

	Sprite getSprite();

	void update(float elapsedTime);

	bool isDead();

	virtual void gotIt(Player* player) = 0;
};

class Ammo : public Item 
{
public:
	Ammo(Vector2f pos);
	virtual void gotIt(Player* player) override;
};

class MedicalKits : public Item
{
public:
	MedicalKits(Vector2f pos);
	virtual void gotIt(Player* player) override;
};

class Shoe : public Item
{
public:
	Shoe(Vector2f pos);
	virtual void gotIt(Player* player) override;
};

class BulletUpgrade : public Item
{
public:
	BulletUpgrade(sf::Vector2f pos);
	virtual void gotIt(Player* player) override;
};

class MoreSp : public Item
{
public:
	MoreSp(sf::Vector2f pos);
	virtual void gotIt(Player* player) override;
};

class MoreHealth : public Item
{
public:
	MoreHealth(sf::Vector2f pos);
	virtual void gotIt(Player* player) override;
};

class AttackUpgrade : public Item
{
public:
	AttackUpgrade(sf::Vector2f pos);
	virtual void gotIt(Player* player) override;
};