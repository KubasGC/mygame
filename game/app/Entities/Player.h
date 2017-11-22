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
	int movementSpeed;

	bool canMoveUp;
	bool canMoveDown;
	bool canMoveLeft;
	bool canMoveRight;

	void ChangeAngle();
	void ResetAbleToMove();
public:
	Player();
	~Player();

	sf::Sprite* GetPlayerSprite();
	sf::Shape* GetPlayerShape();

	void SetDirection(int dir);
	int GetDirection();
	void AnimateMove();
	void UpdatePosition();

	void ToggleMove(int dir, bool toggle);
};

