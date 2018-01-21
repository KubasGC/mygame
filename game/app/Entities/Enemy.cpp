#include "Enemy.h"
#include <iostream>

Enemy::Enemy(sf::Vector2f startPosition, float h, float s)
{
	entityTexture.loadFromFile("resources/sprites/characters.png");
	entitySprite.setTexture(entityTexture);
	entitySprite.setTextureRect(sf::IntRect(424, 0, 35, 43));
	entitySprite.setOrigin(sf::Vector2f(15, 43 / 2));
	entityShape.setSize(sf::Vector2f(40, 40));
	entityShape.setFillColor(sf::Color::Blue);

	entityShape.setPosition(startPosition);
	moveSpeed = s;
	health = h;
	heading = 0.0f;
	position = startPosition;

	// std::cout << "Enemy class has loaded\n";
}

bool Enemy::CheckCollision(std::vector<Enemy*>& enemies, Player * player)
{
	if (player->getEntityShape()->getGlobalBounds().intersects(entityShape.getGlobalBounds()))
	{
		return true;
	}
	for (int i = 0; i < (int)enemies.size(); i++)
	{
		if (enemies[i]->getEntityShape()->getGlobalBounds().intersects(entityShape.getGlobalBounds()))
		{
			return true;
		}
	}
	return false;
}

void Enemy::Move(Player * playerClass)
{
	float tempAngleInRadians = -atan2(playerClass->entityShape.getPosition().x - entityShape.getPosition().x, playerClass->entityShape.getPosition().y - entityShape.getPosition().y);
	float tempAngle = (float)(tempAngleInRadians * (180.0 / 3.14));
	if (tempAngle < 0)
	{
		tempAngle += 360;
	}
	heading = tempAngle + 90.0f;

	float xPos = -sin(tempAngleInRadians) * moveSpeed;
	float yPos = cos(tempAngleInRadians) * moveSpeed;
	bool playerCollide = false;
	GetEntityMovePositionAfterCollide(position.x, position.y, &xPos, &yPos, playerClass, &playerCollide);
	if (playerCollide)
	{
		playerClass->health -= 0.5f;
		playerClass->ColorDamage();
	}

	position = sf::Vector2f(position.x + xPos, position.y + yPos);
}

void Enemy::Draw(sf::RenderWindow * window)
{
	UpdatePosition();
	CheckDamageColor();
	entitySprite.setRotation(heading);
	entityShape.setPosition(position);

	// window->draw(entityShape);
	window->draw(entitySprite);
}

void Enemy::UpdatePosition()
{
	entitySprite.setPosition(sf::Vector2f(entityShape.getPosition().x + 18, entityShape.getPosition().y + 21));
}
