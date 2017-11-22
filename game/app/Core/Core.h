#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "../Entities/Player.h"
class Core
{
private:
	sf::RenderWindow mainWindow;
	sf::View mainCamera;
	sf::Font mainFont;
	Player* playerClass;

	void RenderMap();
	void PlayerCollisionDetection();
	void RenderEntities();
	void KeyboardEvents();
public:
	Core();

	void Init();
	void Loop();

};

