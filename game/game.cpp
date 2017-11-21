#include <iostream>
#include "app/app.h"

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