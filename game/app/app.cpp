#include "app.h"
#include <iostream>

#include <rapidxml/rapidxml_utils.hpp>

std::vector<Textures::Texture *> App::loadedTextures;
std::vector<Tile *> App::loadedMap;

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

void App::LoadMapFromFile(std::string fileName)
{

	for (int i = 0; i < loadedMap.size(); i++)
	{
		delete(loadedMap[i]);
	}
	loadedMap.clear();

	rapidxml::file<> xmlFile(fileName.c_str());
	rapidxml::xml_document<> doc;

	doc.parse<0>(xmlFile.data());

	rapidxml::xml_node<> *node = doc.first_node("map");
	rapidxml::xml_node<> *child = node->first_node();
	while (child != 0)
	{
		if (!strcmp("tile", child->name()))
		{
			Tile* tempTile = new Tile();
			for (rapidxml::xml_attribute<> *attr = child->first_attribute(); attr; attr = attr->next_attribute())
			{
				if (!strcmp("id", attr->name()))
				{
					tempTile->textureId = atoi(attr->value());
				}
				else if (!strcmp("breakable", attr->name()))
				{
					if (!strcmp("true", attr->value()))
						tempTile->breakable = true;
					else
						tempTile->breakable = false;
				}
				else if (!strcmp("x", attr->name()))
				{
					tempTile->posX = atoi(attr->value());
				}
				else if (!strcmp("y", attr->name()))
				{
					tempTile->posY = atoi(attr->value());
				}
			}
			loadedMap.push_back(tempTile);
		}
		child = child->next_sibling();
	}
}