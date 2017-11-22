#include "Player.h"
#include <iostream>
#include "../app.h"


Player::Player()
{
	playerTexture.loadFromFile("resources/sprites/playersprite.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 512, 64, 64));
	playerShape.setSize(sf::Vector2f(32, 32));
	playerShape.setFillColor(sf::Color::Blue);

	direction = 0;
	animation = 0;
	movementSpeed = 1.5f;

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

bool Player::DoesPlayerHasCollision()
{
	sf::FloatRect playerBounds = playerShape.getGlobalBounds();
	for (int i = 0; i < (int)App::loadedMap.size(); i++)
	{
		if (App::loadedMap[i]->collisions)
		{
			switch (direction)
			{
			case 0: // góra
				playerBounds.top -= movementSpeed;
				if (playerBounds.intersects(App::loadedMap[i]->tileSprite.getGlobalBounds()))
					return true;
				break;
			case 1: // dó³
				playerBounds.top += movementSpeed;
				if (playerBounds.intersects(App::loadedMap[i]->tileSprite.getGlobalBounds()))
					return true;
				break;

			case 2: // lewo
				playerBounds.left -= movementSpeed;
				if (playerBounds.intersects(App::loadedMap[i]->tileSprite.getGlobalBounds()))
					return true;
				break;

			case 3: // prawo
				playerBounds.left += movementSpeed;
				if (playerBounds.intersects(App::loadedMap[i]->tileSprite.getGlobalBounds()))
					return true;
				break;
			}
		}
	}
	return false;
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

	int tempAnim = (int)floor(animation / 4);

	switch (direction)
	{
	case 0: // Góra

		playerSprite.setTextureRect(sf::IntRect(64 * tempAnim, 512, 64, 64));
		if (!DoesPlayerHasCollision())
			playerShape.move(0, (float)-movementSpeed);
		break;

	case 1: // Dó³

		playerSprite.setTextureRect(sf::IntRect(64 * tempAnim, 640, 64, 64));
		if (!DoesPlayerHasCollision())
			playerShape.move(0, (float)movementSpeed);
		break;

	case 2: // Lewo

		playerSprite.setTextureRect(sf::IntRect(64 * tempAnim, 576, 64, 64));
		if (!DoesPlayerHasCollision())
			playerShape.move((float)-movementSpeed, 0);

		break;

	case 3: // Prawo
		playerSprite.setTextureRect(sf::IntRect(64 * tempAnim, 704, 64, 64));
		if (!DoesPlayerHasCollision())
			playerShape.move((float)movementSpeed, 0);

		break;
	}

}

void Player::UpdatePosition()
{
	//playerSprite.setPosition(playerShape.getPosition());
	playerSprite.setPosition(sf::Vector2f(playerShape.getPosition().x - 8, playerShape.getPosition().y - 16));
}