#pragma once

#include "Chess_Defines.h"

#include <vector>

struct Event_PieceSelected
{
	Event_PieceSelected(const int aRankIndex, const int aFileIndex)
		: myRankIndex(aRankIndex)
		, myFileIndex(aFileIndex)
	{
	}

	int myRankIndex;
	int myFileIndex;
};

struct Event_EvaulatedPossibleMoves
{
	struct RankFile
	{
		RankFile(const int aRankIndex, const int aFileIndex)
			: myRankIndex(aRankIndex)
			, myFileIndex(aFileIndex)
		{
		}

		int myRankIndex;
		int myFileIndex;
	};

	std::vector<RankFile> myPossibleMoveIndices;
};