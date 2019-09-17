#pragma once

#include "Chess_Defines.h"
#include "Chess_Rule_Special.h"

#include "Event_Defines.h"
#include "Event_Handler.h"

class Chess_Rule_Special_Castle : public Chess_Rule_Special
{
public:
	void Evaluate(const Chess_Tile* const aFromTile, const Chess_Tile* const aToTile, const Chess_Pieces_EnumType /*aTakenPieceType*/, Chess_Board* const aChessBoard, const Event_Source anEventSource) const override
	{
		Chess_Piece* piece = aToTile ? aToTile->GetPiece() : nullptr;

		if (piece && piece->GetType() == Chess_Pieces_EnumType::KING)
		{
			const int aToTileFile = aToTile->GetRankAndFile().myFile;
			const int aFromTileFile = aFromTile->GetRankAndFile().myFile;
	
			if (aToTileFile - aFromTileFile == 2)
			{
				const int rookLocation = 7;
				const int rank = aToTile->GetRankAndFile().myRank;

				Chess_RankAndFile from = Chess_RankAndFile(rookLocation, rank);
				Chess_RankAndFile to = Chess_RankAndFile(aFromTileFile + 1, rank);

				Event_Handler::GetInstance()->SendMovePieceRequestEvent(from, to, false, anEventSource);
			}

			if (aToTileFile - aFromTileFile == -2)
			{
				const int rookLocation = 0;
				const int rank = aToTile->GetRankAndFile().myRank;

				Chess_RankAndFile from = Chess_RankAndFile(rookLocation, rank);
				Chess_RankAndFile to = Chess_RankAndFile(aFromTileFile - 1, rank);

				Event_Handler::GetInstance()->SendMovePieceRequestEvent(from, to, false, anEventSource);
			}
		}
	}
};