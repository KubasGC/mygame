#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>
class Player : public Entity
{
private:
	bool isFightAnim;

	int m_fightAnim;

public:
	Player();

	void UpdatePosition();
	void AnimateFight();

	void Move(sf::RenderWindow & mainWindow);

	bool getFightAnim() const;
	void setFightAnim();
};

