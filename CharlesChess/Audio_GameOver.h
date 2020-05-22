#pragma once

#include "SFML/Audio.hpp"

class Audio_GameOver
{
public:
	Audio_GameOver();
	~Audio_GameOver();

	void Play() { myGameOverSound.play(); }

	sf::SoundBuffer mySoundBuffer;
	sf::Sound myGameOverSound;
};

