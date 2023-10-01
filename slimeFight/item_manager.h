#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "item.h"

class  ItemManager 
{
private:
	ItemManager();
public:
	static Item* getItem(sf::Vector2f pos, int no = -1);
};