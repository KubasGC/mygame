#pragma once

#include "Core/Core.h"
#include "Textures/Texture.h"

class App
{
public:
	static std::vector<Textures::Texture *> loadedTextures;

	static void LoadTextures();
	static sf::Sprite GetSpriteFromTexture(int textureId);
};