#include "Player.h"
#include <iostream>
#include "../app.h"


Player::Player()
{
	entityTexture.loadFromFile("resources/sprites/characters.png");
	entitySprite.setTexture(entityTexture);
	entitySprite.setTextureRect(sf::IntRect(390, 176, 35, 43));
	entitySprite.setOrigin(sf::Vector2f(35 / 2, 43 / 2));
	entityShape.setSize(sf::Vector2f(40, 40));
	entityShape.setFillColor(sf::Color::Blue);

	direction = 0;
	animationFrame = 0;
	moveSpeed = 1.5f;
	health = 100.0f;
	isFightAnim = false;

	MoveType = 1;

	std::cout << "Player class has loaded\n";
}

void Player::AnimateFight()
{
	if (isFightAnim)
	{
		int tempAnim = (int)floor(m_fightAnim / 3);

		switch (direction)
		{
			case 0:
				entitySprite.setTextureRect(sf::IntRect(64 * tempAnim, 768, 64, 64));
				break;

			case 1:
				entitySprite.setTextureRect(sf::IntRect(64 * tempAnim, 896, 64, 64));
				break;

			case 2:
				entitySprite.setTextureRect(sf::IntRect(64 * tempAnim, 832, 64, 64));
				break;

			case 3:
				entitySprite.setTextureRect(sf::IntRect(64 * tempAnim, 960, 64, 64));
				break;
		}
		m_fightAnim++;
		if (m_fightAnim > 15)
		{
			m_fightAnim = 0;
			isFightAnim = false;
			ChangeAngle();
		}
	}
}

void Player::Move(sf::RenderWindow & mainWindow)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(mainWindow);
	sf::Vector2f localPosition = mainWindow.mapPixelToCoords(pixelPos);

	float tempAngleInRadians = -atan2(localPosition.x - entityShape.getPosition().x, localPosition.y - entityShape.getPosition().y);
	float tempAngle = tempAngleInRadians * (180.0 / 3.141592653589793238463);
	if (tempAngle < 0)
	{
		tempAngle += 360;
	}

	float xPos = 0;
	float yPos = 0;
	float speed = 5.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		// xPos += -sin(tempAngleInRadians) * 5;
		// yPos += cos(tempAngleInRadians) * 5;
		yPos -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		yPos += speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		xPos -= speed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			xPos += speed / 2;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		xPos += speed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			xPos -= speed / 2;
		}
	}

	entityShape.move(sf::Vector2f(xPos, yPos));
	entitySprite.setPosition(entityShape.getPosition());
	heading = tempAngle + 90.0f;
	if (heading > 360.0f)
	{
		heading -= 360.0f;
	}
	if (heading < 0)
	{
		heading += 360.0f;
	}
	entitySprite.setRotation(heading);
	entityShape.setRotation(heading);
}

bool Player::getFightAnim() const
{
	return isFightAnim;
}

void Player::setFightAnim()
{
	if (!isFightAnim)
	{
		isFightAnim = true;
		m_fightAnim = 0;
	}
}
