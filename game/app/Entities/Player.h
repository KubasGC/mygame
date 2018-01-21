#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <time.h>
class Player : public Entity
{
private:
	int lastShoot;
	int coolDown;
	float damage;

public:
	Player();

	void UpdatePosition();

	void Move(sf::RenderWindow & mainWindow);
};

