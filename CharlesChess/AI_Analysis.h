#pragma once

#include "Chess_Defines.h"
#include <array>

class Chess_Board;

typedef std::array<std::array<float, 8>, 8> PositionScoreMap;

class AI_Analysis
{
public:
	AI_Analysis(const Chess_Pieces_Colour aMaximizingColour);
	~AI_Analysis();

	const float Analyse(const Chess_Board* const aChessBoard) const;
	const float AnalyseMove(const Chess_Board* const aChessBoard, const Chess_RankAndFile& aFromLocation, const Chess_RankAndFile& aToLocation) const;
private:
	void BuildPositionScoreMaps();
	const float LookUpPositionScore(const Chess_Pieces_EnumType aPieceType, const Chess_RankAndFile& aRankAndFile, const Chess_Pieces_Colour aColour, const bool anIsPassedPawn) const;

	PositionScoreMap myKingPositionMap;
	PositionScoreMap myQueenPositionMap;
	PositionScoreMap myRookPositionMap;
	PositionScoreMap myBishopPositionMap;
	PositionScoreMap myKnightPositionMap;
	PositionScoreMap myPawnPositionMap;
	PositionScoreMap myPassedPawnPositionMap;

	Chess_Pieces_Colour myMaximizingColour;
};

