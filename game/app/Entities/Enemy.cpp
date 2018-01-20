#include "Enemy.h"
#include <iostream>

Enemy::Enemy(sf::Vector2f startPosition)
{
	entityTexture.loadFromFile("resources/sprites/characters.png");
	entitySprite.setTexture(entityTexture);
	entitySprite.setTextureRect(sf::IntRect(424, 0, 35, 43));
	entitySprite.setOrigin(sf::Vector2f(15, 43 / 2));
	entityShape.setSize(sf::Vector2f(40, 40));
	entityShape.setFillColor(sf::Color::Blue);

	entityShape.setPosition(startPosition);
	moveSpeed = 1.5f;
	health = 100.0f;
	heading = 0.0f;
	position = startPosition;

	std::cout << "Enemy class has loaded\n";
}

void Enemy::Move(Player * playerClass)
{
	float tempAngleInRadians = -atan2(playerClass->entityShape.getPosition().x - entityShape.getPosition().x, playerClass->entityShape.getPosition().y - entityShape.getPosition().y);
	float tempAngle = tempAngleInRadians * (180.0 / 3.14);
	if (tempAngle < 0)
	{
		tempAngle += 360;
	}
	heading = tempAngle + 90.0f;

	float xPos = -sin(tempAngleInRadians) * moveSpeed;
	float yPos = cos(tempAngleInRadians) * moveSpeed;

	GetEntityMovePositionAfterCollide(position.x, position.y, &xPos, &yPos, playerClass);

	position = sf::Vector2f(position.x + xPos, position.y + yPos);
}

void Enemy::Draw(sf::RenderWindow * window)
{
	UpdatePosition();
	entitySprite.setRotation(heading);
	entityShape.setPosition(position);

	// window->draw(entityShape);
	window->draw(entitySprite);
}

void Enemy::UpdatePosition()
{
	entitySprite.setPosition(sf::Vector2f(entityShape.getPosition().x + 18, entityShape.getPosition().y + 21));
}
