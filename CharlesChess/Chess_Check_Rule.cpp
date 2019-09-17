#include "Chess_Check_Rule.h"

#include "Chess_Board.h"
#include "Chess_Tile.h"
#include "Chess_Piece.h"

#include "Event_Defines.h"
#include "Debug_Profiler.h"
bool Chess_Check_Rule::Evaluate(const Chess_Board* const aChessBoard, const Chess_Pieces_Colour aKingColour) const
{
	Debug_Profiler::StartProfiling("Check!");
	const std::vector<Chess_Piece*>& piecesToInspect = aKingColour == Chess_Pieces_Colour::WHITE ? aChessBoard->GetBlackPieces() : aChessBoard->GetWhitePieces();
	const std::vector<Chess_Piece*>& piecesToReferenceKingFrom = aKingColour == Chess_Pieces_Colour::WHITE ? aChessBoard->GetWhitePieces() : aChessBoard->GetBlackPieces();
	
	const Chess_Tile* kingTile = GetKingTile(piecesToReferenceKingFrom);

	if (!kingTile)
	{
		Debug_Profiler::EndProfiling();
		return true;
	}

	for (Chess_Piece* piece : piecesToInspect)
	{
		const std::vector<Chess_Tile*> tiles = piece->EvaluateMoves(aChessBoard, Chess_Source::CHECK_EVALUATION);
		for (Chess_Tile* tile : tiles)
		{
			if (tile == kingTile)
			{
				Debug_Profiler::EndProfiling();
				return true;
			}
		}
	}

	Debug_Profiler::EndProfiling();
	return false;
}

bool Chess_Check_Rule::EvaluateTheoretical(const Chess_Board* const aChessBoard, const Chess_Pieces_Colour aKingColour, const Chess_Tile* const aToTile, const Chess_Tile* const aFromTile) const
{
	Chess_Board evaluationChessBoard = *aChessBoard;
	evaluationChessBoard.MovePiece(evaluationChessBoard.GetTile(aToTile->GetRankAndFile()), evaluationChessBoard.GetTile(aFromTile->GetRankAndFile()), Event_Source::EVALUATION);
	return Evaluate(&evaluationChessBoard, aKingColour);
}

const Chess_Tile* Chess_Check_Rule::GetKingTile(const std::vector<Chess_Piece*>& somePieces) const
{
	for (Chess_Piece* piece : somePieces)
	{
		if (piece->GetType() == Chess_Pieces_EnumType::KING)
		{
			return piece->GetTile();
		}
	}

	return nullptr;
}

const bool Chess_Check_Rule::ReverseSearchBishopStyle(const Chess_Board* const aChessBoard, const Chess_Pieces_Colour aKingColour) const
{
	return true;
}

const bool Chess_Check_Rule::ReverseSearchRookStyle(const Chess_Board* const aChessBoard, const Chess_Pieces_Colour aKingColour) const
{
	return true;
}

const bool Chess_Check_Rule::ReverseSearchKnightStyle(const Chess_Board* const aChessBoard, const Chess_Pieces_Colour aKingColour) const
{
	return true;
}