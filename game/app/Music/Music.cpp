#include "Music.h"
#include "../Easing/Quad.h"

// Inicjalizacja
bool MusicSystem::isPlaying;
bool MusicSystem::isSecondSoundShouldFade;
int MusicSystem::musicState;
sf::Music * MusicSystem::musicHandler;
sf::Clock * MusicSystem::musicFadingClock;
int MusicSystem::soundVolume;
int MusicSystem::fadingTime;
std::string MusicSystem::temporaryMusicFilename;

void MusicSystem::Init()
{
	isPlaying = false;
	musicState = MusicStates::NONE;
	musicHandler = new sf::Music();
	musicFadingClock = new sf::Clock();
	soundVolume = 50;
	fadingTime = 2000;
	isSecondSoundShouldFade = false;
	temporaryMusicFilename = std::string("");
}

void MusicSystem::MusicHandler()
{
	if (isPlaying)
	{
		if (musicState == MusicStates::FADINGFIRST)
		{
			sf::Time elapsedTime = musicFadingClock->getElapsedTime();
			float progress = (float)(elapsedTime.asMilliseconds() / (float)fadingTime);

			if (progress > 1)
			{
				musicHandler->stop();
				musicHandler->openFromFile(temporaryMusicFilename.c_str());
				musicHandler->setLoop(true);
				if (isSecondSoundShouldFade)
				{
					musicHandler->setVolume(0);
					musicHandler->play();
					musicFadingClock->restart();
					musicState = MusicStates::FADINGSECOND;
				}
				else
				{
					musicHandler->setVolume((float) soundVolume);
					musicHandler->play();
					musicState = MusicStates::PLAYING;
				}
			}
			else
			{
				float vol = (float)Quad::easeInOut(progress, (float)soundVolume, (float)-soundVolume, 1.0f);
				musicHandler->setVolume(vol);
			}
		}
		else if (musicState == MusicStates::FADINGSECOND)
		{
			sf::Time elapsedTime = musicFadingClock->getElapsedTime();
			float progress = (float)(elapsedTime.asMilliseconds() / (float)fadingTime);

			if (progress > 1)
			{
				musicState = MusicStates::PLAYING;
			}
			else
			{
				float vol = (float)Quad::easeInOut(progress, 0.0f, (float)soundVolume, 1.0f);
				musicHandler->setVolume(vol);
			}
		}
	}
}

void MusicSystem::PlayMusic(std::string fileName, bool withFade)
{
	if (musicState == MusicStates::NONE)
	{
		isPlaying = true;
		musicState = MusicStates::PLAYING;
		musicHandler->openFromFile(fileName.c_str());

		musicHandler->setVolume(soundVolume);
		musicHandler->setLoop(true);

		musicHandler->play();
	}
	else if (musicState == MusicStates::PLAYING)
	{
		musicFadingClock->restart();
		musicState = MusicStates::FADINGFIRST;
		temporaryMusicFilename = fileName;
		isSecondSoundShouldFade = withFade;
	}
	else
	{
		// TODO
	}
}
