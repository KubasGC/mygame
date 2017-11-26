#include "Core.h"
#include "../app.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../Easing/Quad.h"
#include "../Easing/Linear.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

Core::Core()
{

}

void Core::Init()
{
	// Tworzenie okna
	mainWindow.create(sf::VideoMode(1366, 768), "Game by Kubas", !sf::Style::Resize | sf::Style::Close | sf::Style::Titlebar);
	mainWindow.setFramerateLimit(60);
	mainWindow.setVerticalSyncEnabled(true);

	// £adowanie systemu dŸwiêku
	MusicSystem::Init();

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

	// Tworzenie fade rectangle
	fadeRectangle = new RectangleShape();
	fadeRectangle->setFillColor(sf::Color(0, 0, 0, 0));


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

	MusicSystem::PlayMusic("resources/sounds/prologue.ogg");

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
	// W³¹czenie muzyki
	MusicSystem::PlayMusic("resources/sounds/prologue.ogg");

	introStep = 1;
	introClock.restart();
}


// G³ówna pêtla
void Core::Loop()
{
	while (mainWindow.isOpen())
	{
		// Fading - œciemnianie ekranu
		FadeHandler();

		// G³ówna pêtla dla systemu audio
		MusicSystem::MusicHandler();

		// Eventy
		sf::Event event;
		while (mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				if (renderType == RenderType::EDITOR)
				{
					using namespace rapidxml;
					xml_document<> doc;
					xml_node<>* decl = doc.allocate_node(node_declaration);
					decl->append_attribute(doc.allocate_attribute("version", "1.0"));
					decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
					doc.append_node(decl);

					xml_node<>* root = doc.allocate_node(node_element, "map");
					doc.append_node(root);

					for (int i = 0; i < (int)App::loadedMap.size(); i++)
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
						child->append_attribute(doc.allocate_attribute("collisions", App::loadedMap[i]->collisions ? "true" : "false"));
						root->append_node(child);
					}

					std::ofstream file_stored("storedMap.xml");
					file_stored << doc;
					file_stored.close();
					doc.clear();
				}
				mainWindow.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				OnKeyPressed(&event);
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
			/*sf::Text tempText;
			tempText.setFont(mainFont);
			char text[200];
			sprintf_s(text, "Pozycja gracza:\nX: %0.2f\nY: %0.2f", playerClass->getEntityShape()->getPosition().x, playerClass->getEntityShape()->getPosition().y);
			tempText.setString(text);
			tempText.setCharacterSize(20);
			tempText.setPosition(sf::Vector2f(playerClass->getEntityShape()->getPosition().x - mainWindow.getSize().x / 2, playerClass->getEntityShape()->getPosition().y - mainWindow.getSize().y / 2));
			mainWindow.draw(tempText);*/

			// Eventy klawiatury
			GameKeyboardEvents();


			// Render obiektów
			GameRenderEntities();

			// Wyœwietlanie HUDa
			GameRenderHUD();

			// Ustawianie kamery
			mainWindow.setView(mainCamera);

			// Wyœwietlanie fadingu
			RenderFade();

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

			// Render informacji
			EditorRenderInfo();

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
				float progress = (float)(elapsedTime.asMilliseconds() / 500);
				if (progress > 1)
				{

					introStep = 2;
					introClock.restart();
				}
			}
			else if (introStep == 2)
			{
				sf::Time elapsedTime = introClock.getElapsedTime();
				float progress = (float)elapsedTime.asMilliseconds() / (float)3000;

				sf::Text myText;
				myText.setFont(*loadedFonts[0]);
				myText.setString("Kubas");
				float progressingAlpha = Quad::easeInOut(progress, 0.0f, 255.0f, 1.0f);
				if (progress >= 1)
					progressingAlpha = 255.0f;
				myText.setFillColor(sf::Color(255, 255, 255, (sf::Uint8) progressingAlpha));
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
				secondText.setFillColor(sf::Color(255, 255, 255, (sf::Uint8)(220 * progressingAlpha)));
				secondText.setPosition(sf::Vector2f(myText.getGlobalBounds().left + (progressingAlpha * 100), myText.getGlobalBounds().top + 60));
				mainWindow.draw(secondText);

				if (progress > 1)
				{
					introStep = 4;
					introClock.restart();
					MusicSystem::PlayMusic("resources/sounds/no-more-magic.ogg", 3000, false);
				}
			}
			else if (introStep == 4)
			{
				sf::Time elapsedTime = introClock.getElapsedTime();
				float progress = (float)elapsedTime.asMilliseconds() / (float)3000;

				float alpha = (float)Quad::easeInOut(progress, 255.0f, -255.0f, 1.0f);

				sf::Text myText;
				myText.setFont(*loadedFonts[0]);
				myText.setString("Kubas");
				myText.setFillColor(sf::Color(255, 255, 255, (sf::Uint8)alpha));
				myText.setCharacterSize(80);
				myText.setPosition(sf::Vector2f(mainWindow.getSize().x / 2 - myText.getGlobalBounds().width / 2, mainWindow.getSize().y / 2 - myText.getGlobalBounds().height / 2));
				mainWindow.draw(myText);

				sf::Text secondText;
				secondText.setFont(*loadedFonts[0]);
				secondText.setCharacterSize(25);
				secondText.setString("prezentuje...");
				secondText.setFillColor(sf::Color(255, 255, 255, (sf::Uint8)alpha));
				secondText.setPosition(sf::Vector2f(myText.getGlobalBounds().left + 100, myText.getGlobalBounds().top + 60));
				mainWindow.draw(secondText);

				if (progress > 1)
				{
					introStep = 5;
				}
			}
			else if (introStep == 5)
			{
				SetFade(false);
				renderType = RenderType::GAME;
				FadeIn(2000);
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
	for (int i = 0; i < (int)App::loadedMap.size(); i++)
	{
		mainWindow.draw(App::loadedMap[i]->tileSprite);
	}
}

void Core::GameRenderEntities()
{

	// Rysowanie playersprite
	mainCamera.setCenter(playerClass->getEntityShape()->getPosition());
	//mainWindow.draw(*(playerClass->getEntityShape()));
	mainWindow.draw(*(playerClass->getEntitySprite()));
	// Odœwie¿enie pozycji gracza (sprite)
	playerClass->UpdatePosition();
	// Odœwie¿anie animacji uderzania
	playerClass->AnimateFight();
}

void Core::GameRenderHUD()
{
	sf::Vector2f topPos = mainWindow.mapPixelToCoords(sf::Vector2i(0, 0));

	sf::Texture health_bg_t, char_bg_t, health_bar_t;
	sf::Sprite health_bg, char_bg;
	sf::RectangleShape health_bar;

	health_bg_t.loadFromFile("resources/UI/health_bg.png");
	char_bg_t.loadFromFile("resources/UI/char_bg.png");
	health_bar_t.loadFromFile("resources/UI/health_bar.png");

	health_bg.setTexture(health_bg_t);
	char_bg.setTexture(char_bg_t);
	health_bar.setTexture(&health_bar_t);

	float healthBarWidth = Linear::easeNone((float)playerClass->getHealth() / 100.0f, 0.0f, 187.0f, 1.0f);

	health_bar.setSize(sf::Vector2f(healthBarWidth, 12.0f));



	char_bg.setPosition(sf::Vector2f(topPos.x + 15.0f, topPos.y + 10.0f));
	health_bg.setPosition(sf::Vector2f(char_bg.getPosition().x + 63.0f, char_bg.getPosition().y + 10.0f));
	health_bar.setPosition(sf::Vector2f(health_bg.getPosition().x + 15.0f, char_bg.getPosition().y + 20.0f));





	mainWindow.draw(health_bg);
	mainWindow.draw(health_bar);
	mainWindow.draw(char_bg);
}

void Core::GameKeyboardEvents()
{
	if (!playerClass->getFightAnim())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			playerClass->setDirection(0);
			playerClass->AnimateMove();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			playerClass->setDirection(1);
			playerClass->AnimateMove();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			playerClass->setDirection(2);
			playerClass->AnimateMove();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			playerClass->setDirection(3);
			playerClass->AnimateMove();
		}
	}
}

