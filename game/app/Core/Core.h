#pragma once
#include <SFML/Graphics.hpp>
class Core
{
private:
	sf::RenderWindow mainWindow;

	void RenderMap();
public:
	Core();

	void Init();
	void Loop();

};

