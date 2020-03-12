#include "AI_Analysis.h"

#include "Chess_Board.h"
#include "Chess_Helpers.h"
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

const float AI_Analysis::Analyse(const Chess_Board* const aChessBoard, const Chess_Pieces_Colour playerTurn) const
{
	float advantageValue = 0.f;

	auto analysePieces = [&](const Chess_Piece* piece, const Chess_Pieces_Colour colour, const Chess_Pieces_Colour notColour)
	{
		if (piece)
		{	
			if (const Chess_Tile* const tile = piece->GetTile())
			{
				const int multiplier = colour == Chess_Pieces_Colour::WHITE ? 1 : -1;
				const Chess_Pieces_EnumType piecetype = piece->GetType();
				const bool isPassedPawn = piecetype == Chess_Pieces_EnumType::PAWN && aChessBoard->GetIsPassedPawn(static_cast<const Chess_Pawn* const>(piece));
				advantageValue += piece->GetScore() * multiplier;
				advantageValue += LookUpPositionScore(piecetype, tile->GetRankAndFile(), colour, isPassedPawn) * multiplier;
				if (playerTurn == notColour && piecetype != Chess_Pieces_EnumType::PAWN && piecetype != Chess_Pieces_EnumType::KING)
					advantageValue += CalculateAssumedLost(piece, aChessBoard); // This should not use multiplier since this function returns -ve for black and +ve for white
			}
		}
	};

	for (const Chess_Piece* piece : aChessBoard->GetBlackPieces())
	{
		analysePieces(piece, Chess_Pieces_Colour::BLACK, Chess_Pieces_Colour::WHITE);
	}

	for (const Chess_Piece* piece : aChessBoard->GetWhitePieces())
	{
		analysePieces(piece, Chess_Pieces_Colour::WHITE, Chess_Pieces_Colour::BLACK);
	}

	for (const Chess_Piece* piece : aChessBoard->GetExtraBlackPieces())
	{
		analysePieces(piece, Chess_Pieces_Colour::BLACK, Chess_Pieces_Colour::WHITE);
	}

	for (const Chess_Piece* piece : aChessBoard->GetExtraWhitePieces())
	{
		analysePieces(piece, Chess_Pieces_Colour::WHITE, Chess_Pieces_Colour::BLACK);
	}

	return advantageValue;
}

const float AI_Analysis::LookUpPositionScore(const Chess_Pieces_EnumType aPieceType, const Chess_RankAndFile& aRankAndFile, const Chess_Pieces_Colour aColour, const bool anIsPassedPawn) const
{
	const PositionScoreMap* scoreMap = nullptr;

	if (anIsPassedPawn)
	{
#if DEBUG_ENABLED_AI
		if (aPieceType != Chess_Pieces_EnumType::PAWN)
		{
			DEBUG_PRINT_AI("CRITICAL ERROR: PIECE IS LABELLED AS PASSED PAWN BUT IS NOT PAWN");
		}
#endif
		
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

	const int rankIndex = aColour == Chess_Pieces_Colour::BLACK ? 7 - aRankAndFile.myRank : aRankAndFile.myRank;
	return  scoreMap ? (*scoreMap)[rankIndex][aRankAndFile.myFile] : 0.0f;
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

const float AI_Analysis::CalculateAssumedLost(const Chess_Piece* const aPiece, const Chess_Board* const aChessBoard) const
{
	std::vector<const Chess_Piece*> attackingPieces;
	std::vector<const Chess_Piece*> defendingPieces;
	const bool playerIsWhite = aPiece->GetColour() == Chess_Pieces_Colour::WHITE;
	Chess_Helpers::IsTileAccessible(aPiece->GetTile(), aChessBoard, playerIsWhite ? Chess_Pieces_Colour::BLACK : Chess_Pieces_Colour::WHITE, attackingPieces);
	Chess_Helpers::IsTileAccessible(aPiece->GetTile(), aChessBoard, aPiece->GetColour(), defendingPieces);
	auto recursiveCapture = [&](const bool isAttacking, const int scoreToBeat, const int finalScore, auto& recursiveFun) -> int
	{
		int bestScore = scoreToBeat;
		const Chess_Piece* capturingPiece = nullptr;
		
		const std::vector<const Chess_Piece*>& piecesToCheck = isAttacking ? attackingPieces : defendingPieces;
		const std::vector<const Chess_Piece*>& notPiecesToCheck = isAttacking ? defendingPieces : attackingPieces;

		for (const Chess_Piece* piece : piecesToCheck)
		{
			const int pieceScore = piece->GetScore();
			if (pieceScore < bestScore || notPiecesToCheck.size() == 0)
			{
				capturingPiece = piece;
				bestScore = pieceScore;
			}
		}

		if (capturingPiece)
		{
			const Chess_Pieces_Colour capturingPieceColour = capturingPiece->GetColour();
			const int onwardScore = capturingPieceColour == Chess_Pieces_Colour::WHITE ? finalScore + scoreToBeat : finalScore - scoreToBeat;
			return recursiveFun(!isAttacking, capturingPiece->GetScore(), onwardScore, recursiveFun);
		}

		return finalScore;
	};

	return static_cast<float>(recursiveCapture(true, aPiece->GetScore(), 0, recursiveCapture));
}