#pragma endregion

#pragma region EDITOR

void Core::EditorRenderMap()
{
	for (int i = 0; i < (int)App::loadedMap.size(); i++)
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
			newTile->collisions = editorCollision;
			newTile->textureId = editorChoosedTexture;
			newTile->tileSprite = App::GetSpriteFromTexture(newTile->textureId);
			newTile->tileSprite.setScale((float)newTile->scale, (float)newTile->scale);
			newTile->tileSprite.setPosition(sf::Vector2f((float)newTile->posX, (float)newTile->posY));

			App::loadedMap.push_back(newTile);
		}
		else if (aliveTile->textureId != editorChoosedTexture)
		{
			aliveTile->textureId = editorChoosedTexture;
			aliveTile->tileSprite = App::GetSpriteFromTexture(aliveTile->textureId);
			aliveTile->collisions = editorCollision;
			aliveTile->tileSprite.setScale((float)aliveTile->scale, (float)aliveTile->scale);
			aliveTile->tileSprite.setPosition(sf::Vector2f((float)aliveTile->posX, (float)aliveTile->posY));
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		if (!editorRightButtonPressed)
		{
			editorChoosedTexture++;

			if (editorChoosedTexture + 1 > (int)App::loadedTextures.size())
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

void Core::EditorRenderInfo()
{
	sf::Vector2f topPos = mainWindow.mapPixelToCoords(sf::Vector2i(0, 0));
	sf::Text tempText;
	tempText.setFont(*loadedFonts[0]);
	char text[500];
	sprintf_s(text, "Edytor map v1.0\n\nWybrana tekstura: %d\nKolizja: %s", editorChoosedTexture, editorCollision ? "tak" : "nie");
	tempText.setString(text);
	tempText.setCharacterSize(20);
	tempText.setPosition(sf::Vector2f(topPos.x + 5.0f, topPos.y + 5.0f));
	mainWindow.draw(tempText);
}

#pragma endregion

#pragma region INTRO

#pragma endregion

#pragma region FADE
void Core::FadeHandler()
{
	if (isFading)
	{
		sf::Time elapsedTime = fadeClock.getElapsedTime();
		float progress = (float)(elapsedTime.asMilliseconds() / (float)fadeTime);

		if (fadeState) // fade in
		{
			float progressingAlpha = (float)Quad::easeIn(progress, 255.0f, -255.0f, 1.0f);
			fadeRectangle->setFillColor(sf::Color(0, 0, 0, (sf::Uint8)progressingAlpha));
		}
		else // fade out
		{
			float progressingAlpha = (float)Quad::easeIn(progress, 0.0f, 255.0f, 1.0f);
			fadeRectangle->setFillColor(sf::Color(0, 0, 0, (sf::Uint8)progressingAlpha));
		}

		if (progress >= 1)
		{
			SetFade(fadeState);
			isFading = false;
		}

	}
}

void Core::RenderFade()
{
	sf::Vector2f topPos = mainWindow.mapPixelToCoords(sf::Vector2i(0, 0));

	fadeRectangle->setSize(sf::Vector2f((float)mainWindow.getSize().x, (float)mainWindow.getSize().y));
	fadeRectangle->setPosition(topPos);
	mainWindow.draw(*fadeRectangle);
}

void Core::FadeIn(int ms)
{
	fadeState = true;
	fadeTime = ms;

	fadeClock.restart();
	isFading = true;
}

void Core::FadeOut(int ms)
{
	fadeState = false;
	fadeTime = ms;

	fadeClock.restart();
	isFading = true;
}

void Core::SetFade(bool toggle)
{
	isFading = false;
	if (toggle)
	{
		fadeRectangle->setFillColor(sf::Color(0, 0, 0, 0));
	}
	else
	{
		fadeRectangle->setFillColor(sf::Color(0, 0, 0, 255));
	}
}
#pragma endregion

void Core::OnKeyPressed(sf::Event * e)
{
	if (renderType == RenderType::GAME)
	{
		if (e->key.code == sf::Keyboard::Space)
		{
			if (!playerClass->getFightAnim())
			{
				playerClass->setFightAnim();
			}
		}
	}
	else if (renderType == RenderType::EDITOR)
	{
		if (e->key.code == sf::Keyboard::B)
		{
			editorCollision = !editorCollision;
		}
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
