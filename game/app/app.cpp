#include "app.h"
#include <iostream>
#include <string>

#include <rapidxml/rapidxml_utils.hpp>

std::vector<Textures::Texture *> App::loadedTextures;
std::vector<Tile *> App::loadedMap;

/*void App::LoadTextures()
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


}*/

void App::LoadTextures()
{
	std::string Files[1] =
	{
		std::string("Floor")
	};

	int id = 0;

	for (int file = 0; file < 1; file++)
	{
		char fileName[200];
		sprintf_s(fileName, "resources/tiles/%s.xml", Files[file].c_str());
		rapidxml::file<> xmlFile(fileName);
		rapidxml::xml_document<> doc;

		doc.parse<0>(xmlFile.data());

		rapidxml::xml_node<> *node = doc.first_node("textures");
		rapidxml::xml_node<> *child = node->first_node();

		while (child != 0)
		{
			if (!strcmp("texture", child->name()))
			{
				Textures::Texture* tempTexture = new Textures::Texture();
				
				for (rapidxml::xml_attribute<> *attr = child->first_attribute(); attr; attr = attr->next_attribute())
				{
					if (!strcmp("left", attr->name()))
					{
						tempTexture->left = atoi(attr->value());
					}
					else if (!strcmp("top", attr->name()))
					{
						tempTexture->top = atoi(attr->value());
					}
					else if (!strcmp("width", attr->name()))
					{
						tempTexture->width = atoi(attr->value());
					}
					else if (!strcmp("height", attr->name()))
					{
						tempTexture->height = atoi(attr->value());
					}
				}
				
				char pngName[200];
				sprintf_s(pngName, "resources/tiles/%s.png", Files[file].c_str());
				if (!tempTexture->sfmlTexture.loadFromFile(pngName, sf::IntRect(tempTexture->left, tempTexture->top, tempTexture->width, tempTexture->height)))
				{
					std::cout << "Nie udalo sie zaladowac tekstury" << std::endl;
				}
				else
				{
					id++;
					tempTexture->textureId = id;
					loadedTextures.push_back(tempTexture);
					std::cout << "Zaladowano teksture #" << id << std::endl;
				}
				
			}
			child = child->next_sibling();
		}
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
				else if (!strcmp("collisions", attr->name()))
				{
					if (!strcmp("true", attr->value()))
						tempTile->collisions = true;
					else
						tempTile->collisions = false;
				}
				else if (!strcmp("x", attr->name()))
				{
					tempTile->posX = atoi(attr->value());
				}
				else if (!strcmp("y", attr->name()))
				{
					tempTile->posY = atoi(attr->value());
				}
				else if (!strcmp("scale", attr->name()))
				{
					tempTile->scale = atoi(attr->value());
				}
			}
			tempTile->tileSprite = GetSpriteFromTexture(tempTile->textureId);
			tempTile->tileSprite.setScale(tempTile->scale, tempTile->scale);
			tempTile->tileSprite.setPosition(sf::Vector2f(tempTile->posX, tempTile->posY));
			loadedMap.push_back(tempTile);
		}
		child = child->next_sibling();
	}
}