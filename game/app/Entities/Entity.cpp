#include "Entity.h"
#include "../app.h"

Entity::Entity()
{

}

void Entity::AnimateMove()
{
	animationFrame++;

	int tempAnim;
	float tempSpeed = moveSpeed;


	// Bieganie
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		tempSpeed *= 2;

		if (animationFrame > 16)
			animationFrame = 0;
		tempAnim = (int)floor(animationFrame / 2);
	}
	else
	{
		if (animationFrame > 32)
			animationFrame = 0;
		tempAnim = (int)floor(animationFrame / 4);
	}

	switch (direction)
	{
	case 0: // Góra

		entitySprite.setTextureRect(sf::IntRect(64 * tempAnim, 512, 64, 64));
		if (!DoesEntityCollideWithObject())
			entityShape.move(0, (float)-tempSpeed);
		break;

	case 1: // Dó³

		entitySprite.setTextureRect(sf::IntRect(64 * tempAnim, 640, 64, 64));
		if (!DoesEntityCollideWithObject())
			entityShape.move(0, (float)tempSpeed);
		break;

	case 2: // Lewo

		entitySprite.setTextureRect(sf::IntRect(64 * tempAnim, 576, 64, 64));
		if (!DoesEntityCollideWithObject())
			entityShape.move((float)-tempSpeed, 0);

		break;

	case 3: // Prawo
		entitySprite.setTextureRect(sf::IntRect(64 * tempAnim, 704, 64, 64));
		if (!DoesEntityCollideWithObject())
			entityShape.move((float)tempSpeed, 0);
		break;
	}
}

void Entity::UpdatePosition()
{
	entitySprite.setPosition(sf::Vector2f(entityShape.getPosition().x - 16, entityShape.getPosition().y - 32));
}

void Entity::ChangeAngle()
{
	switch (direction)
	{
	case 0: // Góra
		entitySprite.setTextureRect(sf::IntRect(0, 512, 64, 64));
		break;

	case 1: // Dó³
		entitySprite.setTextureRect(sf::IntRect(0, 640, 64, 64));
		break;

	case 2: // Lewo
		entitySprite.setTextureRect(sf::IntRect(0, 576, 64, 64));
		break;

	case 3: // Prawo
		entitySprite.setTextureRect(sf::IntRect(0, 704, 64, 64));
		break;
	}
}

bool Entity::DoesEntityCollideWithObject()
{
	float tempSpeed = moveSpeed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		tempSpeed *= 2;
	}

	sf::FloatRect entityBounds = entityShape.getGlobalBounds();
	switch (direction)
	{
		case 0:
			entityBounds.top -= tempSpeed;
			break;

		case 1:
			entityBounds.top += tempSpeed;
			break;

		case 2:
			entityBounds.left -= tempSpeed;
			break;

		case 3:
			entityBounds.left += tempSpeed;
			break;
	}
	for (int i = 0; i < (int)App::loadedMap.size(); i++)
	{
		if (App::loadedMap[i]->collisions)
		{
			switch (direction)
			{
			case 0: // góra
				if (entityBounds.intersects(App::loadedMap[i]->tileSprite.getGlobalBounds()))
					return true;
				break;
			case 1: // dó³
				if (entityBounds.intersects(App::loadedMap[i]->tileSprite.getGlobalBounds()))
					return true;
				break;

			case 2: // lewo
				if (entityBounds.intersects(App::loadedMap[i]->tileSprite.getGlobalBounds()))
					return true;
				break;

			case 3: // prawo
				if (entityBounds.intersects(App::loadedMap[i]->tileSprite.getGlobalBounds()))
					return true;
				break;
			}
		}
	}
	return false;
}