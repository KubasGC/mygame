#pragma once
#include <SFML/Graphics.hpp>
#include "../Entities/Player.h"
class Core
{
private:
	sf::RenderWindow mainWindow;
	sf::View mainCamera;
	Player* playerClass;
	void RenderMap();
	void RenderEntities();
	void KeyboardEvents();
public:
	Core();

	void Init();
	void Loop();

};

