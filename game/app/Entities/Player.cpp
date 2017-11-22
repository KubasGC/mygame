#include "Player.h"
#include <iostream>


Player::Player()
{
	playerTexture.loadFromFile("resources/sprites/playersprite.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 512, 64, 64));
	playerShape.setSize(sf::Vector2f(32, 32));
	playerShape.setFillColor(sf::Color::Blue);

	direction = 0;
	animation = 0;
	movementSpeed = 2;

	canMoveUp = true;
	canMoveDown = true;
	canMoveLeft = true;
	canMoveRight = true;


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

void Player::ResetAbleToMove()
{
	canMoveUp = true;
	canMoveDown = true;
	canMoveLeft = true;
	canMoveRight = true;
}

sf::Sprite * Player::GetPlayerSprite()
{
	return &playerSprite;
}

sf::Shape * Player::GetPlayerShape()
{
	return &playerShape;
}

void Player::SetDirection(int dir)
{
	if (direction != dir)
	{
		direction = dir;
		ChangeAngle();
	}
}

int Player::GetDirection()
{
	return direction;
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
		if (canMoveUp)
		{
			playerSprite.setTextureRect(sf::IntRect(64 * tempAnim, 512, 64, 64));
			playerShape.move(0, -movementSpeed);
			ResetAbleToMove();
		}
		break;

	case 1: // Dó³
		if (canMoveDown)
		{
			playerSprite.setTextureRect(sf::IntRect(64 * tempAnim, 640, 64, 64));
			playerShape.move(0, movementSpeed);
			ResetAbleToMove();
		}
		break;

	case 2: // Lewo
		if (canMoveLeft)
		{
			playerSprite.setTextureRect(sf::IntRect(64 * tempAnim, 576, 64, 64));
			playerShape.move(-movementSpeed, 0);
			ResetAbleToMove();
		}
		break;

	case 3: // Prawo
		if (canMoveRight)
		{
			playerSprite.setTextureRect(sf::IntRect(64 * tempAnim, 704, 64, 64));
			playerShape.move(movementSpeed, 0);
			ResetAbleToMove();
		}
		break;
	}

}

void Player::UpdatePosition()
{
	//playerSprite.setPosition(playerShape.getPosition());
	playerSprite.setPosition(sf::Vector2f(playerShape.getPosition().x - 16, playerShape.getPosition().y - 32));
}

void Player::ToggleMove(int dir, bool toggle)
{
	switch (dir)
	{
		case 0:
			canMoveUp = toggle;
			break;

		case 1:
			canMoveDown = toggle;
			break;

		case 2:
			canMoveLeft = toggle;
			break;

		case 3:
			canMoveRight = toggle;
			break;
	}
}
