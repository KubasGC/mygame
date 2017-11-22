#pragma once
#include <SFML/Graphics.hpp>
struct Tile
{
	int textureId;
	bool breakable;
	bool collisions;
	int posX;
	int posY;
	sf::Sprite tileSprite;
};