#include "app.h"

void app::Init()
{
	mainWindow.create(sf::VideoMode(1366, 768), "Game");
	mainWindow.setFramerateLimit(60);
}

void app::Loop()
{
	while (mainWindow.isOpen())
	{
#pragma region == Events ==  
		sf::Event event;
		while (mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				mainWindow.close();
		}
#pragma endregion

	}
}
