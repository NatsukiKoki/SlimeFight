#include "zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>



int Zombie::spawn(map& arena, int type, int seed,int roomNum)
{
	int monsterpoint;
	switch (type)
	{
	case 0:
		// Bloater
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/bloater.png"));

		m_Speed = 40;
		m_Health = BLOATER_HEALTH;
		attk = 15;
		monsterpoint = 20;
		break;

	case 1:
		// Chaser
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/chaser.png"));

		m_Speed = 70;
		m_Health = CHASER_HEALTH;
		monsterpoint = 15;
		attk = 5;
		break;

	case 2:
		// Crawler
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/crawler.png"));

		m_Speed = 20;
		m_Health = CRAWLER_HEALTH;
		attk = 7;
		monsterpoint = 10;
		break;
	}

	// Modify the speed to make the zombie unique
	// Every zombie is unique. Create a speed modifier
	//srand((int)time(0) * seed);
	// Somewhere between 80 an 100
	float modifier = (rand() % MAX_VARRIANCE) + OFFSET+20;
	// Express as a fraction of 1
	modifier /= 100; // Now equals between .7 and 1
	m_Speed *= modifier*4;
	monsterpoint *= modifier;

	int roomNumber=roomNum;

	this->livezone.left = arena.space[arena.rooms[roomNumber]->Y][arena.rooms[roomNumber]->X]->X;
	this->livezone.top = arena.space[arena.rooms[roomNumber]->Y][arena.rooms[roomNumber]->X]->Y;
	this->livezone.width = arena.rooms[roomNumber]->Width * 50;
	this->livezone.height=arena.rooms[roomNumber]->Height*50;
	float startX;
	float startY;
	startX = Mathmatic::Box_Muller_Random(this->livezone.left + this->livezone.width / 2, this->livezone.width / 4);
	startY = Mathmatic::Box_Muller_Random(this->livezone.top + this->livezone.height / 2, this->livezone.height/4);
	if (startX < livezone.left)
		startX = livezone.left + 10;
	if (startX > livezone.left + livezone.width)
		startX = livezone.left + livezone.width - 10;
	if (startY < livezone.top)
		startY = livezone.top + 10;
	if (startY > livezone.top + livezone.height)
		startY = livezone.top + livezone.height - 10;
	m_Position.x = startX;
	m_Position.y = startY;
	faceAngle = 90.0;
	actFrec = rand() % 120 + 500;
	actlastcnt = actFrec;
	catchPlayer = false;
	attactFrec = rand() % 15 + 10;
	m_Sprite.setOrigin(25, 25);
	m_Sprite.setPosition(m_Position);
	return monsterpoint;
}

bool Zombie::attackPlayer(Time timehit, map arena, Vector2f playerPos)
{
	bool iii = true;
	if (timehit.asMilliseconds() - lastAttack.asMilliseconds() > attactFrec*100)// 2 tenths of second
	{
		lastAttack = timehit;
		m_Health -= 3;
		

		//m_Sprite.setPosition(m_Position);

		
		iii= true;
	}
	else
	{
		iii= false;
	}
	Vector2f MPBackup = m_Position;
	Vector2f dirct(0, 0);
	dirct = -playerPos + MPBackup;
	Vector2f tmp(1, tanf(faceAngle));

	dirct = Mathmatic::normailze(dirct);
	m_Position.x += dirct.x * 40;
	m_Position.y += dirct.y * 40;
	if (dirct.x * dirct.x + dirct.y * dirct.y > 0.9)
	{
		blowAwaydic = dirct;
	}
	else
		blowAwaydic =Mathmatic::normailze( tmp);


	blowAway = true;
	blowAwaySpeed = m_Speed * 30;
	return iii;
}

bool Zombie::hit(int damage,Time timehit,Vector2f dirc)
{
	m_Health-=damage;
	
	lastAttack = timehit;
	if (m_Health < 0)
	{
		// dead
		m_Alive = false;
		m_Sprite.setTexture(TextureHolder::GetTexture(
			"graphics/blood.png"));

		return true;
	}
	blowAway = true;
	blowAwaySpeed = m_Speed*3;
	blowAwaydic = dirc;
	// injured but not dead yet
	return false;
}

bool Zombie::isAlive()
{
	return m_Alive;
}

int Zombie::getattk()
{
	return attk;
}

FloatRect Zombie::getPosition()
{
	return m_Sprite.getGlobalBounds();
}


