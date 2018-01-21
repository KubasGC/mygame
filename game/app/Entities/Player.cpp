#include "Player.h"
#include <iostream>
#include "../app.h"


Player::Player()
{
	entityTexture.loadFromFile("resources/sprites/characters.png");
	entitySprite.setTexture(entityTexture);
	entitySprite.setTextureRect(sf::IntRect(0, 88, 56, 43));
	entitySprite.setOrigin(sf::Vector2f(15, 43 / 2));
	entityShape.setSize(sf::Vector2f(50, 40));
	entityShape.setFillColor(sf::Color::Blue);

	entityShape.setPosition(sf::Vector2f(130, 352));

	moveSpeed = 1.5f;
	health = 100.0f;
	damage = 15.0f;
	coolDown = 500;

	MoveType = 1;

	std::cout << "Player class has loaded\n";

	lastShoot = clock();
}

void Player::UpdatePosition()
{
	CheckDamageColor();
	entitySprite.setPosition(sf::Vector2f(entityShape.getPosition().x + 28, entityShape.getPosition().y + 21));
}

void Player::Move(sf::RenderWindow & mainWindow)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(mainWindow);
	sf::Vector2f localPosition = mainWindow.mapPixelToCoords(pixelPos);

	float tempAngleInRadians = -atan2(localPosition.x - entityShape.getPosition().x, localPosition.y - entityShape.getPosition().y);
	float tempAngle = (float)(tempAngleInRadians * (180.0 / 3.14));
	if (tempAngle < 0)
	{
		tempAngle += 360;
	}

	sf::FloatRect entityBounds = entityShape.getGlobalBounds();

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
	

	if (xPos != 0 || yPos != 0)
	{
		GetEntityMovePositionAfterCollide(entityBounds.left, entityBounds.top, &xPos, &yPos);
	}

	entityShape.setPosition(sf::Vector2f(entityBounds.left + xPos, entityBounds.top + yPos));
	
	// entitySprite.setPosition(entityShape.getPosition());
	heading = tempAngle + 90.0f;
	if (heading > 360.0f)
	{
		heading -= 360.0f;
	}
	if (heading < 0)
	{
		heading += 360.0f;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		int timeNow = clock();
		int timeTest = (int)((timeNow - lastShoot) / double(CLOCKS_PER_SEC) * 1000);
		if (timeTest > coolDown)
		{
			float bulletPosX = -sin(tempAngleInRadians) * 40;
			float bulletPosY = cos(tempAngleInRadians) * 40;

			Projectile* newBullet = new Projectile(sf::Vector2f(bulletPosX + entityBounds.left + (entityBounds.width / 2), bulletPosY + entityBounds.top + (entityBounds.height / 2)), tempAngleInRadians, heading, 20.0f, false);
			App::loadedBullets.push_back(newBullet);

			lastShoot = clock();
		}

		// lastShoot = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		
	}

	entitySprite.setRotation(heading);
	// entityShape.setRotation(heading);
}