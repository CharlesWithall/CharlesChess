#include "AI_Analysis.h"

#include "Chess_Board.h"
#include "Chess_Pawn.h"
#include "Chess_Piece.h"
#include "Chess_Tile.h"

AI_Analysis::AI_Analysis(const Chess_Pieces_Colour aMaximizingColour)
	: myMaximizingColour(aMaximizingColour)
{
	BuildPositionScoreMaps();
}


AI_Analysis::~AI_Analysis()
{
}

const float AI_Analysis::Analyse(const Chess_Board* const aChessBoard) const
{
	float advantageValue = 0.f;
	for (const Chess_Piece* const piece : aChessBoard->GetBlackPieces())
	{
		if (piece)
		{
			if (const Chess_Tile* const tile = piece->GetTile())
			{
				const Chess_Pieces_EnumType piecetype = piece->GetType();
				const bool isPassedPawn = piecetype == Chess_Pieces_EnumType::PAWN && aChessBoard->GetIsPassedPawn(static_cast<const Chess_Pawn* const>(piece));
				advantageValue -= piece->GetScore();
				advantageValue -= LookUpPositionScore(piecetype, tile->GetRankAndFile(), Chess_Pieces_Colour::BLACK, isPassedPawn);
			}
		}
	}

	for (const Chess_Piece* const piece : aChessBoard->GetWhitePieces())
	{
		if (piece)
		{
			if (const Chess_Tile* const tile = piece->GetTile())
			{
				const Chess_Pieces_EnumType piecetype = piece->GetType();
				const bool isPassedPawn = piecetype == Chess_Pieces_EnumType::PAWN && aChessBoard->GetIsPassedPawn(static_cast<const Chess_Pawn* const>(piece));
				advantageValue += piece->GetScore();
				advantageValue += LookUpPositionScore(piecetype, tile->GetRankAndFile(), Chess_Pieces_Colour::WHITE, isPassedPawn);
			}
		}
	}

	return myMaximizingColour == Chess_Pieces_Colour::BLACK ? -advantageValue : advantageValue;
}

const float AI_Analysis::AnalyseMove(const Chess_Board* const aChessBoard, const Chess_RankAndFile& aFromLocation, const Chess_RankAndFile& aToLocation) const
{
	const Chess_Piece* const fromPiece = aChessBoard->GetTile(aFromLocation) ? aChessBoard->GetTile(aFromLocation)->GetPiece() : nullptr;
	if (!fromPiece)
		return 0.0f;

	const Chess_Pieces_EnumType pieceType = fromPiece->GetType();
	const bool isPassedPawn = pieceType == Chess_Pieces_EnumType::PAWN && aChessBoard->GetIsPassedPawn(static_cast<const Chess_Pawn* const>(fromPiece));

	const Chess_Pieces_Colour fromColour = fromPiece->GetColour();
	const Chess_Piece* const toPiece = aChessBoard->GetTile(aToLocation) ? aChessBoard->GetTile(aToLocation)->GetPiece() : nullptr;
	const float takePieceScore = toPiece ? toPiece->GetScore() : 0.0f;
	const float toLocationScore = LookUpPositionScore(pieceType, aToLocation, fromPiece->GetColour(), isPassedPawn);
	const float fromLocationScore = LookUpPositionScore(pieceType, aFromLocation, fromPiece->GetColour(), isPassedPawn);
	const float finalEvaluation = takePieceScore + (toLocationScore - fromLocationScore);

	return myMaximizingColour == Chess_Pieces_Colour::BLACK ? -finalEvaluation : finalEvaluation;
}

