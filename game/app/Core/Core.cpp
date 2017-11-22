#include "Core.h"
#include "../app.h"
#include <iostream>

Core::Core()
{
}

void Core::Init()
{
	using namespace std;
	
	// �adowanie tekstur
	cout << "Loading textures..." << endl;
	App::LoadTextures();

	// Tworzenie klasy gracza
	playerClass = new Player();

	// �adowanie mapy
	App::LoadMapFromFile("resources/maps/defaultMap.xml");

	// Wczytywanie czcionki
	mainFont.loadFromFile("resources/fonts/BarlowSemiCondensed-Light.ttf");

	// Tworzenie okna
	mainWindow.create(sf::VideoMode(1366, 768), "Game");
	mainWindow.setFramerateLimit(60);

	// Ustawianie kamery
	mainCamera.setSize(sf::Vector2f((float) mainWindow.getSize().x, (float) mainWindow.getSize().y));
	mainCamera.setCenter(0, 0);
	mainWindow.setView(mainCamera);
}

// G��wna p�tla
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
	for (int i = 0; i < App::loadedMap.size(); i++)
	{
		sf::Sprite tempSprite = App::GetSpriteFromTexture(App::loadedMap[i]->textureId);
		tempSprite.setPosition(sf::Vector2f((float) App::loadedMap[i]->posX, (float) App::loadedMap[i]->posY));
		mainWindow.draw(tempSprite);
	}
}

void Core::RenderEntities()
{
	// Rysowanie playersprite
	mainWindow.draw(*(playerClass->GetPlayerSprite()));
	mainCamera.setCenter(playerClass->GetPlayerSprite()->getPosition());

	// Ustawianie kamery
	mainWindow.setView(mainCamera);

	// Tekst (debug) z pozycj�
	sf::Text tempText;
	tempText.setFont(mainFont);
	char text[200];
	sprintf_s(text, "Pozycja gracza:\nX: %0.2f\nY: %0.2f", playerClass->GetPlayerSprite()->getPosition().x, playerClass->GetPlayerSprite()->getPosition().y);
	tempText.setString(text);
	tempText.setCharacterSize(16);
	tempText.setPosition(playerClass->GetPlayerSprite()->getPosition().x - mainWindow.getSize().x / 2 + 10, playerClass->GetPlayerSprite()->getPosition().y - mainWindow.getSize().y / 2 + 5);
	mainWindow.draw(tempText);
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
