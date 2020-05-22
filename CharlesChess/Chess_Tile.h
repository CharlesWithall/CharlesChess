#pragma once

#include "Chess_Defines.h"

class Chess_Piece;

class Chess_Tile
{
public:
	Chess_Tile(const Chess_RankAndFile& aRankAndFile)
		: myRankAndFile(aRankAndFile)
		, myPiece(nullptr)
	{};
	
	Chess_Piece* GetPiece() const { return myPiece; }
	void SetPiece(Chess_Piece* aPiece);

	const Chess_RankAndFile& GetRankAndFile() const { return myRankAndFile; }

private:
	Chess_RankAndFile myRankAndFile;
	Chess_Piece* myPiece;
};

