#include "Core.h"
#include "../app.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../Easing/Quad.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

Core::Core()
{

}

void Core::Init()
{
	// Tworzenie okna
	mainWindow.create(sf::VideoMode(1366, 768), "Game by Kubas", !sf::Style::Resize | sf::Style::Close | sf::Style::Titlebara);
	mainWindow.setFramerateLimit(60);
	mainWindow.setVerticalSyncEnabled(true);


	// £adowanie czcionek
	std::string Files[1] =
	{
		std::string("resources/fonts/Roboto-Light.ttf") // 0
	};
	for (int file = 0; file < 1; file++)
	{
		sf::Font * tempFont = new sf::Font();
		tempFont->loadFromFile(Files[file].c_str());
		loadedFonts.push_back(tempFont);
		std::cout << "Zaladowano czcionke " << Files[file].c_str() << ".\n";
	}


	// Ustawienie typu renderowanego obiektu
	renderType = RenderType::INTRO;

	// Wprowadzenie danych ustawieñ

	switch (renderType)
	{
		case RenderType::GAME:
			InitGame();
			break;

		case RenderType::EDITOR:
			InitEditor();
			break;

		case RenderType::INTRO:
			InitIntro();
			break;
	}
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

	// Tworzenie RectangleShape do view.
	editorCenterShape = new sf::RectangleShape();
	editorCenterShape->setSize(sf::Vector2f((float)64, (float)64));
	editorShape->setPosition(0.0f, 0.0f);

	// £adowanie mapy
	App::LoadMapFromFile("resources/maps/defaultMap.xml");

	// Tworzenie Sprite
	editorSprite = App::GetSpriteFromTexture(editorChoosedTexture);
	editorSprite.setScale(4, 4);
	editorSprite.setColor(sf::Color(255, 255, 255, 100));
	editorSprite.setPosition(sf::Vector2f(0.0f, 0.0f));

	mainCamera.setSize(sf::Vector2f((float)mainWindow.getSize().x, (float)mainWindow.getSize().y));
}

void Core::InitIntro()
{
	introStep = 1;
	introClock.restart();
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

			// Render mapy
			GameRenderMap();

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

			mainCamera.setCenter(editorCenterShape->getPosition());

			// Ustawianie kamery
			mainWindow.setView(mainCamera);

			// Wyœwietlenie klatki
			mainWindow.display();
		}
		else if (renderType == RenderType::INTRO)
		{
			// Czyszczenie okna
			mainWindow.clear(sf::Color::Black);
			if (introStep == 1)
			{
				sf::Time elapsedTime = introClock.getElapsedTime();
				float progress = elapsedTime.asMilliseconds() / 500;
				if (progress > 1)
				{
					introStep = 2;
					introClock.restart();
				}
			}
			else if (introStep == 2)
			{
				sf::Time elapsedTime = introClock.getElapsedTime();
				float progress = (float) elapsedTime.asMilliseconds() / (float) 3000;

				sf::Text myText;
				myText.setFont(*loadedFonts[0]);
				myText.setString("Kubas");
				float progressingAlpha = Quad::easeInOut(progress, 0.0f, 255.0f, 1.0f);
				if (progress >= 1)
					progressingAlpha = 255.0f;
				myText.setFillColor(sf::Color(255, 255, 255, progressingAlpha));
				myText.setCharacterSize(80);
				myText.setPosition(sf::Vector2f(mainWindow.getSize().x / 2 - myText.getGlobalBounds().width / 2, mainWindow.getSize().y / 2 - myText.getGlobalBounds().height / 2));
				mainWindow.draw(myText);

				if (progress > 1)
				{
					introStep = 3;
					introClock.restart();
				}
			}
			else if (introStep == 3)
			{
				sf::Time elapsedTime = introClock.getElapsedTime();
				float progress = (float)elapsedTime.asMilliseconds() / (float)3000;

				sf::Text myText;
				myText.setFont(*loadedFonts[0]);
				myText.setString("Kubas");
				myText.setFillColor(sf::Color(255, 255, 255, 255));
				myText.setCharacterSize(80);
				myText.setPosition(sf::Vector2f(mainWindow.getSize().x / 2 - myText.getGlobalBounds().width / 2, mainWindow.getSize().y / 2 - myText.getGlobalBounds().height / 2));
				mainWindow.draw(myText);

				float progressingAlpha = Quad::easeInOut(progress, 0.0f, 1.0f, 1.0f);

				sf::Text secondText;
				secondText.setFont(*loadedFonts[0]);
				secondText.setCharacterSize(25);
				secondText.setString("prezentuje...");
				secondText.setFillColor(sf::Color(255, 255, 255, (float)220 * (float)progressingAlpha));
				secondText.setPosition(sf::Vector2f(myText.getGlobalBounds().left + (progressingAlpha * 100), myText.getGlobalBounds().top + 60));
				mainWindow.draw(secondText);

				if (progress > 1)
				{
					introStep = 4;
					introClock.restart();
				}
			}
			else if (introStep == 4)
			{
				sf::Time elapsedTime = introClock.getElapsedTime();
				float progress = (float)elapsedTime.asMilliseconds() / (float)3000;

				float alpha = (float) Quad::easeInOut(progress, 255.0f, -255.0f, 1.0f);

				sf::Text myText;
				myText.setFont(*loadedFonts[0]);
				myText.setString("Kubas");
				myText.setFillColor(sf::Color(255, 255, 255, alpha));
				myText.setCharacterSize(80);
				myText.setPosition(sf::Vector2f(mainWindow.getSize().x / 2 - myText.getGlobalBounds().width / 2, mainWindow.getSize().y / 2 - myText.getGlobalBounds().height / 2));
				mainWindow.draw(myText);

				sf::Text secondText;
				secondText.setFont(*loadedFonts[0]);
				secondText.setCharacterSize(25);
				secondText.setString("prezentuje...");
				secondText.setFillColor(sf::Color(255, 255, 255, alpha));
				secondText.setPosition(sf::Vector2f(myText.getGlobalBounds().left + 100, myText.getGlobalBounds().top + 60));
				mainWindow.draw(secondText);

				if (progress > 1)
				{
					introStep = 5;
				}
			}
			else if (introStep == 5)
			{
				renderType = RenderType::GAME;
				InitGame();
			}
			// Wyœwietlenie klatki
			mainWindow.display();
		}
	}
}

#pragma region GAME

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

#pragma endregion

#pragma region EDITOR

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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		
		editorCenterShape->move(0.0f, -5.0f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		editorCenterShape->move(0.0f, 5.0f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		editorCenterShape->move(-5.0f, 0.0f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		editorCenterShape->move(5.0f, 0.0f);
	}

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

#pragma endregion

#pragma region INTRO

#pragma endregion

sf::Vector2i Core::GetTileFromMouse()
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(mainWindow);
	sf::Vector2f localPosition = mainWindow.mapPixelToCoords(pixelPos);

	int searchedX = ((int)floor(localPosition.x / 64)) * 64;
	int searchedY = ((int)floor(localPosition.y / 64)) * 64;

	return sf::Vector2i(searchedX, searchedY);
}
