#include <iostream>

#include "app/app.h"

#include <SFML/Graphics.hpp>

int main()
{
	app* mainClass = new app();

	/*
		Uruchomienie aplikacji
	*/
	mainClass->Init();
	/*
		Uruchomienie g��wnej p�tli
	*/
	mainClass->Loop();
	return 0;
}