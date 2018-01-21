#pragma once
#include "Entity.h"
#include "Player.h"
class Enemy : public Entity
{
public:
	Enemy(sf::Vector2f startPosition, float h, float s);
	bool CheckCollision(std::vector<Enemy *> & enemies, Player * player);
	void Move(Player * playerClass);
	void Draw(sf::RenderWindow * window);
	void UpdatePosition();

	sf::Vector2f position;
};