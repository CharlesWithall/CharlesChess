#pragma once

#include "Chess_Defines.h"

class Chess_Piece;

class Chess_Tile
{
public:
	Chess_Tile(const Chess_File aFile, const int aRank) : myFile(aFile), myRank(aRank) {};

	Chess_Piece* GetPiece() const { return myPiece; }
	void SetPiece(Chess_Piece* aPiece) { myPiece = aPiece; }

	const Chess_File GetFile() const { return myFile; }
	const int GetRank() const { return myRank; }

private:
	Chess_File myFile;
	int myRank;
	Chess_Piece* myPiece;
};

