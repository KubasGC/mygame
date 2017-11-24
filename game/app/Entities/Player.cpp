#include "Player.h"
#include <iostream>
#include "../app.h"


Player::Player()
{
	entityTexture.loadFromFile("resources/sprites/playersprite.png");
	entitySprite.setTexture(entityTexture);
	entitySprite.setTextureRect(sf::IntRect(0, 512, 64, 64));
	entityShape.setSize(sf::Vector2f(32, 32));
	entityShape.setFillColor(sf::Color::Blue);

	direction = 0;
	animationFrame = 0;
	moveSpeed = 1.5f;

	std::cout << "Player class has loaded\n";
}