Sprite Zombie::getSprite()
{
	return m_Sprite;
}

void Zombie::update(float elapsedTime,
	Vector2f playerLocation)
{
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;
	Vector2f posbackup = m_Position;
	FloatRect player(playerLocation, Vector2f(10, 10));
	FloatRect me(m_Position, Vector2f(10, 10));
	Vector2f dict(0, 0);
	int rande = rand() % 1000;
	
	//actlastcnt--;
	if (!blowAway&&!stan)
	{
		if (livezone.intersects(player) && rande > 50 && actlastcnt <= 0)
		{
			dict = playerLocation - m_Position;
			catchPlayer = true;

			faceAngle = (atan2(dict.y,
				dict.x)
				* 180) / 3.141;
			dict = Mathmatic::normailze(dict);
			nowdict = dict;
			actFrec = rand() % 120 + 500;
			actlastcnt = actFrec;
		}
		else if (actlastcnt <= 0)
		{

			actFrec = rand() % 120 + 500;
			actlastcnt = actFrec;
			if (rand() % 100 > 40 && !catchPlayer)
			{
				dict.x = rand() % 100 - 50;
				dict.y = rand() % 100 - 50;
				faceAngle = (atan2(dict.y, dict.x) * 180) / 3.141;
			}
			else if (!catchPlayer)
				dict = Vector2f(0, 0);
			catchPlayer = false;

			dict = Mathmatic::normailze(dict);
			nowdict = dict;
		}
		else
		{
			if (catchPlayer)
			{
				dict = playerLocation - m_Position;
				catchPlayer = true;

				faceAngle = (atan2(dict.y,
					dict.x)
					* 180) / 3.141;
				dict = Mathmatic::normailze(dict);
				nowdict = dict;
			}
			actlastcnt -= rand() % 2;
		}
		m_Position += m_Speed * elapsedTime * nowdict;
	}
	else if(blowAway&&!stan)
	{
		m_Position += blowAwaySpeed * blowAwaydic*elapsedTime;
		blowAwaySpeed = blowAwaySpeed - blowAwaySpeed / 15;
		faceAngle = (atan2(blowAwaydic.y, blowAwaydic.x) * 180) / 3.141;
		if (blowAwaySpeed < 2)
		{
			blowAway = false;
			stan = true;
			stantime = 60 + rand() % 60;
		}
		faceAngle = (atan2(-blowAwaydic.y, -blowAwaydic.x) * 180) / 3.141;
	}
	else if (stan)
	{
		stantime-=rand()%2;
		if (stantime <= 0)
		{
			stan = false;
		}
	}
	// Update the zombie position variables
	me.left = m_Position.x;
	me.top = m_Position.y;
	if (!livezone.intersects(me)&&!blowAway)
		m_Position = posbackup;
	
	
	if (blowAway)
	{
		/*
		int xx = m_Position.x / 50;
		int yy = m_Position.y / 50;
		int xx_ = posbackup.x / 50;
		int yy_ = posbackup.y / 50;
		float m_BulletDistanceX = m_Position.x - posbackup.x;
		float m_BulletDistanceY = m_Position.y - posbackup.y;
		float xxx;
		float yyy;
		if (m_BulletDistanceX >= 0)
		{
			xxx = (livezone.left + livezone.width - posbackup.x);
		}
		else
		{
			xxx = posbackup.x - livezone.left;
		}
		if (m_BulletDistanceY >= 0)
		{
			yyy = (livezone.top + livezone.height - posbackup.y);
		}
		else
		{
			yyy = posbackup.y - livezone.top;
		}

		if (m_BulletDistanceY * (xxx / m_BulletDistanceX) > yyy)
		{
			m_BulletDistanceY = -m_BulletDistanceY;
		}
		else
		{
			m_BulletDistanceX = -m_BulletDistanceX;
		}*/
		if (m_Position.x > livezone.left + livezone.width)
			m_Position.x = livezone.left + livezone.width;
		if (m_Position.x < livezone.left)
			m_Position.x = livezone.left;
		if (m_Position.y > livezone.top + livezone.height)
			m_Position.y = livezone.top + livezone.height;
		if (m_Position.y < livezone.top)
			m_Position.y = livezone.top;
	}
	
	
	
	
	
	// Move the sprite






	m_Sprite.setPosition(m_Position);

	// Face the sprite in the correct direction
	

	m_Sprite.setRotation(faceAngle);


}