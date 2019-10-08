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

				aChessBoard->GetLatestMove()->SetSpecialMoveType(Chess_Special_Move_Type::CASTLE);
				Event_Handler::GetInstance()->SendMovePieceRequestEvent(from, to, false, anEventSource);
			}

			if (aToTileFile - aFromTileFile == -2)
			{
				const int rookLocation = 0;
				const int rank = aToTile->GetRankAndFile().myRank;

				Chess_RankAndFile from = Chess_RankAndFile(rookLocation, rank);
				Chess_RankAndFile to = Chess_RankAndFile(aFromTileFile - 1, rank);

				aChessBoard->GetLatestMove()->SetSpecialMoveType(Chess_Special_Move_Type::CASTLE);
				Event_Handler::GetInstance()->SendMovePieceRequestEvent(from, to, false, anEventSource);
			}
		}
	}

	void Revert(Chess_Board* aChessBoard, const Chess_Move* aMove) const override
	{
		if (aMove->mySpecialMoveType == Chess_Special_Move_Type::CASTLE)
		{
			Chess_Tile* toTile = aMove->myToTile;
			Chess_Tile* fromTile = aMove->myFromTile;
			Chess_Piece* piece = toTile ? toTile->GetPiece() : nullptr;

			if (!fromTile || !piece)
				return;

			aChessBoard->MovePiece(fromTile, toTile, Event_Source::EVALUATION, false, false);
			piece->ForceResetHasMoved();

			const Chess_RankAndFile& toRankAndFile = toTile->GetRankAndFile();
			const bool isQueensideCastle = toRankAndFile.myFile < 4;
			const int aRank = toRankAndFile.myRank;
			const int aFile = isQueensideCastle ? 0 : 7;
			const int aFromFile = isQueensideCastle ? 3 : 5;

			Chess_Tile* rookToTile = aChessBoard->GetTile(Chess_RankAndFile(aFile, aRank));
			Chess_Tile* rookFromTile = aChessBoard->GetTile(Chess_RankAndFile(aFromFile, aRank));

			aChessBoard->MovePiece(rookToTile, rookFromTile, Event_Source::EVALUATION, false, false);
			rookToTile->GetPiece()->ForceResetHasMoved();
		}
	}

	const Chess_Special_Move_Type GetSpecialRuleType() const override { return Chess_Special_Move_Type::CASTLE; }
};