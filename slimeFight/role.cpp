#include "role.h"
#include "TextureHolder.h"

Role::Role(int type, sf::Sprite sprite, int start_health, Vector2f start_speed, int att, int recover, float recover_time, int sp, int recoverSp, float reSp_time, int b_r, sf::Vector2f pos) {
	m_type = type;
	m_Sprite = sprite;
	m_Health = start_health;
	m_MaxHealth = start_health;
	m_Speed = start_speed;
	moveacc = 2000;
	maxSpeed = 500;
	fict = 900;
	attack = att;
	bullet_r = b_r;
	
	m_Recover = recover;
	m_RecoverTime = recover_time;
	m_LastRecover = 0;

	m_Sp = sp;
	m_MaxSp = sp;
	m_MaxSp / 30 + 1 + m_Sp / 40;
	m_RecoverSp = recoverSp;
	m_RecoverSpTime = reSp_time;
	m_LastRecoverSp = 0;

	m_Sprite.setOrigin(25, 25);//
	m_Sprite.setPosition(pos);

	m_LastHit = sf::seconds(0);
	m_HitGap = 0.2;

	m_Toward = 1;
}

sf::FloatRect Role::getBound() {
	return m_Sprite.getGlobalBounds();
}

sf::Vector2f Role::getPosition() {
	return m_Sprite.getPosition();
}

bool Role::getDamage(int damage)
{
	this->m_Health -= damage;
	if (damage <= 0)
		return false;
	else
		return true;
}

void Role::resetPlayerStats()
{
	m_Speed = Vector2f(0,0);
	m_Health = 100;
	m_MaxHealth = 100;
}

sf::Sprite Role::getSprite()
{
	return m_Sprite;;
}

int Role::getHealth() {
	return m_Health;
}

int Role::getSp()
{
	return m_Sp;
}

int Role::getMaxSp()
{
	return m_MaxSp;
}

int Role::getSpCost()
{
	return spcost;
}

void Role::moveLeft()
{
	m_LeftPressed = true;
}

void Role::moveRight()
{
	m_RightPressed = true;
}

void Role::moveUp()
{
	m_UpPressed = true;
}

void Role::moveDown()
{
	m_DownPressed = true;
}

void Role::stopLeft()
{
	m_Toward = 1;
	m_LeftPressed = false;
}

void Role::stopRight()
{
	m_Toward = -1;
	m_RightPressed = false;
}

void Role::stopUp()
{
	m_UpPressed = false;
}

void Role::stopDown()
{
	m_DownPressed = false;
}

bool Role::hit(sf::Time time, sf::Vector2f damagefrom, int hurt) {
	if (time.asSeconds() - m_LastHit.asSeconds() >= m_HitGap) {
		m_Health -= hurt;
		if (m_Health <= 0) {
			dead = true;
		}
		m_LastHit = time;

		sf::Vector2f toward = (damagefrom - this->getPosition());

		ifblowup = true;
		
		m_LastRecover = m_RecoverTime*2;
		sf::Vector2f a = toward;
		float mol = 0.0;
		mol += a.x * a.x + a.y * a.y;
		mol = sqrtf(mol);
		if (mol < 0.0001)
		{
			a = Mathmatic::normailze(m_Speed);
			a.x = -a.x;
			a.y = -a.y;
		}
			
		else
		{
			a.x = a.x / mol;
			a.y = a.y / mol;
		}
		blowupDirect = a;
		blowupSpeed = maxSpeed * 1.5;
		
		return true;
	}
	return false;
}

sf::Vector2f Role::hit(sf::Time time, Bullet* bullet) {
	if (bullet->getType() != m_type && time.asSeconds() - m_LastHit.asSeconds() >= m_HitGap) {
		bullet->stop();
		m_Health -= bullet->getAttack();

		m_LastHit = time;

		if (m_Health <= 0)
			dead = true;

		sf::Vector2f toward = (bullet->getPosition() - this->getPosition());

		m_Speed = Vector2f(0, 0);

		sf::Vector2f a = toward;
		float mol = 0.0;
		mol += a.x * a.x + a.y * a.y;
		mol = sqrtf(mol);
		if (mol < 0.01)
			return a;
		a.x = a.x / mol;
		a.y = a.y / mol;
		return a;
	}
	return { -1, -1 };
}

bool Role::isDead() {
	return dead;
}

