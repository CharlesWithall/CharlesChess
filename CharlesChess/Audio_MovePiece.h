#pragma once

#include "SFML/Audio.hpp"

class Audio_MovePiece
{
public:
	Audio_MovePiece();
	~Audio_MovePiece();

	void Play() { myMovePieceSound.play(); }

	sf::SoundBuffer mySoundBuffer;
	sf::Sound myMovePieceSound;
};

