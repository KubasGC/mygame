#include "Core.h"
#include "../app.h"
#include <iostream>

Core::Core()
{

}

void Core::Init()
{
	// Tworzenie okna
	mainWindow.create(sf::VideoMode(1366, 768), "Game");
	mainWindow.setFramerateLimit(60);
	mainWindow.setVerticalSyncEnabled(true);

	// Ustawienie typu renderowanego obiektu
	renderType = RenderType::EDITOR;

	InitEditor();
}

void Core::InitGame()
{
	using namespace std;

	cout << "Game init...\n\n\n";

	// £adowanie tekstur
	cout << "Loading textures..." << endl;
	App::LoadTextures();

	// Tworzenie klasy gracza
	playerClass = new Player();
	// £adowanie mapy
	App::LoadMapFromFile("resources/maps/defaultMap.xml");

	// Wczytywanie czcionki
	mainFont.loadFromFile("resources/fonts/BarlowSemiCondensed-Light.ttf");

	// Ustawianie kamery
	mainCamera.setSize(sf::Vector2f((float)mainWindow.getSize().x, (float)mainWindow.getSize().y));
}

void Core::InitEditor()
{
	using namespace std;

	cout << "Editor init...\n\n\n";

	// Tworzenie RectangleShape
	editorShape = new sf::RectangleShape();
	editorShape->setSize(sf::Vector2f((float)64, (float)64));
	editorShape->setFillColor(sf::Color::White);

	// £adowanie tekstur
	cout << "Loading textures..." << endl;
	App::LoadTextures();

	// £adowanie mapy
	App::LoadMapFromFile("resources/maps/defaultMap.xml");
}

// G³ówna pêtla
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

		// Renderowanie gry
		if (renderType == RenderType::GAME)
		{
			// Czyszczenie okna
			mainWindow.clear(sf::Color::Black);

			// Tekst (debug) z pozycj¹
			sf::Text tempText;
			tempText.setFont(mainFont);
			char text[200];
			sprintf_s(text, "Pozycja gracza:\nX: %0.2f\nY: %0.2f", playerClass->GetPlayerShape()->getPosition().x, playerClass->GetPlayerShape()->getPosition().y);
			tempText.setString(text);
			tempText.setCharacterSize(20);
			tempText.setPosition(sf::Vector2f(playerClass->GetPlayerShape()->getPosition().x - mainWindow.getSize().x / 2, playerClass->GetPlayerShape()->getPosition().y - mainWindow.getSize().y / 2));
			mainWindow.draw(tempText);

			// Eventy klawiatury
			GameKeyboardEvents();

			// Render mapy
			GameRenderMap();

			// Render obiektów
			GameRenderEntities();

			// Ustawianie kamery
			mainWindow.setView(mainCamera);

			// Wyœwietlenie klatki
			mainWindow.display();
		}
		else if (renderType == RenderType::EDITOR)
		{
			// Czyszczenie okna
			mainWindow.clear(sf::Color::Black);

			// Render mapy
			EditorRenderMap();

			// Wyœwietlenie klatki
			mainWindow.display();
		}
	}
}

void Core::GameRenderMap()
{
	for (int i = 0; i < App::loadedMap.size(); i++)
	{
		mainWindow.draw(App::loadedMap[i]->tileSprite);
	}
}

void Core::GameRenderEntities()
{

	// Rysowanie playersprite
	mainCamera.setCenter(playerClass->GetPlayerShape()->getPosition());
	// mainWindow.draw(*(playerClass->GetPlayerShape()));
	mainWindow.draw(*(playerClass->GetPlayerSprite()));
	// Odœwie¿eni pozycji gracza (sprite)f
	playerClass->UpdatePosition();


	sf::Sprite testSprite;

}

void Core::GameKeyboardEvents()
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

void Core::EditorRenderMap()
{
	for (int i = 0; i < App::loadedMap.size(); i++)
	{
		mainWindow.draw(App::loadedMap[i]->tileSprite);
	}
	
	// Render myszki
	sf::Vector2i mouseTile = GetTileFromMouse();
	editorShape->setPosition(sf::Vector2f((float)mouseTile.x, (float)mouseTile.y));
	mainWindow.draw(*editorShape);
	std::cout << mouseTile.x << std::endl;
}

sf::Vector2i Core::GetTileFromMouse()
{
	sf::Vector2i localPosition = sf::Mouse::getPosition(mainWindow);

	int searchedX = ((int) floor(localPosition.x / 64)) * 64;
	int searchedY = ((int)floor(localPosition.y / 64)) * 64;

	return sf::Vector2i(searchedX, searchedY);
}
