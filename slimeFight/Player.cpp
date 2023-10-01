#include "player.h"
#include "TextureHolder.h"




Player::Player()
{
	m_Speed = START_SPEED*4;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	// Associate a texture with the sprite
	// !!Watch this space!!
	m_Sprite = Sprite(TextureHolder::GetTexture(
		"graphics/player.png"));

	// Set the origin of the sprite to the centre, 
	// for smooth rotation
	m_Sprite.setOrigin(25, 25);
}

bool Player::getDamage(int damage)
{
	this->m_Health -= damage;
	if (damage<=0)
		return false;
	else
		return true;
}


void Player::resetPlayerStats()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
}

void Player::spawn(map& arena, Vector2f resolution, int tileSize)
{
	// Place the player in the middle of the arena
	
	//
	

}

Time Player::getLastHitTime()
{
	return m_LastHit;
}

bool Player::hit(Time timeHit, map& arean,Vector2f zombieplace)
{
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 1000)// 2 tenths of second
	{
		m_LastHit = timeHit;
		m_Health -= 10;
		Vector2f MPBackup = m_Position;
		Vector2f dirct(0, 0);
		dirct = zombieplace - MPBackup;

		dirct = Mathmatic::normailze(dirct);
		//m_Position.x += dirct.x * 40;
		//m_Position.y += dirct.y * 40;


		//m_Sprite.setPosition(m_Position);

		int xx = m_Position.x / 50;
		int yy = m_Position.y / 50;

		if (arean.space[yy][xx]->texturetype == usoftexture::wall || arean.space[yy][xx]->ifCollusible)
			m_Position = MPBackup;
		m_Sprite.setPosition(m_Position);

		return true;
	}
	else
	{
		return false;
	}

}

FloatRect Player::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Vector2f Player::getCenter()
{
	return m_Position;
}

float Player::getRotation()
{
	return m_Sprite.getRotation();
}

Sprite Player::getSprite()
{
	return m_Sprite;
}

int Player::getHealth()
{
	return m_Health;
}

void Player::moveLeft()
{
	m_LeftPressed = true;
}

void Player::moveRight()
{
	m_RightPressed = true;
}

void Player::moveUp()
{
	m_UpPressed = true;
}

void Player::moveDown()
{
	m_DownPressed = true;
}

void Player::stopLeft()
{
	m_LeftPressed = false;
}

void Player::stopRight()
{
	m_RightPressed = false;
}

void Player::stopUp()
{
	m_UpPressed = false;
}

void Player::stopDown()
{
	m_DownPressed = false;
}

void Player::update(float elapsedTime, Vector2i mousePosition,map& arena)
{

	Vector2f MPBackup = m_Position;
	Vector2f dirct(0, 0);

	if (m_UpPressed)
	{
		dirct.y -= 1;
	}

	if (m_DownPressed)
	{
		dirct.y += 1;
	}

	if (m_RightPressed)
	{
		dirct.x += 1;
	}

	if (m_LeftPressed)
	{
		dirct.x -= 1;
	}

	dirct = Mathmatic::normailze(dirct);
	m_Position.x += dirct.x * m_Speed * elapsedTime;
	m_Position.y += dirct.y * m_Speed * elapsedTime;


	//m_Sprite.setPosition(m_Position);

	int xx = m_Position.x / 50;
	int yy = m_Position.y / 50;

	if (arena.space[yy][xx]->texturetype == usoftexture::wall||arena.space[yy][xx]->ifCollusible)
		m_Position = MPBackup;

	// Keep the player in the arena
	/*if (m_Position.x > m_Arena.width - m_TileSize)
	{
		m_Position.x = m_Arena.width - m_TileSize;
	}

	if (m_Position.x < m_Arena.left + m_TileSize)
	{
		m_Position.x = m_Arena.left + m_TileSize;
	}

	if (m_Position.y > m_Arena.height - m_TileSize)
	{
		m_Position.y = m_Arena.height - m_TileSize;
	}

	if (m_Position.y < m_Arena.top + m_TileSize)
	{
		m_Position.y = m_Arena.top + m_TileSize;
	}*/

	// Calculate the angle the player is facing
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2,
		mousePosition.x - m_Resolution.x / 2)
		* 180) / 3.141;
	m_Sprite.setPosition(m_Position);
	m_Sprite.setRotation(angle);
}

void Player::upgradeSpeed()
{
	// 20% speed upgrade
	m_Speed += (START_SPEED * .2);
}

void Player::upgradeHealth()
{
	// 20% max health upgrade
	m_MaxHealth += (START_HEALTH * .2);

}

void Player::increaseHealthLevel(int amount)
{
	m_Health += amount;

	// But not beyond the maximum
	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}

