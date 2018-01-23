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
	srand(time(NULL));
	// Tworzenie okna gry
	mainWindow.create(sf::VideoMode(1366, 768), "Zombies attack", !sf::Style::Resize || sf::Style::Titlebar);
	mainWindow.setFramerateLimit(60);
	mainWindow.setVerticalSyncEnabled(false);
	score = 0;

	// system dŸwiêku todo

	// £adowanie czcionek
	std::string Files[2] =
	{
		std::string("resources/fonts/Roboto-Light.ttf"), // 0
		std::string("resources/fonts/defused.ttf") // 1
	};
	for (int file = 0; file < 2; file++)
	{
		sf::Font * tempFont = new sf::Font();
		tempFont->loadFromFile(Files[file].c_str());
		loadedFonts.push_back(tempFont);
		std::cout << "Zaladowano czcionke " << Files[file].c_str() << ".\n";
	}

	// Tworzenie fade rectangle
	fadeRectangle = new sf::RectangleShape();
	fadeRectangle->setFillColor(sf::Color(0, 0, 0, 0));

	App::LoadTextures();
	App::LoadMapFromFile("resources/maps/newMap.xml");

	gameState = 1;
	killedZombies = 0;
	isAlive = true;

	playerClass = new Player();
	renderType = RenderType::GAME;
	GenerateEnemies(4);
	mainCamera.setSize(sf::Vector2f((float)mainWindow.getSize().x, (float)mainWindow.getSize().y));
}
void Core::Loop()
{
	while (mainWindow.isOpen())
	{
		sf::Event windowEvent;

		FadeHandler(); // Handler do œciemniania ekranu
		// todo pêtla do systemu audio
		while (mainWindow.pollEvent(windowEvent))
		{
			// todo eventy
		}
		
		if (renderType == RenderType::MENU)
		{

		}
		else if (renderType == RenderType::GAME)
		{
			mainWindow.clear(sf::Color::Black);
			RenderMap();

			for (int i = 0; i < (int)App::loadedBullets.size(); i++)
			{
				int index = -1;
				bool playerHit = false;
				if (App::loadedBullets[i]->doesProjectileShouldBeRemoved(App::loadedMap, App::loadedEnemies, &index, playerClass, &playerHit))
				{
					App::DestroyBullet(App::loadedBullets[i]);
					if (index != -1)
					{
						App::loadedEnemies[index]->health -= 15.0f;
						// App::loadedEnemies[index]->entitySprite.setColor(sf::Color(255, 0, 0, 255));
						App::loadedEnemies[index]->ColorDamage();
						if (App::loadedEnemies[index]->health <= 0)
						{
							if (App::loadedEnemies[index]->enemyType == 2)
							{
								score += 9;
							}
							else
							{
								score += 5;
							}
							App::DestroyEnemy(index);
							killedZombies++;
							if (App::loadedEnemies.size() == 0)
							{
								score += 20 + (gameState * 18);
								gameState++;
								GenerateEnemies(4 * gameState);
							}
						}
					}
					if (playerHit)
					{
						playerClass->ColorDamage();
						playerClass->health -= 10.0f;
					}
					
				}
				else
				{
					App::loadedBullets[i]->Draw(&mainWindow);
				}
			}

			for (int i = 0; i < (int)App::loadedEnemies.size(); i++)
			{
				App::loadedEnemies[i]->Move(playerClass);
				App::loadedEnemies[i]->Draw(&mainWindow);
			}

			GameRenderHUD();

			if (playerClass->health <= 0 && isAlive)
			{

				isAlive = false;
				FadeOut(3000);

				/*playerClass->getEntityShape()->setPosition(sf::Vector2f(CENTER_X, CENTER_Y));
				playerClass->health = 100.0f;
				for (int i = App::loadedEnemies.size() - 1; i >= 0; i--)
				{
					App::DestroyEnemy(App::loadedEnemies[i]);
				}
				gameState = 1;
				GenerateEnemies(4);
				score = 0;*/
			}

			// mainWindow.draw(*(playerClass->getEntityShape()));
			mainWindow.draw(*(playerClass->getEntitySprite()));
			if (isAlive)
			{
				playerClass->Move(mainWindow);
			}
			
			playerClass->UpdatePosition();
			mainCamera.setCenter(playerClass->getEntityShape()->getPosition());
			mainWindow.setView(mainCamera);

			/*sf::Text tempText;
			tempText.setFont(*loadedFonts[0]);
			char text[200];
			sprintf_s(text, "Pozycja gracza:\nX: %0.2f\nY: %0.2f", playerClass->getEntityShape()->getPosition().x, playerClass->getEntityShape()->getPosition().y);
			tempText.setString(text);
			tempText.setCharacterSize(20);
			tempText.setPosition(playerClass->getEntityShape()->getPosition());
			mainWindow.draw(tempText);*/
			

			RenderFade();
			mainWindow.display();
		}
		else if (renderType == RenderType::DEAD)
		{
			mainWindow.clear(sf::Color::Black);


			RenderFade();
			mainWindow.display();
		}
	}
}
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
void Core::RenderMap()
{
	for (int i = 0; i < (int)App::loadedMap.size(); i++)
	{
		mainWindow.draw(App::loadedMap[i]->tileSprite);
	}
}
void Core::GenerateEnemies(int count)
{
	int secondTypeCount = 0;
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			int randomAngleDegrees = rand() % 360;
			float distance = rand() % 350 + 250;
			if (distance > 350)
			{
				distance = 350.0f;
			}
			float randomAngleRadians = (randomAngleDegrees * 3.14) / 180;
			float xPos = -sin(randomAngleRadians) * distance;
			float yPos = cos(randomAngleRadians) * distance;

			int enemyType = 1;
			if (gameState > 2)
			{
				if (secondTypeCount < (gameState - 2))
				{
					enemyType = 2;
				}
			}

			Enemy * randomEnemy = new Enemy(sf::Vector2f(CENTER_X + xPos, CENTER_Y + yPos), 45.0f, 1.0f, enemyType);
			if (randomEnemy->CheckCollision(App::loadedEnemies, playerClass))
			{
				delete randomEnemy;
				continue;
			}
			else
			{
				App::loadedEnemies.push_back(randomEnemy);
				if (enemyType == 2)
				{
					secondTypeCount++;
				}
				break;
			}
		}
	}
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

	float pHealth = playerClass->health;
	if (pHealth < 0.0f)
	{
		pHealth = 0.0f;
	}

	float healthBarWidth = Linear::easeNone((float)pHealth / 100.0f, 0.0f, 187.0f, 1.0f);
	health_bar.setSize(sf::Vector2f(healthBarWidth, 12.0f));

	char_bg.setPosition(sf::Vector2f(topPos.x + 15.0f, topPos.y + 10.0f));
	health_bg.setPosition(sf::Vector2f(char_bg.getPosition().x + 63.0f, char_bg.getPosition().y + 10.0f));
	health_bar.setPosition(sf::Vector2f(health_bg.getPosition().x + 15.0f, char_bg.getPosition().y + 20.0f));

	mainWindow.draw(health_bg);
	mainWindow.draw(health_bar);
	mainWindow.draw(char_bg);

	sf::Text tempText;
	tempText.setFont(*loadedFonts[0]);
	char text[300];
	sprintf_s(text, "Fala: %d\nPokonanych przeciwników: %d\nWynik: %d", gameState, killedZombies, score);
	tempText.setString(text);
	tempText.setCharacterSize(15);
	tempText.setPosition(topPos.x + 5.0f, topPos.y + 110.0f);
	mainWindow.draw(tempText);
}