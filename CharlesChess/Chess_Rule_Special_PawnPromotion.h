#pragma once

#include "Chess_Defines.h"
#include "Chess_Piece.h"
#include "Chess_Rule_Special.h"

#include "Event_Defines.h"
#include "Event_Handler.h"

class Chess_Rule_Special_PawnPromotion : public Chess_Rule_Special
{
public:
	bool Evaluate(const Chess_Tile* const aFromTile, const Chess_Tile* const aToTile, Chess_Board* const aChessBoard) const override
	{
		if (!aToTile)
			return false;

		Chess_Piece* piece = aFromTile->GetPiece();
		if (!piece)
			return false;

		const Chess_RankAndFile& toTileRankAndFile = aToTile->GetRankAndFile();

		const bool isPawn = piece && piece->GetType() == Chess_Pieces_EnumType::PAWN;
		const int tileRank = toTileRankAndFile.myRank;
		const bool isLastRank = tileRank == 0 || tileRank == 7;

		return isPawn && isLastRank;
	}

	void Execute(Chess_Tile* const aFromTile, Chess_Tile* const aToTile, Chess_Board* const aChessBoard) override
	{
		// Kill Pawn
		Chess_Piece* promotedPawn = aFromTile->GetPiece();
		aChessBoard->RemovePiece(promotedPawn);
		aChessBoard->GetLatestMove()->myPromotedPawn = promotedPawn;

		// Kill Potential Enemy Piece
		aChessBoard->RemovePiece(aToTile->GetPiece());

		// Add Queen
		aChessBoard->PlacePiece(aToTile->GetRankAndFile(), promotedPawn->GetColour(), Chess_Pieces_EnumType::QUEEN);	
	}

	void Revert(const Chess_Move& aMove, Chess_Board* const aChessBoard) override
	{
		// Kill Queen
		aChessBoard->RemovePiece(aMove.myToTile->GetPiece());

		// Revive Pawn
		aChessBoard->PlacePiece(aMove.myPromotedPawn, aMove.myFromTile);

		// Revive Potential Enemy Piece
		if (aMove.myTakenPiece)
			aChessBoard->PlacePiece(aMove.myTakenPiece, aMove.myToTile);
	}

	const Chess_Special_Move_Type GetSpecialRuleType() const override { return Chess_Special_Move_Type::PAWNPROMOTION; }
};