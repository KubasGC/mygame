#include "Core.h"
#include "../app.h"
#include <iostream>

Core::Core()
{
}

void Core::Init()
{
	using namespace std;

	cout << "Loading textures..." << endl;
	App::LoadTextures();

	playerClass = new Player();

	mainWindow.create(sf::VideoMode(1366, 768), "Game");
	mainWindow.setFramerateLimit(60);
}

void Core::Loop()
{
	while (mainWindow.isOpen())
	{
		// Events
		sf::Event event;
		while (mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				mainWindow.close();
		}

		KeyboardEvents();

		// Render
		mainWindow.clear(sf::Color::Black);
		RenderMap();
		mainWindow.display();

	}
}

void Core::RenderMap()
{
	for (int i = 0; i < 5; i++)
	{
		int x = 64 * i;
		sf::Texture tempTexture;
		sf::Sprite tempSprite = App::GetSpriteFromTexture(0);
		tempSprite.setPosition(sf::Vector2f(x, 0));

		mainWindow.draw(tempSprite);
	}

	// Draw player sprite
	mainWindow.draw(*(playerClass->GetPlayerSprite()));
}

void Core::KeyboardEvents()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		playerClass->SetDirection(0);
		playerClass->AnimateMove();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		playerClass->SetDirection(1);
		playerClass->AnimateMove();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		playerClass->SetDirection(2);
		playerClass->AnimateMove();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		playerClass->SetDirection(3);
		playerClass->AnimateMove();
	}
}
