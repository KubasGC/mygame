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
	health = 100.0f;
	isFightAnim = false;

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
