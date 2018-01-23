#pragma once
#include <SFML/Graphics.hpp>
#include <time.h>

class Entity
{
protected:
	void CheckDamageColor();
public:

	sf::Texture entityTexture;
	sf::Sprite entitySprite;
	sf::RectangleShape entityShape;

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
	sf::Sprite * getEntitySprite() { return &entitySprite; }
	sf::Shape * getEntityShape() { return &entityShape; }

	float getMoveSpeed() { return moveSpeed; }

	float getHealth() { return health; }
	
};

