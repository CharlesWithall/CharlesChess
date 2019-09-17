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
		Chess_Piece* piece = aToTile ? aToTile->GetPiece() : nullptr;

		const bool isPawn = piece && piece->GetType() == Chess_Pieces_EnumType::PAWN;
		const int tileRank = aToTile ? aToTile->GetRankAndFile().myRank : -1;
		const bool isLastRank = tileRank == 0 || tileRank == 7;

		if (isPawn && isLastRank)
		{
			Event_Handler::GetInstance()->SendReplacePieceRequestEvent(aToTile->GetRankAndFile(), piece->GetColour(), Chess_Pieces_EnumType::PAWN, Chess_Pieces_EnumType::INVALID);
		}
	}
};