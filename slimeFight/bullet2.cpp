#include "bullet2.h"

Bullet::Bullet(int type, int att, float r, sf::Color color, float speed, float startX, float startY, float targetX, float targetY) : m_BulletShape(r, 30) {
	m_BulletShape.setFillColor(color);
	m_BulletShape.setOutlineColor(sf::Color::Black);
	m_BulletShape.setOutlineThickness(2.0f);
	m_BulletShape.setPosition(startX, startY);

	m_BulletSpeed = speed;

	from_type = type;

	attack = att;

	dead = false;

	life = 5.0;
	bondetimes = 3;

	float gradient = (startX - targetX) / (startY - targetY);

	if (gradient < 0){
		gradient *= -1;
	}

	

	float ratioXY = m_BulletSpeed / (1 + gradient);

	m_BulletDistanceY = ratioXY;
	m_BulletDistanceX = ratioXY * gradient;

	if (targetX < startX){
		m_BulletDistanceX *= -1;
	}

	if (targetY < startY){
		m_BulletDistanceY *= -1;
	}
	dic = Vector2f(m_BulletDistanceX, m_BulletDistanceY);
	dic = Mathmatic::normailze(dic);
}

bool Bullet::isDead() {
	return dead;
}

void Bullet::stop()
{
	dead = true;
}

Vector2f Bullet::getDic()
{
	return dic;
}

sf::CircleShape Bullet::getShape()
{
	return this->m_BulletShape;
}

sf::FloatRect Bullet::getBound() {
	return m_BulletShape.getGlobalBounds();
}

sf::Vector2f Bullet::getPosition() {
	return m_BulletShape.getPosition();
}

int Bullet::getAttack() {
	return attack;
}

int Bullet::getType() {
	return from_type;
}

void Bullet::update(float elapsedTime, map arena)
{
	
	// Update the bullet position variables
	sf::Vector2f m_Position = this->getPosition();
	sf::Vector2f backup = m_Position;
	m_Position.x += m_BulletDistanceX * elapsedTime;
	m_Position.y += m_BulletDistanceY * elapsedTime;

	// Move the bullet
	

	int xx = m_Position.x / 50;
	int yy = m_Position.y / 50;
	int xx_ = backup.x / 50;
	int yy_ = backup.y / 50;
	if (xx < 1)
		xx = 1;
	if (xx >= arena.space[0].size() - 1)
		xx = arena.space[0].size() - 2;
	if (yy < 1)
		yy = 1;
	if (yy >= arena.space.size() - 1)
		yy = arena.space.size() - 2;
	if (xx_ < 1)
		xx_ = 1;
	if (xx_ >= arena.space[0].size() - 1)
		xx_ = arena.space[0].size() - 2;
	if (yy_ < 1)
		yy = 1;
	if (yy_ >= arena.space.size() - 1)
		yy_ = arena.space.size() - 2;


	float xxx;
	float yyy;
	if (m_BulletDistanceX >= 0)
	{
		xxx = (arena.space[yy_][xx_]->X + arena.space[yy_][xx_]->Width - backup.x);
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
		bondetimes--;
		if (bondetimes > 0)
		{
			
			if (arena.space[yy_][xx_]->texturetype == usoftexture::left || arena.space[yy_][xx_]->texturetype == usoftexture::right)
			{
				m_BulletDistanceX = -m_BulletDistanceX;
			}
			else if (arena.space[yy_][xx_]->texturetype == usoftexture::up || arena.space[yy_][xx_]->texturetype == usoftexture::down)
			{
				m_BulletDistanceY = -m_BulletDistanceY;
			}
			
			else
			{
				float xxxx = m_Position.x - arena.space[yy_][xx_]->X;
				float yyyy = m_Position.y - arena.space[yy_][xx_]->Y;


				if (xxxx <= 0)
				{
					m_BulletDistanceX=-m_BulletDistanceX;
				}
				if (yyyy <= 0)
				{
					m_BulletDistanceY=-m_BulletDistanceY;
				}
				xxxx = m_Position.x - arena.space[yy_][xx_]->X - arena.space[yy_][xx_]->Width;
				yyyy = m_Position.y - arena.space[yy_][xx_]->Y - arena.space[yy_][xx_]->Height;
				if (xxxx >= 0)
				{
					m_BulletDistanceX = -m_BulletDistanceX;
				}
				if (yyyy >= 0)
				{
					m_BulletDistanceY = -m_BulletDistanceY;
				}
			}
		}
		else
		{
			dead = true;
		}
	}
	m_BulletShape.setPosition(m_Position);
	life -= elapsedTime;

	if (life <= 0) {
		dead = true;
	}

}