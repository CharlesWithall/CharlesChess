#pragma once

#include "Chess_Defines.h"
#include "Chess_Rule_Special.h"

#include "Event_Defines.h"
#include "Event_Handler.h"

class Chess_Rule_Special_EnPassant : public Chess_Rule_Special
{
public:
	void Evaluate(const Chess_Tile* const aFromTile, const Chess_Tile* const aToTile, const Chess_Pieces_EnumType aTakenPieceType, Chess_Board* const aChessBoard, const Event_Source anEventSource) const override
	{
		Chess_Piece* piece = aToTile ? aToTile->GetPiece() : nullptr;

		if (piece && piece->GetType() != Chess_Pieces_EnumType::PAWN)
			return;

		if (aTakenPieceType != Chess_Pieces_EnumType::INVALID)
			return;

		const Chess_RankAndFile& toRankAndFile = aToTile->GetRankAndFile();
		const Chess_RankAndFile& fromRankAndFile = aFromTile->GetRankAndFile();
		const int fileDifference = toRankAndFile.myFile - fromRankAndFile.myFile;

		if (fileDifference != 0)
		{
			Chess_Tile* takeTile = aChessBoard->GetRelativeTile(aFromTile, fileDifference, 0);
			aChessBoard->TakePiece(takeTile->GetPiece());
			Event_Handler::GetInstance()->SendRemovePieceRequestEvent(takeTile->GetRankAndFile(), anEventSource);
		}
	}
};