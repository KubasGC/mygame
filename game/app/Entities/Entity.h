#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class Entity
{
public:

	sf::Texture entityTexture;
	sf::Sprite entitySprite;
	sf::RectangleShape entityShape;

	int MoveType;

	float moveSpeed;
	int direction;
	int animationFrame;

	float health;

	// Konstruktor
	Entity();

	// Metody
	void AnimateMove();
	void UpdatePosition();
	void ChangeAngle();
	bool DoesEntityCollideWithObject();

	// Gettery i settery
	Sprite * getEntitySprite() { return &entitySprite; }
	Shape * getEntityShape() { return &entityShape; }

	float getMoveSpeed() { return moveSpeed; }

	int getDirection() { return direction; }
	void setDirection(int dir) { direction = dir; }

	float getHealth() { return health; }
	
};

