#include "Enemy.h"
#include <iostream>

Enemy::Enemy(sf::Vector2f startPosition)
{
	entityTexture.loadFromFile("resources/sprites/playersprite.png");
	entitySprite.setTexture(entityTexture);
	entitySprite.setTextureRect(sf::IntRect(0, 512, 64, 64));
	entityShape.setSize(sf::Vector2f(32, 32));
	entityShape.setFillColor(sf::Color::Blue);

	entityShape.setPosition(startPosition);
	entitySprite.setPosition(sf::Vector2f(entityShape.getPosition().x - 16, entityShape.getPosition().y - 32));


	direction = 1;
	animationFrame = 0;
	moveSpeed = 1.5f;
	health = 100.0f;

	ChangeAngle();

	std::cout << "Enemy class has loaded\n";
}

void Enemy::DoJob()
{

}