const float AI_Analysis::LookUpPositionScore(const Chess_Pieces_EnumType aPieceType, const Chess_RankAndFile& aRankAndFile, const Chess_Pieces_Colour aColour, const bool anIsPassedPawn) const
{
	const PositionScoreMap* scoreMap = nullptr;

	if (anIsPassedPawn)
	{
		scoreMap = &myPassedPawnPositionMap;
	}
	else
	{
		switch (aPieceType)
		{
		case Chess_Pieces_EnumType::KING:
			scoreMap = &myKingPositionMap;
			break;
		case Chess_Pieces_EnumType::QUEEN:
			scoreMap = &myQueenPositionMap;
			break;
		case Chess_Pieces_EnumType::ROOK:
			scoreMap = &myRookPositionMap;
			break;
		case Chess_Pieces_EnumType::BISHOP:
			scoreMap = &myBishopPositionMap;
			break;
		case Chess_Pieces_EnumType::KNIGHT:
			scoreMap = &myKnightPositionMap;
			break;
		case Chess_Pieces_EnumType::PAWN:
			scoreMap = &myPawnPositionMap;
			break;
		default:
			break;
		}
	}

	Chess_RankAndFile rankAndFile = aRankAndFile;
	if (aColour == Chess_Pieces_Colour::BLACK)
	{
		rankAndFile.myRank = 7 - rankAndFile.myRank;
	}

	return  scoreMap ? (*scoreMap)[rankAndFile.myRank][rankAndFile.myFile] : 0.0f;
}

