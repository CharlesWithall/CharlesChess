#pragma once

class Audio_MovePiece;

class Audio_Model
{
public:
	static Audio_Model* GetInstance();

	void Play_MovePiece();

private:
	Audio_Model();
	~Audio_Model();

	static Audio_Model* myInstance;
	Audio_MovePiece* myAudioMovePiece;
};

