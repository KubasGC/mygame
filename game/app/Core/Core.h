#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "../Entities/Player.h"
class Core
{
private:
	sf::RenderWindow mainWindow;
	sf::View mainCamera;
	sf::Font mainFont;
	Player* playerClass;

	int renderType;

	// Renderowanie gry

	void GameRenderMap();
	void GameRenderEntities();
	void GameKeyboardEvents();
public:
	Core();

	enum RenderType
	{
		INTRO,
		MENU,
		GAME
	};

	void Init();
	void Loop();

};