void Role::update(float elapsedTime, map& arena)
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
	float mol = dirct.x * dirct.x + dirct.y * dirct.y;
	Vector2f dis;
	if  (Mathmatic::mol( m_Speed) < maxSpeed&&mol>=0.9&&!ifblowup&& Mathmatic::mol(m_Speed) >2)
	{

		dis = moveacc *dirct* elapsedTime * (elapsedTime / 2) + m_Speed * elapsedTime - fict * Mathmatic::normailze(m_Speed) * elapsedTime * (elapsedTime / 2);
		m_Speed += moveacc*dirct * elapsedTime- fict * Mathmatic::normailze(m_Speed) * elapsedTime;
	}
	else if (Mathmatic::mol(m_Speed) < maxSpeed && mol >= 0.9 && !ifblowup && Mathmatic::mol(m_Speed) <= 2)
	{

		dis = moveacc * dirct * elapsedTime * (elapsedTime / 2) + m_Speed * elapsedTime ;
		m_Speed += moveacc * dirct * elapsedTime;
	}
	else if(Mathmatic::mol(m_Speed)>2&&!ifblowup)
	{
		dis = m_Speed * elapsedTime - fict* Mathmatic::normailze(m_Speed) * elapsedTime * (elapsedTime / 2);
		m_Speed -= fict * Mathmatic::normailze(m_Speed) * elapsedTime;
	}
	else if (ifblowup)
	{
		m_Speed = blowupSpeed * blowupDirect;
		dis = m_Speed * elapsedTime;
		ifblowup = false;
	}
	m_Position.x +=  dis.x;
	m_Position.y += dis.y;

	spcost = m_MaxSp / 30 + 1+m_Sp/40;
	//m_Sprite.setPosition(m_Position);

	int xx = m_Position.x / 50;
	int yy = m_Position.y / 50;
	int xx_ = MPBackup.x / 50;
	int yy_ = MPBackup.y / 50;

	if (xx < 1)
		xx = 1;
	if (xx >= arena.space[0].size()-1)
		xx = arena.space[0].size()-2;
	if (yy < 1)
		yy = 1;
	if (yy >= arena.space.size()-1)
		yy = arena.space.size()-2;
	if (xx_ < 1)
		xx_ = 1;
	if (xx_ >= arena.space[0].size()-1)
		xx_ = arena.space[0].size()-2;
	if (yy_ < 1)
		yy = 1;
	if (yy_ >= arena.space.size()-1)
		yy_ = arena.space.size()-2;


	float tanaa = dis.y / dis.x;
	

	if (arena.space[yy][xx]->texturetype == usoftexture::wall || arena.space[yy][xx]->ifCollusible)
	{
		
		if (arena.space[yy_][xx_]->texturetype == usoftexture::up || arena.space[yy_][xx_]->texturetype == usoftexture::down)
		{
			m_Speed.y = -m_Speed.y*0.3;
		}
		else if (arena.space[yy_][xx_]->texturetype == usoftexture::left || arena.space[yy_][xx_]->texturetype == usoftexture::right)
		{
			m_Speed.x = -m_Speed.x*0.3;
		}
		else
		{
			float xxxx = m_Position.x - arena.space[yy_][xx_]->X;
			float yyyy = m_Position.y - arena.space[yy_][xx_]->Y;


			if (xxxx <= 0)
			{
				m_Speed.x = -m_Speed.x*0.3;
			}
			if (yyyy <= 0)
			{
				m_Speed.y = -m_Speed.y*0.3;
			}
			xxxx = m_Position.x - arena.space[yy_][xx_]->X- arena.space[yy_][xx_]->Width;
			yyyy = m_Position.y - arena.space[yy_][xx_]->Y- arena.space[yy_][xx_]->Height;
			if (xxxx >= 0)
			{
				m_Speed.x = -m_Speed.x*0.3;
			}
			if (yyyy >= 0)
			{
				m_Speed.y = -m_Speed.y*0.3;
			}
			
		}
		m_Position = MPBackup;
	}
		


	m_Sprite.setPosition(m_Position);

	//ÅÐ¶ÏÇ½Ìå

	m_Sprite.setScale(m_Toward * 1.f, 1.f);

	if (m_Health < m_MaxHealth) {
		m_LastRecover -= elapsedTime;
		if (m_LastRecover <= 0) {
			m_LastRecover = m_RecoverTime;
			increaseHealth(m_Recover);
		}
	}
	else {
		m_LastRecover = m_RecoverTime;
	}

	if (m_Sp < m_MaxSp) {
		m_LastRecoverSp -= elapsedTime;
		if (m_LastRecoverSp <= 0) {
			m_LastRecoverSp = m_RecoverSpTime;
			increaseSp(m_RecoverSp);
			
		}
	}
	else {
		m_LastRecoverSp = m_RecoverSpTime;
	}
}

void Role::increaseHealth(int i) {
	m_Health += i;
	if (m_Health > m_MaxHealth)
		m_Health = m_MaxHealth;
}

void Role::increaseSp(int i) {
	m_Sp += i;
	if (m_Sp > m_MaxSp)
		m_Sp = m_MaxSp;
}

void Role::increaseSpeed(int i) {
	maxSpeed += i;
}

void Role::increaseBulletR(int i) {
	bullet_r += i;
}

void Role::increaseAttack(int i) {
	attack += i;
}

void Role::spawn(map& arena, Vector2f resolution, int tileSize)
{
	srand(time(0));
	int roomnumber = rand() % arena.rooms.size();
	roomnum = roomnumber;
	m_Position.x = arena.space[arena.rooms[roomnumber]->Y + arena.rooms[roomnumber]->Height / 2][arena.rooms[roomnumber]->X + arena.rooms[roomnumber]->Width / 2]->X;
	m_Position.y = arena.space[arena.rooms[roomnumber]->Y + arena.rooms[roomnumber]->Height / 2][arena.rooms[roomnumber]->X + arena.rooms[roomnumber]->Width / 2]->Y;
	// Copy the details of the arena to the player's m_Arena

	// Remember how big the tiles are in this arena
	m_TileSize = tileSize;

	// Strore the resolution for future use
	//m_Resolution.x = resolution.x;
	//m_Resolution.y = resolution.y;
}

bool Role::shoot(float targetX, float targetY, std::list<Bullet*>& bulletList) {
	if (m_Sp > spcost)
	{
		Bullet* bullet = new Bullet(m_type, attack, bullet_r, sf::Color::Green, 500, getPosition().x, getPosition().y, targetX, targetY);
		bulletList.push_back(bullet);
		m_Sp -= spcost;
		m_LastRecoverSp = m_RecoverSpTime;
		return true;
	}
	else
	{
		return false;
	}
}

Player::Player(sf::Vector2f pos) : Role(1, sf::Sprite(TextureHolder::GetTexture("graphics/Slime.png")), 100, Vector2f(0,0), 8, 1, 0.7, 100, 5, 1, 5, pos) {

}

void Role::increaseMaxHealth(int i) {
	m_MaxHealth += i;
	m_Health += i;
}

void Role::increaseMaxSp(int i) {
	m_MaxSp += i;
	m_Sp += i;
}