void AI_Analysis::BuildPositionScoreMaps()
{
	myKingPositionMap[7] = { -3.0f, -4.0f, -4.0f, -5.0f, -5.0f, -4.0f, -4.0f, -3.0f };
	myKingPositionMap[6] = { -3.0f, -4.0f, -4.0f, -5.0f, -5.0f, -4.0f, -4.0f, -3.0f };
	myKingPositionMap[5] = { -3.0f, -4.0f, -4.0f, -5.0f, -5.0f, -4.0f, -4.0f, -3.0f };
	myKingPositionMap[4] = { -3.0f, -4.0f, -4.0f, -5.0f, -5.0f, -4.0f, -4.0f, -3.0f };
	myKingPositionMap[3] = { -2.0f, -3.0f, -3.0f, -4.0f, -4.0f, -3.0f, -3.0f, -2.0f };
	myKingPositionMap[2] = { -1.0f, -2.0f, -2.0f, -2.0f, -2.0f, -2.0f, -2.0f, -1.0f };
	myKingPositionMap[1] = {  2.0f,  2.0f,  0.0f,  0.0f,  0.0f,  0.0f,  2.0f,  2.0f };
	myKingPositionMap[0] = {  2.0f,  3.0f,  1.0f,  0.0f,  0.0f,  1.0f,  3.0f,  2.0f };

	myQueenPositionMap[7] = { -2.0f, -1.0f, -1.0f, -0.5f, -0.5f, -1.0f, -1.0f, -2.0f };
	myQueenPositionMap[6] = { -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f };
	myQueenPositionMap[5] = { -1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.5f,  0.0f, -1.0f };
	myQueenPositionMap[4] = { -0.5f,  0.0f,  0.5f,  0.5f,  0.5f,  0.5f,  0.0f, -0.5f };
	myQueenPositionMap[3] = {  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.5f,  0.0f, -0.5f };
	myQueenPositionMap[2] = { -1.0f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.0f, -1.0f };
	myQueenPositionMap[1] = { -1.0f,  0.0f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f };
	myQueenPositionMap[0] = { -2.0f, -1.0f, -1.0f, -0.5f, -0.5f, -1.0f, -1.0f, -2.0f };

	myRookPositionMap[7] = {  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f };
	myRookPositionMap[6] = {  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.5f };
	myRookPositionMap[5] = { -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.5f };
	myRookPositionMap[4] = { -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.5f };
	myRookPositionMap[3] = { -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.5f };
	myRookPositionMap[2] = { -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.5f };
	myRookPositionMap[1] = { -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.5f };
	myRookPositionMap[0] = {  0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f,  0.0f };

	myBishopPositionMap[7] = { -2.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -2.0f };
	myBishopPositionMap[6] = { -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f };
	myBishopPositionMap[5] = { -1.0f,  0.0f,  0.5f,  1.0f,  1.0f,  0.5f,  0.0f, -1.0f };
	myBishopPositionMap[4] = { -1.0f,  0.5f,  0.5f,  1.0f,  1.0f,  0.5f,  0.5f, -1.0f };
	myBishopPositionMap[3] = { -1.0f,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f, -1.0f };
	myBishopPositionMap[2] = { -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f };
	myBishopPositionMap[1] = { -1.0f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  0.5f, -1.0f };
	myBishopPositionMap[0] = { -2.0f, -1.0f, -1.0f, -0.5f, -0.5f, -1.0f, -1.0f, -2.0f };

	myKnightPositionMap[7] = { -5.0f, -4.0f, -3.0f, -3.0f, -3.0f, -3.0f, -4.0f, -5.0f };
	myKnightPositionMap[6] = { -4.0f, -2.0f,  0.0f,  0.0f,  0.0f,  0.0f, -2.0f, -4.0f };
	myKnightPositionMap[5] = { -3.0f,  0.0f,  1.0f,  1.5f,  1.5f,  1.0f,  0.0f, -3.0f };
	myKnightPositionMap[4] = { -3.0f,  0.5f,  1.5f,  2.0f,  2.0f,  1.5f,  0.5f, -3.0f };
	myKnightPositionMap[3] = { -3.0f,  0.0f,  1.5f,  2.0f,  2.0f,  1.5f,  0.0f, -3.0f };
	myKnightPositionMap[2] = { -3.0f,  0.5f,  1.0f,  1.5f,  1.5f,  1.0f,  0.5f, -3.0f };
	myKnightPositionMap[1] = { -4.0f, -2.0f,  0.0f,  0.5f,  0.5f,  0.0f, -2.0f, -4.0f };
	myKnightPositionMap[0] = { -5.0f, -4.0f, -3.0f, -3.0f, -3.0f, -3.0f, -4.0f, -5.0f };

	myPawnPositionMap[7] = { 80.0f,  80.0f,  80.0f,  80.0f,  80.0f,  80.0f,  80.0f, 80.0f };
	myPawnPositionMap[6] = { 5.0f,  5.0f,  5.0f,  5.0f,  5.0f,  5.0f,  5.0f, 5.0f };
	myPawnPositionMap[5] = { 1.0f,  1.0f,  2.0f,  3.5f,  3.5f,  2.0f,  1.0f, 1.0f };
	myPawnPositionMap[4] = { 0.5f,  0.5f,  1.0f,  3.0f,  3.0f,  1.0f,  0.5f, 0.5f };
	myPawnPositionMap[3] = { 0.0f,  0.0f,  0.0f,  3.0f,  3.0f,  0.0f,  0.0f, 0.0f };
	myPawnPositionMap[2] = { 0.5f, -0.5f, -1.0f,  2.0f,  2.0f, -1.0f, -0.5f, 0.5f };
	myPawnPositionMap[1] = { 0.5f,  1.0f,  1.0f, -2.0f, -2.0f,  1.0f,  1.0f, 0.5f };
	myPawnPositionMap[0] = { 0.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.0f,  0.0f, 0.0f };

	myPassedPawnPositionMap[7] = { 80.0f,  80.0f,  80.0f,  80.0f,  80.0f,  80.0f,  80.0f, 80.0f };
	myPassedPawnPositionMap[6] = { 10.0f,  10.0f,  10.0f,  10.0f,  10.0f,  10.0f,  10.0f, 10.0f };
	myPassedPawnPositionMap[5] = { 5.0f,  5.0f,  5.0f,  5.0f,  5.0f,  5.0f,  5.0f, 5.0f };
	myPassedPawnPositionMap[4] = { 4.0f,  4.0f,  4.0f,  4.0f,  4.0f,  4.0f,  4.0f, 4.0f };
	myPassedPawnPositionMap[3] = { 3.0f,  3.0f,  3.0f,  3.0f,  3.0f,  3.0f,  3.0f, 3.0f };
	myPassedPawnPositionMap[2] = { 2.0f,  2.0f,  2.0f,  2.0f,  2.0f,  2.0f,  2.0f, 2.0f };
	myPassedPawnPositionMap[1] = { 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, 0.0f };
	myPassedPawnPositionMap[0] = { 0.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.0f,  0.0f, 0.0f };
}
