#pragma once
#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include <SFML/Graphics.hpp>

class Projectile
{
private:
	sf::Vector2f startPosition;
	float heading;
	float speed;
public:
	void Move();
};
#endif