#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <time.h>
class Player : public Entity
{
private:
	bool isFightAnim;

	int m_fightAnim;
	int lastShoot;

public:
	Player();

	void UpdatePosition();
	void AnimateFight();

	void Move(sf::RenderWindow & mainWindow);

	bool getFightAnim() const;
	void setFightAnim();
};

