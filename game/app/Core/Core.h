#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "../Entities/Player.h"
#include "../Entities/Enemy.h"
#include "../Music/Music.h"
#include <time.h>

#define CENTER_X 130
#define CENTER_Y 352

class Core
{
private:
	sf::RenderWindow mainWindow;
	sf::View mainCamera;
	sf::Font mainFont;
	int renderType;

	sf::RectangleShape * fadeRectangle;
	sf::Clock fadeClock;
	int fadeTime;
	bool isFading;
	bool fadeState; // true - fadeIn, false - fadeOut
	int gameState;

	std::vector<sf::Font *> loadedFonts;

	// Fading
	void FadeHandler();
	void RenderFade();

	void FadeIn(int ms);
	void FadeOut(int ms);
	void SetFade(bool toggle);

	void GameRenderHUD();

	// Renderowanie gry
	Player * playerClass;

	void RenderMap();

	void GenerateEnemies(int count);
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

