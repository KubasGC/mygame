#pragma once

#include "Core/Core.h"
#include "Textures/Texture.h"
#include "Tiles/Tile.h"

class App
{
public:
	static std::vector<Textures::Texture *> loadedTextures;
	static std::vector<Tile *> loadedMap;

	static void LoadTextures();
	static sf::Sprite GetSpriteFromTexture(int textureId);

	static void LoadMapFromFile(std::string fileName);
};