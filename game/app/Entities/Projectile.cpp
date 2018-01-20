#include <iostream>
#include "Projectile.h"
#include <iostream>

Projectile::Projectile(sf::Vector2f position, float headingRad, float heading, float speed)
{
	bulletPosition = position;
	bulletHeading = heading;
	bulletHeadingRad = headingRad;
	bulletSpeed = speed;
	bulletStartPosition = position;

	shape.setSize(sf::Vector2f(10, 5));
	shape.setFillColor(sf::Color::Green);
	shape.setOrigin(sf::Vector2f(5, 2.5));
	shape.setRotation(bulletHeading);
	shape.setPosition(bulletPosition);

	// std::cout << "Pocisk stworzony\n";
}

void Projectile::Draw(sf::RenderWindow * window)
{
	float bulletPosX = -sin(bulletHeadingRad) * bulletSpeed;
	float bulletPosY = cos(bulletHeadingRad) * bulletSpeed;
	bulletPosition = sf::Vector2f(bulletPosition.x + bulletPosX, bulletPosition.y + bulletPosY);
	shape.setPosition(bulletPosition);
	window->draw(shape);

	
}

bool Projectile::doesProjectileShouldBeRemoved(std::vector<Tile *> & map, std::vector<Enemy *> & enemies, int * enemyIndex)
{
	double distance = sqrt(pow(bulletStartPosition.x - bulletPosition.x, 2) + pow(bulletStartPosition.y - bulletPosition.y, 2));
	if (distance > 1000)
	{
		return true;
	}
	sf::FloatRect futureProjectilePos = shape.getGlobalBounds();
	futureProjectilePos.left += -sin(bulletHeadingRad) * bulletSpeed;
	futureProjectilePos.top += cos(bulletHeadingRad) * bulletSpeed;

	for (int i = 0; i < (int)enemies.size(); i++)
	{
		if (futureProjectilePos.intersects(enemies[i]->getEntityShape()->getGlobalBounds()))
		{
			*(enemyIndex) = i;
			return true;
		}
	}
	for (int i = 0; i < (int)map.size(); i++)
	{
		if (map[i]->collisions)
		{
			if (futureProjectilePos.intersects(map[i]->tileSprite.getGlobalBounds()))
			{
				return true;
			}
		}
	}
	return false;
}

sf::RectangleShape * Projectile::getShape()
{
	return &shape;
}
