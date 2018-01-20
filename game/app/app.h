#pragma once

#include "Core/Core.h"
#include "Textures/Texture.h"
#include "Tiles/Tile.h"
#include "Entities/Projectile.h"

class App
{
public:
	static std::vector<Enemy *> loadedEnemies;
	static std::vector<Textures::Texture *> loadedTextures;
	static std::vector<Tile *> loadedMap;
	static std::vector<Projectile *> loadedBullets;

	static void DestroyBullet(Projectile * bulletPointer);
	static void LoadTextures();
	static sf::Sprite GetSpriteFromTexture(int textureId);

	static void LoadMapFromFile(std::string fileName);
	static void LoadPathfinderFromMap();
	static Tile * GetTileFromPosition(int x, int y);

	static Tile * GetTextureFromPosition(int left, int top);
};