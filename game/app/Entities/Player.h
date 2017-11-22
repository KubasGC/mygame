#pragma once
#include <SFML/Graphics.hpp>
class Player
{
private:
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	sf::RectangleShape playerShape;

	int direction;
	int animation;
	float movementSpeed;

	void ChangeAngle();

	bool DoesPlayerHasCollision();
public:
	Player();
	~Player();

	sf::Sprite* GetPlayerSprite();
	sf::Shape* GetPlayerShape();

	void SetDirection(int dir);
	int GetDirection();
	int GetMovementSpeed() { return movementSpeed; }
	void AnimateMove();
	void UpdatePosition();
};

