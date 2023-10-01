#include "item_manager.h"

Item* ItemManager::getItem(sf::Vector2f pos, int no) {
	if (no == -1) {
		no = std::rand();
	}

	switch (no % 19) {
	case 0:
	case 7:
	case 13:
	case 15:
	case 17:
		return new Ammo(pos);
	case 1:
	case 8:
	case 14:
	case 16:
	case 18:
		return new MedicalKits(pos);
	case 2:
	case 9:
		return new Shoe(pos);
	case 3:
		return new BulletUpgrade(pos);
	case 4:
	case 10:
		return new MoreSp(pos);
	case 5:
	case 11:
		return new MoreHealth(pos);
	case 6:
		return new AttackUpgrade(pos);
	default:
		return nullptr;
	}
	return nullptr;
}