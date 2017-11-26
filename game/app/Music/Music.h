#pragma once
#ifndef _MUSIC_H_
#define _MUSIC_H_

#include <SFML/Audio.hpp>
#include <iostream>

class MusicSystem
{
private:
	enum MusicStates
	{
		NONE,
		PLAYING,
		FADINGFIRST,
		FADINGSECOND
	};

	static bool isPlaying;
	static int musicState;
	static int soundVolume;
	static int fadingTime;
	static bool isSecondSoundShouldFade;
	static sf::Clock * musicFadingClock;
	static sf::Music * musicHandler;
	static std::string temporaryMusicFilename;

public:
	static void Init();
	static void MusicHandler();

	static void PlayMusic(std::string fileName, bool withFade = true);
	static void StopMusic();
};

#endif // _MUSIC_H_
