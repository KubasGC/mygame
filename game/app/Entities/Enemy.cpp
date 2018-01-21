#include "Enemy.h"
#include <iostream>

#include "../app.h"

Enemy::Enemy(sf::Vector2f startPosition, float h, float s, int type)
{
	

	enemyType = type;
	switch (enemyType)
	{
		case 1:
			entityTexture.loadFromFile("resources/sprites/characters.png");
			entitySprite.setTexture(entityTexture);
			entitySprite.setTextureRect(sf::IntRect(424, 0, 35, 43));
			entitySprite.setOrigin(sf::Vector2f(15, 43 / 2));
			entityShape.setSize(sf::Vector2f(40, 40));
			entityShape.setFillColor(sf::Color::Blue);
			break;

		case 2:
			// srand(time(NULL));
			entityTexture.loadFromFile("resources/sprites/characters.png");
			entitySprite.setTexture(entityTexture);
			entitySprite.setTextureRect(sf::IntRect(55, 176, 54, 43));
			entitySprite.setOrigin(sf::Vector2f(15, 43 / 2));
			entityShape.setSize(sf::Vector2f(50, 40));
			entityShape.setFillColor(sf::Color::Blue);
			shootTime = clock() + (rand() % 5000);
			break;
	}

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

	if (enemyType == 2)
	{
		int timeTest = (int)((clock() - shootTime) / double(CLOCKS_PER_SEC) * 1000);
		if (timeTest > 3000)
		{
			shootTime = clock();

			float bulletPosX = -sin(tempAngleInRadians) * 40;
			float bulletPosY = cos(tempAngleInRadians) * 40;

			sf::FloatRect entityBounds = entityShape.getGlobalBounds();

			Projectile* newBullet = new Projectile(sf::Vector2f(bulletPosX + entityBounds.left + (entityBounds.width / 2), bulletPosY + entityBounds.top + (entityBounds.height / 2)), tempAngleInRadians, heading, 15.0f, true);
			App::loadedBullets.push_back(newBullet);
		}
	}
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
