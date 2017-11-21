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

	mainCamera.setSize(sf::Vector2f(mainWindow.getSize().x, mainWindow.getSize().y));
	mainCamera.setCenter(0, 0);

	mainWindow.setView(mainCamera);
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
		RenderEntities();
		mainWindow.display();

	}
}

void Core::RenderMap()
{
	for (int i = 0; i < 1; i++)
	{
		int x = 64 * i;
		sf::Texture tempTexture;
		sf::Sprite tempSprite = App::GetSpriteFromTexture(0);
		tempSprite.setPosition(sf::Vector2f(x, 0));

		mainWindow.draw(tempSprite);
	}
}

void Core::RenderEntities()
{
	// Draw player sprite
	mainWindow.draw(*(playerClass->GetPlayerSprite()));
	mainCamera.setCenter(playerClass->GetPlayerSprite()->getPosition());
	mainWindow.setView(mainCamera);
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
