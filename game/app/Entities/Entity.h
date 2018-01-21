#pragma once
#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;
class Entity
{
protected:
	void CheckDamageColor();
public:

	sf::Texture entityTexture;
	sf::Sprite entitySprite;
	sf::RectangleShape entityShape;

	int MoveType;
	float moveSpeed;
	float health;
	float heading;
	bool damageColored;
	int damageTime;

	// Konstruktor
	Entity();

	// Metody
	virtual void UpdatePosition();

	bool GetEntityMovePositionAfterCollide(float startPosX, float startPosY, float * posX, float * posY, Entity * playerClass = nullptr, bool * playerCollision = nullptr);
	void ColorDamage();

	// Gettery i settery
	Sprite * getEntitySprite() { return &entitySprite; }
	Shape * getEntityShape() { return &entityShape; }

	float getMoveSpeed() { return moveSpeed; }

	float getHealth() { return health; }
	
};

