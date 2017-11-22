#include "Core.h"
#include "../app.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

Core::Core()
{

}

void Core::Init()
{
	// Tworzenie okna
	mainWindow.create(sf::VideoMode(1366, 768), "Game by Kubas");
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

	// Ustawianie wybranej tekstury
	editorChoosedTexture = 0;

	// £adowanie tekstur
	cout << "Loading textures..." << endl;
	App::LoadTextures();

	// Tworzenie RectangleShape
	editorShape = new sf::RectangleShape();
	editorShape->setSize(sf::Vector2f((float)64, (float)64));
	editorShape->setFillColor(sf::Color::White);

	// Tworzenie Sprite
	editorSprite = App::GetSpriteFromTexture(editorChoosedTexture);
	editorSprite.setScale(4, 4);
	editorSprite.setColor(sf::Color(255, 255, 255, 100));

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
			{
				using namespace rapidxml;
				xml_document<> doc;
				xml_node<>* decl = doc.allocate_node(node_declaration);
				decl->append_attribute(doc.allocate_attribute("version", "1.0"));
				decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
				doc.append_node(decl);

				xml_node<>* root = doc.allocate_node(node_element, "map");
				doc.append_node(root);

				for (int i = 0; i < App::loadedMap.size(); i++)
				{
					std::ostringstream ss;
					xml_node<>* child = doc.allocate_node(node_element, "tile");
					ss << App::loadedMap[i]->textureId;

					std::cout << ss.str();
					child->append_attribute(doc.allocate_attribute("id", doc.allocate_string(ss.str().c_str())));
					child->append_attribute(doc.allocate_attribute("breakable", "false"));
					ss.str("");
					ss.clear();
					ss << App::loadedMap[i]->posX;
					child->append_attribute(doc.allocate_attribute("x", doc.allocate_string(ss.str().c_str())));
					ss.str("");
					ss.clear();
					ss << App::loadedMap[i]->posY;
					child->append_attribute(doc.allocate_attribute("y", doc.allocate_string(ss.str().c_str())));
					ss.str("");
					ss.clear();
					ss << App::loadedMap[i]->scale;
					child->append_attribute(doc.allocate_attribute("scale", doc.allocate_string(ss.str().c_str())));
					child->append_attribute(doc.allocate_attribute("collisions", "false"));
					root->append_node(child);
					
				}

				std::ofstream file_stored("storedMap.xml");
				file_stored << doc;
				file_stored.close();
				doc.clear();
				mainWindow.close();
			}
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

			// Eventy myszki
			EditorMouseEvents();

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
	editorSprite.setPosition(editorShape->getPosition());
	mainWindow.draw(editorSprite);
}

void Core::EditorMouseEvents()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		sf::Vector2i tilePosition = GetTileFromMouse();

		auto aliveTile = App::GetTextureFromPosition(tilePosition.x, tilePosition.y);
		if (aliveTile == nullptr)
		{
			Tile* newTile = new Tile();

			newTile->posX = tilePosition.x;
			newTile->posY = tilePosition.y;
			newTile->scale = 4;
			newTile->breakable = false;
			newTile->collisions = false;
			newTile->textureId = editorChoosedTexture;
			newTile->tileSprite = App::GetSpriteFromTexture(newTile->textureId);
			newTile->tileSprite.setScale(newTile->scale, newTile->scale);
			newTile->tileSprite.setPosition(sf::Vector2f(newTile->posX, newTile->posY));

			App::loadedMap.push_back(newTile);
		}
		else if (aliveTile->textureId != editorChoosedTexture)
		{
			aliveTile->textureId = editorChoosedTexture;
			aliveTile->tileSprite = App::GetSpriteFromTexture(aliveTile->textureId);
			aliveTile->tileSprite.setScale(aliveTile->scale, aliveTile->scale);
			aliveTile->tileSprite.setPosition(sf::Vector2f(aliveTile->posX, aliveTile->posY));
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		if (!editorRightButtonPressed)
		{
			editorChoosedTexture++;

			if (editorChoosedTexture + 1 > App::loadedTextures.size())
				editorChoosedTexture = 0;

			editorSprite = App::GetSpriteFromTexture(editorChoosedTexture);
			editorSprite.setScale(4, 4);
			editorSprite.setColor(sf::Color(255, 255, 255, 100));

			editorRightButtonPressed = true;
		}
	}
	else
	{
		if (editorRightButtonPressed)
			editorRightButtonPressed = false;
	}
}

sf::Vector2i Core::GetTileFromMouse()
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(mainWindow);
	sf::Vector2f localPosition = mainWindow.mapPixelToCoords(pixelPos);

	int searchedX = ((int)floor(localPosition.x / 64)) * 64;
	int searchedY = ((int)floor(localPosition.y / 64)) * 64;

	return sf::Vector2i(searchedX, searchedY);
}
