#pragma once
#include <SFML/Graphics.hpp>
class Player
{
private:
	sf::Texture playerTexture;
	sf::Sprite playerSprite;

	int direction;
	int animation;
	int movementSpeed;

	void ChangeAngle();
public:
	Player();
	~Player();

	sf::Sprite* GetPlayerSprite();
	void SetDirection(int dir);
	void AnimateMove();
};

