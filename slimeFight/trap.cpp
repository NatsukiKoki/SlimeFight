#include "trap.h"
#include "TextureHolder.h"


int Trap::attack(Time timehit, map arena, Vector2f playerPos)
{
	int iii = 0;
	if (isAttack)// 2 tenths of second
	{
		lastAttack = timehit;
		m_Health -= 3;
		isAttack = false;

		//m_Sprite.setPosition(m_Position);


		iii = attackPoint;
	}
	else
	{
		iii = 0;
	}
	return iii;
}

void Trap::update(Time time, Vector2f playerLocation)
{
	if (!isAttack&&time.asMilliseconds() - lastAttack.asMilliseconds() > attactFrec * 120)
	{
		isAttack = true;
	}
	if (!isAttack)
	{
		m_Sprite.setColor(Color(155, 0, 155));
	}
	else
	{
		m_Sprite.setColor(Color(255, 255, 255));
	}
	//isAttack = !isAttack;
}

Sprite Trap::getSprite__(int k)
{
	//if (isAttack)
		return m_Sprite;
	//else
		//return m_Sprite_;
}

int Trap::spawn(map& arena, int type, int seed, int roomNum)
{
	int monsterpoint;
	bool tmpp = false;
	switch (type)
	{
	case 0:
		// Bloater
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/trap1_2.png"));
		m_Sprite_ = Sprite(TextureHolder::GetTexture(
			"graphics/trap1_1.png"));

		m_Speed = 40;
		m_Health = BLOATER_HEALTH;
		monsterpoint = 30;
		break;

	case 1:
		// Chaser
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/trap2_1.png"));
		m_Sprite_ = Sprite(TextureHolder::GetTexture(
			"graphics/trap2_2.png"));

		m_Speed = 70;
		m_Health = CHASER_HEALTH;
		monsterpoint = 15;
		break;

	case 2:
		// Crawler
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/trap3_1.png"));
		m_Sprite_ = Sprite(TextureHolder::GetTexture(
			"graphics/trap3_2.png"));

		m_Speed = 20;
		m_Health = CRAWLER_HEALTH;
		monsterpoint = 10;
		break;
	}

	// Modify the speed to make the zombie unique
	// Every zombie is unique. Create a speed modifier
	//srand((int)time(0) * seed);
	// Somewhere between 80 an 100
	float modifier = (rand() % MAX_VARRIANCE) + OFFSET + 20;
	// Express as a fraction of 1
	modifier /= 100; // Now equals between .7 and 1
	m_Speed *= modifier * 4;
	//monsterpoint *= modifier;

	int roomNumber = roomNum;

	this->livezone.left = arena.space[arena.rooms[roomNumber]->Y][arena.rooms[roomNumber]->X]->X;
	this->livezone.top = arena.space[arena.rooms[roomNumber]->Y][arena.rooms[roomNumber]->X]->Y;
	this->livezone.width = arena.rooms[roomNumber]->Width * 50;
	this->livezone.height = arena.rooms[roomNumber]->Height * 50;
	float startX;
	float startY;
	isAttack = true;
	startX=rand()%(int)(this->livezone.width)+livezone.left;
	startY = rand() % (int)(this->livezone.height)+livezone.top;
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
	attackPoint = rand() % 10+5;
	monsterpoint += monsterpoint * attackPoint / 10.0 / 2;
	attactFrec = rand() % 15 + 10;
	m_Sprite.setOrigin(25, 25);
	m_Sprite.setPosition(m_Position);
	m_Alive = true;
	return monsterpoint;
}