#include "Audio_MovePiece.h"

Audio_MovePiece::Audio_MovePiece()
{
	mySoundBuffer.loadFromFile("piece_move.wav");
	myMovePieceSound.setBuffer(mySoundBuffer);
	myMovePieceSound.setLoop(false);
}

Audio_MovePiece::~Audio_MovePiece()
{
}
