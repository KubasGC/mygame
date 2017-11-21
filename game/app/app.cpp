#include "app.h"
#include <iostream>

std::vector<Textures::Texture *> App::loadedTextures;

void App::LoadTextures()
{
	const char *  tileName = "resources/tiles/forest_tiles.png";
	Textures::Texture* tempTexture = new Textures::Texture();
	tempTexture->textureId = 1;
	tempTexture->sfmlTexture.loadFromFile(tileName, sf::IntRect(0, 0, 16, 16));

	loadedTextures.push_back(tempTexture);
	std::cout << "Added texture no. 1" << std::endl;
}
