#pragma once

#include "Chess_Defines.h"
#include "Chess_Piece.h"
#include "Chess_Rule_Special.h"

#include "Event_Defines.h"
#include "Event_Handler.h"

class Chess_Rule_Special_PawnPromotion : public Chess_Rule_Special
{
public:
	void Evaluate(const Chess_Tile* const aFromTile, const Chess_Tile* const aToTile, const Chess_Pieces_EnumType /*aTakenPieceType*/, Chess_Board* const aChessBoard, const Event_Source anEventSource) const override
	{
		if (!aToTile)
			return;

		Chess_Piece* piece = aToTile->GetPiece();
		if (!piece)
			return;

		const Chess_RankAndFile& toTileRankAndFile = aToTile->GetRankAndFile();

		const bool isPawn = piece && piece->GetType() == Chess_Pieces_EnumType::PAWN;
		const int tileRank = toTileRankAndFile.myRank;
		const bool isLastRank = tileRank == 0 || tileRank == 7;

		if (isPawn && isLastRank)
		{
			if (anEventSource == Event_Source::EVALUATION)
			{
				// Assume a Queen - we ought to assume a knight as well but for evaluation let's stick to queen for now
				//aChessBoard->ReplacePiece(toTileRankAndFile, piece->GetColour(), Chess_Pieces_EnumType::QUEEN);
				// CPW: TODO: I don't work for some reason
			}
			else
			{
				aChessBoard->GetLatestMove()->SetSpecialMoveType(Chess_Special_Move_Type::PAWNPROMOTION);
				Event_Handler::GetInstance()->SendReplacePieceRequestEvent(toTileRankAndFile, piece->GetColour(), Chess_Pieces_EnumType::PAWN, Chess_Pieces_EnumType::INVALID);
			}
		}
	}

	void Revert(Chess_Board* aChessBoard, const Chess_Move* aMove) const override
	{
		if (aMove->mySpecialMoveType == Chess_Special_Move_Type::PAWNPROMOTION)
		{
			Chess_Tile* fromTile = aMove->myFromTile;
			const Chess_Piece* piece = fromTile->GetPiece();

			if (piece)
			{
				aChessBoard->MovePiece(fromTile, aMove->myToTile, Event_Source::EVALUATION, false, false);
				aChessBoard->ReplacePiece(fromTile->GetRankAndFile(), piece->GetColour(), Chess_Pieces_EnumType::PAWN);
			}
			
		}
	}

	const Chess_Special_Move_Type GetSpecialRuleType() const override { return Chess_Special_Move_Type::PAWNPROMOTION; }
};