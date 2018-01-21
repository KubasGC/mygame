#include "Entity.h"
#include "../app.h"

void Entity::CheckDamageColor()
{
	if (damageColored)
	{
		if (clock() - damageTime > 100)
		{
			damageColored = false;
			entitySprite.setColor(sf::Color(255, 255, 255, 255));
		}
	}
}

Entity::Entity()
{
	damageColored = false;
}

void Entity::UpdatePosition()
{
	entitySprite.setPosition(sf::Vector2f(entityShape.getPosition().x + 18, entityShape.getPosition().y + 21));
}

bool Entity::GetEntityMovePositionAfterCollide(float startPosX, float startPosY, float * posX, float * posY, Entity * playerClass, bool * playerCollision)
{
	bool xChanged = false;
	bool yChanged = false;

	if (playerClass != nullptr)
	{
		if (!xChanged)
		{
			sf::FloatRect entityBounds = entityShape.getGlobalBounds();
			entityBounds.left = startPosX + *(posX);
			if (entityBounds.intersects(playerClass->getEntityShape()->getGlobalBounds()))
			{
				// std::cout << "X ZMIENIONE" << std::endl;;
				xChanged = true;
				*(playerCollision) = true;
				*(posX) = 0.0f;
			}
		}
		if (!yChanged)
		{
			sf::FloatRect entityBounds = entityShape.getGlobalBounds();
			entityBounds.top = startPosY + *(posY);
			if (entityBounds.intersects(playerClass->getEntityShape()->getGlobalBounds()))
			{
				// std::cout << "X ZMIENIONE" << std::endl;;
				yChanged = true;
				*(playerCollision) = true;
				*(posY) = 0.0f;
			}
		}
	}
	for (int i = 0; i < (int)App::loadedEnemies.size(); i++)
	{
		if (App::loadedEnemies[i] != this)
		{
			if (!xChanged)
			{
				sf::FloatRect entityBounds = entityShape.getGlobalBounds();
				entityBounds.left = startPosX + *(posX);
				if (entityBounds.intersects(App::loadedEnemies[i]->getEntityShape()->getGlobalBounds()))
				{
					// std::cout << "X ZMIENIONE" << std::endl;;
					xChanged = true;
					*(posX) = 0.0f;
				}
			}
			if (!yChanged)
			{
				sf::FloatRect entityBounds = entityShape.getGlobalBounds();
				entityBounds.top = startPosY + *(posY);
				if (entityBounds.intersects(App::loadedEnemies[i]->getEntityShape()->getGlobalBounds()))
				{
					// std::cout << "X ZMIENIONE" << std::endl;;
					yChanged = true;
					*(posY) = 0.0f;
				}
			}
		}
	}
	for (int i = 0; i < (int)App::loadedMap.size(); i++)
	{
		if (App::loadedMap[i]->collisions)
		{
			// Sprawdzanie x
			if (!xChanged)
			{
				sf::FloatRect entityBounds = entityShape.getGlobalBounds();
				entityBounds.left = startPosX + *(posX);
				if (entityBounds.intersects(App::loadedMap[i]->tileSprite.getGlobalBounds()))
				{
					// std::cout << "X ZMIENIONE" << std::endl;;
					xChanged = true;
					*(posX) = 0.0f;
				}
			}
			// sprawdzanie y
			if (!yChanged)
			{
				sf::FloatRect entityBounds = entityShape.getGlobalBounds();
				entityBounds.top = startPosY + *(posY);
				if (entityBounds.intersects(App::loadedMap[i]->tileSprite.getGlobalBounds()))
				{
					// std::cout << "Y ZMIENIONE" << std::endl;;
					yChanged = true;
					*(posY) = 0.0f;
				}
			}
		}
	}
	if (xChanged || yChanged)
	{
		return true;
	}
	return false;
}

void Entity::ColorDamage()
{
	damageTime = clock();
	damageColored = true;

	entitySprite.setColor(sf::Color(255, 0, 0, 255));
}
