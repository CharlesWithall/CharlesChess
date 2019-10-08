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

		if (!piece || piece->GetType() != Chess_Pieces_EnumType::PAWN)
			return;

		if (aTakenPieceType != Chess_Pieces_EnumType::INVALID)
			return;

		const Chess_RankAndFile& toRankAndFile = aToTile->GetRankAndFile();
		const Chess_RankAndFile& fromRankAndFile = aFromTile->GetRankAndFile();
		const int fileDifference = toRankAndFile.myFile - fromRankAndFile.myFile;

		if (fileDifference != 0)
		{
			Chess_Tile* takeTile = aChessBoard->GetRelativeTile(aFromTile, fileDifference, 0);
			aChessBoard->RemovePiece(takeTile->GetPiece());

			aChessBoard->GetLatestMove()->SetSpecialMoveType(Chess_Special_Move_Type::ENPASSANT);
			Event_Handler::GetInstance()->SendRemovePieceRequestEvent(takeTile->GetRankAndFile(), anEventSource);
		}
	}

	void Revert(Chess_Board* aChessBoard, const Chess_Move* aMove) const override
	{
		if (aMove->mySpecialMoveType == Chess_Special_Move_Type::ENPASSANT)
		{
			Chess_Tile* toTile = aMove->myToTile;
			Chess_Tile* fromTile = aMove->myFromTile;
			Chess_Piece* piece = toTile ? toTile->GetPiece() : nullptr;

			if (!fromTile || !piece)
				return;

			aChessBoard->MovePiece(fromTile, toTile, Event_Source::EVALUATION, false, false);

			const Chess_Pieces_Colour restoreColour = piece->GetColour();
			const int aRank = fromTile->GetRankAndFile().myRank;
			const int aFile = toTile->GetRankAndFile().myFile;
			
			Chess_Piece* restorePiece = Chess_Piece::CreatePiece(restoreColour, Chess_Pieces_EnumType::PAWN);
			aChessBoard->PlacePiece(restorePiece, aChessBoard->GetTile(Chess_RankAndFile(aFile, aRank)));
		}
	}

	const Chess_Special_Move_Type GetSpecialRuleType() const override { return Chess_Special_Move_Type::ENPASSANT; }
};