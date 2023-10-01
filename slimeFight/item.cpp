#include "item.h"

using namespace sf;

Item::Item(Sprite s, Vector2f pos) 
{
	m_Sprite = s;
	m_Sprite.setOrigin(25, 25);
	m_Sprite.setPosition(pos);

	m_SecondsToLive = SECONDS_TO_LIVE;

	dead = false;
}

FloatRect Item::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Item::getSprite()
{
	return m_Sprite;
}

void Item::update(float elapsedTime) {
	m_SecondsToLive -= elapsedTime;

	if (m_SecondsToLive <= 0)
	{
		dead = true;
	}
}

bool Item::isDead() {
	return dead;
}

Ammo::Ammo(Vector2f pos) : Item(Sprite((TextureHolder::GetTexture("graphics/ammo.png"))), pos)
{
	
}

void Ammo::gotIt(Player* player) 
{
	player->increaseSp(20);
	dead = true;
}

MedicalKits::MedicalKits(Vector2f pos) : Item(Sprite((TextureHolder::GetTexture("graphics/health.png"))), pos)
{
	
}

void MedicalKits::gotIt(Player* player) {
	player->increaseHealth(20);
	dead = true;
}

Shoe::Shoe(Vector2f pos) : Item(Sprite(TextureHolder::GetTexture("graphics/shoe.png")), pos)
{

}

void Shoe::gotIt(Player* player)
{
	player->increaseSpeed(100);
	dead = true;
}

BulletUpgrade::BulletUpgrade(Vector2f pos) : Item(Sprite(TextureHolder::GetTexture("graphics/bullet_upgrade.png")), pos)
{
	
}

void BulletUpgrade::gotIt(Player* player)
{
	player->increaseBulletR(2);
	dead = true;
}

MoreSp::MoreSp(Vector2f pos) : Item(Sprite(TextureHolder::GetTexture("graphics/sp_upgrade.png")), pos)
{
	
}

void MoreSp::gotIt(Player* player)
{
	player->increaseMaxSp(10);
	dead = true;
}

MoreHealth::MoreHealth(Vector2f pos) : Item(Sprite(TextureHolder::GetTexture("graphics/health_upgrade.png")), pos)
{

}

void MoreHealth::gotIt(Player* player)
{
	player->increaseMaxHealth(20);
	dead = true;
}

AttackUpgrade::AttackUpgrade(Vector2f pos) : Item(Sprite(TextureHolder::GetTexture("graphics/attack_upgrade.png")), pos)
{
	
}

void AttackUpgrade::gotIt(Player* player) 
{
	player->increaseAttack(5);
	dead = true;
}