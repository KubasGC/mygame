#pragma once
#include <SFML/Graphics.hpp>
#include "../Entities/Player.h"
class Core
{
private:
	sf::RenderWindow mainWindow;
	Player* playerClass;
	void RenderMap();
	void KeyboardEvents();
public:
	Core();

	void Init();
	void Loop();

};

