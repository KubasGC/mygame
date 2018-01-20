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

	float health;
	float heading;

	// Konstruktor
	Entity();

	// Metody
	virtual void UpdatePosition();

	bool GetEntityMovePositionAfterCollide(float startPosX, float startPosY, float * posX, float * posY, Entity * playerClass = nullptr);

	// Gettery i settery
	Sprite * getEntitySprite() { return &entitySprite; }
	Shape * getEntityShape() { return &entityShape; }

	float getMoveSpeed() { return moveSpeed; }

	float getHealth() { return health; }
	
};

