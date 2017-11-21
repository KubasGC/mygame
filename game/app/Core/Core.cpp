#include "Core.h"
#include "../app.h"
#include <iostream>

Core::Core()
{
}

void Core::Init()
{
	using namespace std;

	mainWindow.create(sf::VideoMode(1366, 768), "Game");
	mainWindow.setFramerateLimit(60);

	cout << "Loading textures..." << endl;
	App::LoadTextures();
}

void Core::Loop()
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

#pragma region == Render ==  
		mainWindow.clear(sf::Color::Black);
		RenderMap();
		mainWindow.display();
#pragma endregion
	}
}

void Core::RenderMap()
{

}