#pragma once

class Audio_GameOver;
class Audio_MovePiece;

class Audio_Model
{
public:
	static Audio_Model* GetInstance();

	void Play_GameOver();
	void Play_MovePiece();

private:
	Audio_Model();
	~Audio_Model();

	static Audio_Model* myInstance;
	Audio_GameOver* myAudioGameOver;
	Audio_MovePiece* myAudioMovePiece;
};

