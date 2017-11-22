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
	Player * playerClass;
	int renderType;

	void InitGame();
	void InitEditor();

	// Renderowanie gry
	void GameRenderMap();
	void GameRenderEntities();
	void GameKeyboardEvents();

	// Edytor map
	sf::RectangleShape * editorShape;
	void EditorRenderMap();
	sf::Vector2i GetTileFromMouse();
public:
	Core();

	enum RenderType
	{
		INTRO,
		MENU,
		GAME,
		EDITOR
	};

	void Init();
	void Loop();

};

