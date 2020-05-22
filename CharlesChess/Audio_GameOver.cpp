#include "Audio_GameOver.h"

Audio_GameOver::Audio_GameOver()
{
	mySoundBuffer.loadFromFile("gameover.wav");
	myGameOverSound.setBuffer(mySoundBuffer);
	myGameOverSound.setLoop(false);
}

Audio_GameOver::~Audio_GameOver()
{
}
