#include "Player.h"
#include <iostream>


Player::Player()
{
	playerTexture.loadFromFile("resources/sprites/playersprite.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 512, 64, 64));
	direction = 0;
	animation = 0;
	movementSpeed = 2;

	std::cout << "Player class has loaded\n";
}


Player::~Player()
{

}

void Player::ChangeAngle()
{
	switch (direction)
	{
	case 0: // Góra
		playerSprite.setTextureRect(sf::IntRect(0, 512, 64, 64));
		break;

	case 1: // Dó³
		playerSprite.setTextureRect(sf::IntRect(0, 640, 64, 64));
		break;

	case 2: // Lewo
		playerSprite.setTextureRect(sf::IntRect(0, 576, 64, 64));
		break;

	case 3: // Prawo
		playerSprite.setTextureRect(sf::IntRect(0, 704, 64, 64));
		break;
	}
}

sf::Sprite * Player::GetPlayerSprite()
{
	return &playerSprite;
}

void Player::SetDirection(int dir)
{
	if (direction != dir)
	{
		direction = dir;
		ChangeAngle();
	}
}

void Player::AnimateMove()
{
	animation++;
	if (animation > 32)
		animation = 0;

	int tempAnim = (int) floor(animation / 4);

	switch (direction)
	{
	case 0: // Góra
		playerSprite.setTextureRect(sf::IntRect(64 * tempAnim, 512, 64, 64));
		playerSprite.move(0, -movementSpeed);
		break;

	case 1: // Dó³
		playerSprite.setTextureRect(sf::IntRect(64 * tempAnim, 640, 64, 64));
		playerSprite.move(0, movementSpeed);
		break;

	case 2: // Lewo
		playerSprite.setTextureRect(sf::IntRect(64 * tempAnim, 576, 64, 64));
		playerSprite.move(-movementSpeed, 0);
		break;

	case 3: // Prawo
		playerSprite.setTextureRect(sf::IntRect(64 * tempAnim, 704, 64, 64));
		playerSprite.move(movementSpeed, 0);
		break;
	}

}
