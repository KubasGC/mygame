#include "app.h"
#include <iostream>

std::vector<Textures::Texture *> App::loadedTextures;

void App::LoadTextures()
{
	const char *  tileName = "resources/tiles/forest_tiles.png";
	Textures::Texture* tempTexture = new Textures::Texture();
	tempTexture->textureId = 0;
	if (tempTexture->sfmlTexture.loadFromFile(tileName, sf::IntRect(0, 0, 64, 64)))
	{
		loadedTextures.push_back(tempTexture);
		std::cout << "Added texture no. 1" << std::endl;
	}
	else
	{
		std::cout << "Texture no. 1 isn't loaded." << std::endl;
		system("pause");
		exit(1);
	}

	
}

sf::Sprite App::GetSpriteFromTexture(int textureId)
{
	return sf::Sprite(loadedTextures[textureId]->sfmlTexture);
}
