#pragma once
#include "Entity.h"
#include "Player.h"
class Enemy : public Entity
{
public:
	Enemy(sf::Vector2f startPosition);

	void Move(Player * playerClass);
	void Draw(sf::RenderWindow * window);
	void UpdatePosition();

	sf::Vector2f position;
};