#pragma once

#include "Chess_Defines.h"
#include "Chess_Rule_Special.h"

#include "Event_Defines.h"
#include "Event_Handler.h"

class Chess_Rule_Special_Castle : public Chess_Rule_Special
{
public:
	bool Evaluate(const Chess_Tile* const aFromTile, const Chess_Tile* const aToTile, Chess_Board* const aChessBoard) const override
	{
		Chess_Piece* piece = aFromTile ? aFromTile->GetPiece() : nullptr;

		if (piece && piece->GetType() == Chess_Pieces_EnumType::KING)
		{
			const int aToTileFile = aToTile->GetRankAndFile().myFile;
			const int aFromTileFile = aFromTile->GetRankAndFile().myFile;

			if (aToTileFile - aFromTileFile == 2 || aToTileFile - aFromTileFile == -2)
			{
				return true;
			}
		}

		return false;
	}

	void Execute(Chess_Tile* const aFromTile, Chess_Tile* const aToTile, Chess_Board* const aChessBoard) override
	{
		// Move King
		aChessBoard->MovePiece(aFromTile, aToTile);

		// Move Rook
		const int rookFileFrom = aToTile->GetRankAndFile().myFile > 4 ? 7 : 0;
		const int rookFileTo = rookFileFrom == 0 ? 3 : 5;
		const int rookRank = aToTile->GetRankAndFile().myRank;

		Chess_Tile* const rookFromTile = aChessBoard->GetTile(Chess_RankAndFile(rookFileFrom, rookRank));
		Chess_Tile* const rookToTile = aChessBoard->GetTile(Chess_RankAndFile(rookFileTo, rookRank));

		aChessBoard->MovePiece(rookFromTile, rookToTile);
	}

	void Revert(const Chess_Move& aMove, Chess_Board* const aChessBoard) override
	{
		// Move King
		aChessBoard->MovePiece(aMove.myToTile, aMove.myFromTile);
		aMove.myFromTile->GetPiece()->ResetHasMoved(aChessBoard->GetHistoryIndex());

		// Move Rook
		const int rookFileTo = aMove.myToTile->GetRankAndFile().myFile > 4 ? 7 : 0;
		const int rookFileFrom = rookFileTo == 0 ? 3 : 5;
		const int rookRank = aMove.myFromTile->GetRankAndFile().myRank;

		Chess_Tile* const rookFromTile = aChessBoard->GetTile(Chess_RankAndFile(rookFileFrom, rookRank));
		Chess_Tile* const rookToTile = aChessBoard->GetTile(Chess_RankAndFile(rookFileTo, rookRank));

		aChessBoard->MovePiece(rookFromTile, rookToTile);
		rookToTile->GetPiece()->ResetHasMoved(aChessBoard->GetHistoryIndex());
	}

	const Chess_Special_Move_Type GetSpecialRuleType() const override { return Chess_Special_Move_Type::CASTLE; }
};