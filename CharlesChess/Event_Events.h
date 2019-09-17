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
	Event_MovePieceRequest(const Chess_RankAndFile& aFromPosition, const Chess_RankAndFile& aToPosition, const bool aShouldEndturn)
		: myFromPosition(aFromPosition)
		, myToPosition(aToPosition)
		, myShouldEndTurn(aShouldEndturn)
	{
	}

	Chess_RankAndFile myFromPosition;
	Chess_RankAndFile myToPosition;
	bool myShouldEndTurn;
};

struct Event_ReplacePieceRequest
{
	Event_ReplacePieceRequest(const Chess_RankAndFile& aRankAndFile, const Chess_Pieces_Colour aColour, const Chess_Pieces_EnumType aFromPieceType, const Chess_Pieces_EnumType aToPieceType)
		: myRankAndFile(aRankAndFile)
		, myColour(aColour)
		, myFromPieceType(aFromPieceType)
		, myToPieceType(aToPieceType)
	{
	}

	Chess_RankAndFile myRankAndFile;
	Chess_Pieces_Colour myColour;
	Chess_Pieces_EnumType myFromPieceType;
	Chess_Pieces_EnumType myToPieceType;
};

struct Event_RemovePieceRequest
{
	Event_RemovePieceRequest(const Chess_RankAndFile& aRankAndFile)
		: myRankAndFile(aRankAndFile)
	{
	}

	Chess_RankAndFile myRankAndFile;
};

struct Event_GameOver
{
	Event_GameOver(const Chess_GameOverResult aGameOverResult)
		: myGameOverResult(aGameOverResult)
	{
	}

	Chess_GameOverResult myGameOverResult;
};