#pragma once

#include "Chess_Rule_GameOver.h"

#include "Chess_Piece.h"

class Chess_Rule_GameOver_NotEnoughPieces : public Chess_Rule_GameOver
{
public:
	void Evaluate(Chess_Board* const aChessBoard) const override
	{
		const std::array<Chess_Piece*, 16>& whitePieces = aChessBoard->GetWhitePieces();
		const std::array<Chess_Piece*, 16>& blackPieces = aChessBoard->GetBlackPieces();
		const std::vector<Chess_Piece*>& extraWhitePieces = aChessBoard->GetExtraWhitePieces();
		const std::vector<Chess_Piece*>& extraBlackPieces = aChessBoard->GetExtraBlackPieces();

		const int whitePiecesCount = whitePieces.size() + extraWhitePieces.size();
		const int blackPiecesCount = blackPieces.size() + extraBlackPieces.size();

		if (whitePiecesCount <= 3 && blackPiecesCount <= 3)
		{
			int whiteKnightCount = 0;
			int blackKnightCount = 0;

			for (const Chess_Piece* const piece : whitePieces)
			{
				const Chess_Pieces_EnumType type = piece->GetType();
				switch (type)
				{
				case Chess_Pieces_EnumType::KING:
				case Chess_Pieces_EnumType::BISHOP:
					break;
				case Chess_Pieces_EnumType::KNIGHT:
					whiteKnightCount++;
					break;
				default:
					return;
				}
			}

			for (const Chess_Piece* const piece : blackPieces)
			{
				const Chess_Pieces_EnumType type = piece->GetType();
				switch (type)
				{
				case Chess_Pieces_EnumType::KING:
				case Chess_Pieces_EnumType::BISHOP:
					break;
				case Chess_Pieces_EnumType::KNIGHT:
					blackKnightCount++;
					break;
				default:
					return;
				}
			}

			if (whitePiecesCount >= 2 && blackPiecesCount >= 2) Event_Handler::GetInstance()->SendGameOverEvent(Chess_GameOverResult::STALEMATE); return;
			if (whiteKnightCount == 2 || blackKnightCount == 2) Event_Handler::GetInstance()->SendGameOverEvent(Chess_GameOverResult::STALEMATE); return;
		}
	}
};