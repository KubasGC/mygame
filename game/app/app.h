#pragma once

#include <SFML/Graphics.hpp>

class app
{
private:
	sf::Window mainWindow;

public:
	void Init();
	void Loop();
};