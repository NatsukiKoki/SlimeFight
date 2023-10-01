#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <list>
#include "bullet2.h"
#include"map.h"

class Role 
{
protected:
	sf::Vector2f m_Position;

	sf::Sprite m_Sprite;

	//sf::Texture m_Texture;

	int m_TileSize;

	Vector2f m_Speed;

	int attack;
	int bullet_r;

	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	int m_Health;
	int m_MaxHealth;
	int m_Recover;
	float m_RecoverTime;
	float m_LastRecover;

	int m_Sp;
	int m_MaxSp;
	int  m_RecoverSp;
	float m_RecoverSpTime;
	float m_LastRecoverSp;

	float moveacc;

	float maxSpeed;

	float fict;




	sf::Time m_LastHit;
	float m_HitGap;

	bool ifblowup;

	Vector2f blowupDirect;
	float blowupSpeed;

	bool stan;

	Time stantime;

	bool dead;

	int spcost;

	int m_Toward;

	int m_type;

public:
	int roomnum;
	Role(int type, sf::Sprite sprite, int start_health, Vector2f start_speed, int att, int recover, float recover_time, int sp, int recoverSp, float recoverSp_time, int b_r, sf::Vector2f pos);

	bool hit(sf::Time time, sf::Vector2f damagefrom, int hurt);

	sf::Vector2f hit(sf::Time, Bullet* bullet);

	sf::FloatRect getBound();

	sf::Vector2f getPosition();

	bool getDamage(int damage);
	void resetPlayerStats();
	sf:: Sprite getSprite();

	int getHealth();
	
	int getSp();
	int getMaxSp();
	int getSpCost();
	void moveLeft();

	void moveRight();

	void moveUp();

	void moveDown();

	void stopLeft();
	void increaseMaxHealth(int i);
	void increaseMaxSp(int i);
	void stopRight();

	void stopUp();

	void stopDown();

	bool isDead();

	void update(float elapsedTime, map& arena);

	bool shoot(float targetX, float targetY, std::list<Bullet*>& bulletList);

	void increaseHealth(int i);

	void increaseSp(int i);

	void increaseSpeed(int i);

	void increaseBulletR(int i);

	void increaseAttack(int i);
	void spawn(map& arena, Vector2f resolution, int tileSize);
};

class Player : public Role {
public:
	Player(sf::Vector2f pos);
};