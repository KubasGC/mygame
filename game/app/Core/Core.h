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

	std::vector<sf::Font *> loadedFonts;

	void InitGame();
	void InitEditor();

	// Renderowanie gry
	void GameRenderMap();
	void GameRenderEntities();
	void GameKeyboardEvents();

	// Edytor map
	sf::RectangleShape * editorShape;
	sf::RectangleShape * editorCenterShape;
	sf::Sprite editorSprite;
	int editorChoosedTexture;
	bool editorRightButtonPressed;

	void EditorRenderMap();
	void EditorMouseEvents();
	sf::Vector2i GetTileFromMouse();

	// Intro
	int introStep;
	sf::Clock introClock;

	void InitIntro();
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

