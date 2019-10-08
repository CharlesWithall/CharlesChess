#include "Chess_Check_Rule.h"

#include "Chess_Board.h"
#include "Chess_Tile.h"
#include "Chess_Piece.h"

#include "Event_Defines.h"
#include "Debug_Profiler.h"
bool Chess_Check_Rule::Evaluate(const Chess_Board* const aChessBoard, const Chess_Pieces_Colour aKingColour) const
{
	Debug_Profiler::StartProfiling("Check!");
	const std::vector<Chess_Piece*>& piecesToReferenceKingFrom = aKingColour == Chess_Pieces_Colour::WHITE ? aChessBoard->GetWhitePieces() : aChessBoard->GetBlackPieces();
	
	const Chess_Tile* kingTile = piecesToReferenceKingFrom[0]->GetTile();

	if (!kingTile)
	{
		Debug_Profiler::EndProfiling();
		return true;
	}

	const bool isWhite = aKingColour == Chess_Pieces_Colour::WHITE;

	if (ReverseSearchBishopStyle(aChessBoard, kingTile, aKingColour, 1, 1, isWhite)) { return true; }
	if (ReverseSearchBishopStyle(aChessBoard, kingTile, aKingColour, 1, -1, !isWhite)) { return true; }
	if (ReverseSearchBishopStyle(aChessBoard, kingTile, aKingColour, -1, 1, isWhite)) { return true; }
	if (ReverseSearchBishopStyle(aChessBoard, kingTile, aKingColour, -1, -1, !isWhite)) { return true; }

	if (ReverseSearchRookStyle(aChessBoard, kingTile, aKingColour, 1, 0)) { return true; }
	if (ReverseSearchRookStyle(aChessBoard, kingTile, aKingColour, -1, 0)) { return true; }
	if (ReverseSearchRookStyle(aChessBoard, kingTile, aKingColour, 0, 1)) { return true; }
	if (ReverseSearchRookStyle(aChessBoard, kingTile, aKingColour, 0, -1)) { return true; }

	if (ReverseSearchKnightStyle(aChessBoard, kingTile, aKingColour)) { return true; }

	Debug_Profiler::EndProfiling();
	return false;
}

bool Chess_Check_Rule::EvaluateTheoretical(Chess_Board* const aChessBoard, const Chess_Pieces_Colour aKingColour, Chess_Tile* const aToTile, Chess_Tile* const aFromTile) const
{
	aChessBoard->MovePiece(aToTile, aFromTile, Event_Source::EVALUATION, true);
	const bool result = Evaluate(aChessBoard, aKingColour);
	aChessBoard->TakeBackLastMove(Event_Source::EVALUATION);
	return result;
}

const bool Chess_Check_Rule::ReverseSearchBishopStyle(const Chess_Board* const aChessBoard, const Chess_Tile* aKingTile, const Chess_Pieces_Colour aKingColour, const int anXOffset, const int aYOffset, const bool aPawnSearch) const
{
	if (Chess_Tile* possibleTile = aChessBoard->GetRelativeTile(aKingTile, anXOffset, aYOffset))
	{
		const Chess_Piece* piece = possibleTile->GetPiece();
		if (!piece)
		{
			return ReverseSearchBishopStyle(aChessBoard, possibleTile, aKingColour, anXOffset, aYOffset, false);
		}
		else if (piece->GetColour() != aKingColour)
		{
			const Chess_Pieces_EnumType type = piece->GetType();
			if (aPawnSearch && type == Chess_Pieces_EnumType::PAWN)
			{
				return true;
			}

			return (type == Chess_Pieces_EnumType::BISHOP || type == Chess_Pieces_EnumType::QUEEN);
		}
		else
		{
			return false;
		}
	}

	return false;
}

const bool Chess_Check_Rule::ReverseSearchRookStyle(const Chess_Board* const aChessBoard, const Chess_Tile* aKingTile, const Chess_Pieces_Colour aKingColour, const int anXOffset, const int aYOffset) const
{
	if (Chess_Tile* possibleTile = aChessBoard->GetRelativeTile(aKingTile, anXOffset, aYOffset))
	{
		const Chess_Piece* piece = possibleTile->GetPiece();
		if (!piece)
		{
			return ReverseSearchRookStyle(aChessBoard, possibleTile, aKingColour, anXOffset, aYOffset);
		}
		else if (piece->GetColour() != aKingColour)
		{
			const Chess_Pieces_EnumType type = piece->GetType();
			return (type == Chess_Pieces_EnumType::ROOK || type == Chess_Pieces_EnumType::QUEEN);
		}
		else
		{
			return false;
		}
	}
}

const bool Chess_Check_Rule::ReverseSearchKnightStyle(const Chess_Board* const aChessBoard, const Chess_Tile* aKingTile, const Chess_Pieces_Colour aKingColour) const
{
	const int possibleXs[]{ 2,2,1,1,-1,-1,-2,-2 };
	const int possibleYs[]{ 1,-1,2,-2,2,-2,1,-1 };

	for (int i = 0; i < 8; ++i)
	{
		if (Chess_Tile* possibleTile = aChessBoard->GetRelativeTile(aKingTile, possibleXs[i], possibleYs[i]))
		{
			Chess_Piece* piece = possibleTile->GetPiece();
			if (piece && piece->GetType() == Chess_Pieces_EnumType::KNIGHT && piece->GetColour() != aKingColour)
			{
				return true;
			}
		}
	}

	return false;
}