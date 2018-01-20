#include "app.h"
#include <iostream>
#include <string>

#include <rapidxml/rapidxml_utils.hpp>

#include "PathFinder/PathFinder.h"
#include "PathFinder/AStar.h"

std::vector<Textures::Texture *> App::loadedTextures;
std::vector<Tile *> App::loadedMap;
std::vector<Enemy *> App::loadedEnemies;
std::vector<Projectile *> App::loadedBullets;

class MapTile : public AStarNode
{
private:
	bool m_Walkable;

	float m_x;
	float m_y;
public:
	MapTile(float x, float y, bool walkable)
	{
		m_Walkable = walkable;
		m_x = x;
		m_y = y;
	}

	~MapTile()
	{

	}


};

void App::DestroyBullet(Projectile * bulletPointer)
{
	int foundIndex = -1;
	for (int i = 0; i < (int)App::loadedBullets.size(); i++)
	{
		if (App::loadedBullets[i] == bulletPointer)
		{
			foundIndex = i;
			break;
		}
	}
	if (foundIndex != -1)
	{
		delete App::loadedBullets[foundIndex];
		App::loadedBullets.erase(App::loadedBullets.begin() + foundIndex);
	}
}

void App::DestroyEnemy(Enemy * enemyPointer)
{
	int foundIndex = -1;
	for (int i = 0; i < (int)App::loadedEnemies.size(); i++)
	{
		if (App::loadedEnemies[i] == enemyPointer)
		{
			foundIndex = i;
			break;
		}
	}
	if (foundIndex != -1)
	{
		delete App::loadedEnemies[foundIndex];
		App::loadedEnemies.erase(App::loadedEnemies.begin() + foundIndex);
	}
}

void App::DestroyEnemy(int index)
{
	delete App::loadedEnemies[index];
	App::loadedEnemies.erase(App::loadedEnemies.begin() + index);
}

void App::LoadTextures()
{
	std::string Files[1] =
	{
		// std::string("Floor"),
		std::string("Tilesheet")
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

	for (int i = 0; i < (int) loadedMap.size(); i++)
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
			tempTile->tileSprite.setScale((float)tempTile->scale, (float)tempTile->scale);
			tempTile->tileSprite.setPosition(sf::Vector2f((float)tempTile->posX, (float)tempTile->posY));
			loadedMap.push_back(tempTile);
		}
		child = child->next_sibling();
	}
}
void App::LoadPathfinderFromMap()
{

}
Tile * App::GetTileFromPosition(int x, int y)
{
	for (int i = 0; i < loadedMap.size(); i++)
	{
		if (loadedMap[i]->posX < x && loadedMap[i]->posX + (loadedMap[i]->scale * 16) > x)
		{
			if (loadedMap[i]->posY < y && loadedMap[i]->posY + (loadedMap[i]->scale * 16) > y)
			{
				return loadedMap[i];
			}
		}
	}
	return nullptr;
}
Tile * App::GetTextureFromPosition(int left, int top)
{
	for (int i = 0; i < (int)loadedMap.size(); i++)
	{
		if (loadedMap[i]->posX == left && loadedMap[i]->posY == top)
			return loadedMap[i];
	}
	return nullptr;
}
