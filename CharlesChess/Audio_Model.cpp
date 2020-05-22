#include "Audio_Model.h"

#include "Audio_GameOver.h"
#include "Audio_MovePiece.h"

Audio_Model* Audio_Model::myInstance = nullptr;

Audio_Model* Audio_Model::GetInstance()
{
	if (myInstance == nullptr)	myInstance = new Audio_Model();
	return myInstance;
}

Audio_Model::Audio_Model()
{
	myAudioGameOver = new Audio_GameOver();
	myAudioMovePiece = new Audio_MovePiece();
}

Audio_Model::~Audio_Model()
{
	delete myAudioGameOver;
	delete myAudioMovePiece;
}

void Audio_Model::Play_MovePiece()
{ 
	myAudioMovePiece->Play();
}

void Audio_Model::Play_GameOver()
{
	myAudioGameOver->Play();
}


