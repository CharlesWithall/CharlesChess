#pragma once

#include "Chess_Defines.h"
#include "Chess_Rule_Special.h"

#include "Event_Defines.h"
#include "Event_Handler.h"

class Chess_Rule_Special_EnPassant : public Chess_Rule_Special
{
public:
	bool Evaluate(const Chess_Tile* const aFromTile, const Chess_Tile* const aToTile, Chess_Board* const aChessBoard) const override
	{
		Chess_Piece* piece = aFromTile ? aFromTile->GetPiece() : nullptr;

		if (!piece || piece->GetType() != Chess_Pieces_EnumType::PAWN)
			return false;

		if (aToTile->GetPiece())
			return false;

		const Chess_RankAndFile& toRankAndFile = aToTile->GetRankAndFile();
		const Chess_RankAndFile& fromRankAndFile = aFromTile->GetRankAndFile();
		const int fileDifference = toRankAndFile.myFile - fromRankAndFile.myFile;

		return fileDifference != 0;
	}

	void Execute(Chess_Tile* const aFromTile, Chess_Tile* const aToTile, Chess_Board* const aChessBoard) override
	{
		// Move Friendly Pawn
		aChessBoard->MovePiece(aFromTile, aToTile);

		// Kill Enemy Pawn
		const int enemyPawnFile = aToTile->GetRankAndFile().myFile;
		const int enemyPawnRank = aFromTile->GetRankAndFile().myRank;

		Chess_Piece* takenPiece = aChessBoard->GetTile(Chess_RankAndFile(enemyPawnFile, enemyPawnRank))->GetPiece();
		aChessBoard->RemovePiece(takenPiece);

		Chess_Move* latestMove = aChessBoard->GetLatestMove();
		if (latestMove)
			latestMove->myTakenPiece = takenPiece;
	}

	void Revert(const Chess_Move& aMove, Chess_Board* const aChessBoard) override
	{
		// Move Friendly Pawn
		aChessBoard->MovePiece(aMove.myToTile, aMove.myFromTile);

		// Revive Enemy Pawn
		const int enemyPawnFile = aMove.myToTile->GetRankAndFile().myFile;
		const int enemyPawnRank = aMove.myFromTile->GetRankAndFile().myRank;

		aChessBoard->PlacePiece(aMove.myTakenPiece, aChessBoard->GetTile(Chess_RankAndFile(enemyPawnFile, enemyPawnRank)));
	}

	const Chess_Special_Move_Type GetSpecialRuleType() const override { return Chess_Special_Move_Type::ENPASSANT; }
};