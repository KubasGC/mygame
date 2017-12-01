#pragma once
#ifndef  _ENEMY_H_
#define _ENEMY_H_

#include "Entity.h"
class Enemy : public Entity
{
public:
	Enemy(sf::Vector2f startPosition);

	void DoJob();
};

#endif // ! _ENEMY_H_