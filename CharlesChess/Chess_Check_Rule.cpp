#include "Chess_Check_Rule.h"

#include "Chess_Board.h"
#include "Chess_Helpers.h"
#include "Chess_Piece.h"
#include "Chess_Tile.h"

#include "Event_Defines.h"
#include "Debug_Profiler.h"

bool Chess_Check_Rule::Evaluate(const Chess_Board* const aChessBoard, const Chess_Pieces_Colour aKingColour) const
{
	const Chess_Pieces_Colour notKingColour = aKingColour == Chess_Pieces_Colour::WHITE ? Chess_Pieces_Colour::BLACK : Chess_Pieces_Colour::WHITE;
	const std::array<Chess_Piece*, 16>& piecesToReferenceKingFrom = aKingColour == Chess_Pieces_Colour::WHITE ? aChessBoard->GetWhitePieces() : aChessBoard->GetBlackPieces();
	
	const Chess_Tile* kingTile = piecesToReferenceKingFrom[0]->GetTile();

	if (!kingTile)
	{
		DEBUG_PRINT_CHESS("ERROR NO KING TILE FOR COLOUR %s", aKingColour == Chess_Pieces_Colour::WHITE ? "WHITE" : "BLACK");
		return true;
	}

	return Chess_Helpers::IsTileAccessible(kingTile, aChessBoard, notKingColour);
}

bool Chess_Check_Rule::EvaluateTheoretical(Chess_Board* const aChessBoard, const Chess_Pieces_Colour aKingColour, Chess_Tile* const aToTile, Chess_Tile* const aFromTile) const
{
	aChessBoard->PerformMovePieceRequest(aFromTile, aToTile, false);
	const bool result = Evaluate(aChessBoard, aKingColour);
	aChessBoard->OnTakeBackPieceRequested();
	return result;
}