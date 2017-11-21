#include <iostream>
#include "app/app.h"
#include <SFML/Graphics.hpp>

int main()
{
	Core* mainClass = new Core();

	/*
		Uruchomienie aplikacji
	*/
	mainClass->Init();
	/*
		Uruchomienie g³ównej pêtli
	*/
	mainClass->Loop();
	return 0;
}