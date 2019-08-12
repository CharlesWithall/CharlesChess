#pragma once

#include "Chess_Defines.h"

#include <vector>

struct Event_PieceSelected
{
	Event_PieceSelected(const Chess_RankAndFile& aRankAndFile)
		: myRankAndFile(aRankAndFile)
	{
	}

	Chess_RankAndFile myRankAndFile;
};

struct Event_EvaulatedPossibleMoves
{
	std::vector<Chess_RankAndFile> myPossibleRankAndFiles;
};

struct Event_MovePieceRequest
{
	Event_MovePieceRequest(const Chess_RankAndFile& aFromPosition, const Chess_RankAndFile& aToPosition)
		: myFromPosition(aFromPosition)
		, myToPosition(aToPosition)
	{
	}

	Chess_RankAndFile myFromPosition;
	Chess_RankAndFile myToPosition;
};