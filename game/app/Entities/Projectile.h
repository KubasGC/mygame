#pragma once
#include <SFML/Graphics.hpp>
#include "../Tiles//Tile.h"

class Projectile
{
private:
	sf::Vector2f startPosition;
	float bulletHeadingRad;
	float bulletHeading;
	float bulletSpeed;

	float width;
	float height;

	sf::Vector2f bulletPosition;
	sf::Vector2f bulletStartPosition;
	sf::RectangleShape shape;
public:
	Projectile(sf::Vector2f position, float headingRad, float heading, float speed);
	void Draw(sf::RenderWindow * window);
	bool doesProjectileShouldBeRemoved(std::vector<Tile *> & map);
	sf::RectangleShape * getShape();
};