#pragma once
#include <SFML/Graphics.hpp>
#include "../Tiles/Tile.h"
#include "Enemy.h"
#include "Player.h"

class Projectile
{
private:
	sf::Vector2f startPosition;
	float bulletHeadingRad;
	float bulletHeading;
	float bulletSpeed;

	float width;
	float height;
	bool isEnemyProjectile;

	sf::Vector2f bulletPosition;
	sf::Vector2f bulletStartPosition;
	sf::RectangleShape shape;
public:
	Projectile(sf::Vector2f position, float headingRad, float heading, float speed, bool enemyProjectile = false);
	void Draw(sf::RenderWindow * window);
	bool doesProjectileShouldBeRemoved(std::vector<Tile *> & map, std::vector<Enemy *> & enemies, int * enemyIndex, Player * playerClass, bool * playerHit);
	sf::RectangleShape * getShape();
};