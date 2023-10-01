#include "bullet.h"


// The constructor
Bullet::Bullet()
{
	m_BulletShape.setSize(sf::Vector2f(2, 2));
}

void Bullet::shoot(float startX, float startY,
	float targetX, float targetY)
{
	// Keep track of the bullet
	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;

	bondetimes = 3;

	// Calculate the gradient of the flight path
	float gradient = (startX - targetX) / (startY - targetY);

	// Any gradient less than zero needs to be negative
	if (gradient < 0)
	{
		gradient *= -1;

	}

	// Calculate the ratio between x and t
	float ratioXY = m_BulletSpeed / (1 + gradient);

	// Set the "speed" horizontally and vertically
	m_BulletDistanceY = ratioXY;
	m_BulletDistanceX = ratioXY * gradient;
	
	// Point the bullet in the right direction
	if (targetX < startX)
	{
		m_BulletDistanceX *= -1;
	}

	if (targetY < startY)
	{
		m_BulletDistanceY *= -1;
	}

	// Finally, assign the results to the
	// member variables
	m_XTarget = targetX;
	m_YTarget = targetY;

	// Set a max range of 1000 pixels
	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;
	
	// Position the bullet ready to be drawn
	m_BulletShape.setPosition(m_Position);
}

void Bullet::stop()
{
	m_InFlight = false;
}

bool Bullet::isInFlight()
{
	return m_InFlight;
}

FloatRect Bullet::getPosition()
{
	return m_BulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape()
{
	return m_BulletShape;
}


void Bullet::update(float elapsedTime, map arena)
{
	Vector2f backup = m_Position;
	// Update the bullet position variables
	m_Position.x += m_BulletDistanceX * elapsedTime;
	m_Position.y += m_BulletDistanceY * elapsedTime;

	// Move the bullet
	m_BulletShape.setPosition(m_Position);

	// Has the bullet gone out of range?
	int xx = m_Position.x / 50;
	int yy = m_Position.y / 50;
	int xx_ = backup.x / 50;
	int yy_ = backup.y / 50;
	float xxx;
	float yyy;
	if (m_BulletDistanceX >= 0)
	{
		xxx =(arena.space[yy_][xx_]->X + arena.space[yy_][xx_]->Width - backup.x);
	}
	else
	{
		xxx = backup.x - arena.space[yy_][xx_]->X;
	}
	if (m_BulletDistanceY >= 0)
	{
		yyy = (arena.space[yy_][xx_]->Y + arena.space[yy_][xx_]->Height - backup.y);
	}
	else
	{
		yyy = backup.y - arena.space[yy_][xx_]->Y;
	}

	if (arena.space[yy][xx]->texturetype == usoftexture::wall || arena.space[yy][xx]->ifCollusible)
	{
		if (bondetimes > 0)
		{
			bondetimes--;
			if (arena.space[yy_][xx_]->texturetype == usoftexture::left || arena.space[yy_][xx_]->texturetype == usoftexture::right)
			{
				m_BulletDistanceX = -m_BulletDistanceX;
			}
			else if (arena.space[yy_][xx_]->texturetype == usoftexture::up || arena.space[yy_][xx_]->texturetype == usoftexture::down)
			{
				m_BulletDistanceY = -m_BulletDistanceY;
			}
			else if (m_BulletDistanceY * (xxx / m_BulletDistanceX) > yyy)
			{
				m_BulletDistanceY = -m_BulletDistanceY;
			}
			else
			{
				m_BulletDistanceX = -m_BulletDistanceX;
			}
		}
		else
		{
			m_InFlight = false;
		}
	}

}