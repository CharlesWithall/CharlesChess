#pragma once

#include "Chess_Defines.h"
#include "Chess_Rule.h"

////////////////////////////
////// Work In Progress ////
////////////////////////////

class Chess_Rule_Pawn_EnPassant : public Chess_Rule
{
public:
	std::vector<Chess_Tile*> Evaluate(const Chess_Tile* const anOriginTile, const Chess_Board* const aChessBoard) const override
	{
		std::vector<Chess_Tile*> outVector;
		//Chess_Piece* piece = anOriginTile->GetPiece();
		//
		//if (Chess_Tile* leftTile = aChessBoard->GetRelativeTile(anOriginTile, -1, 0))
		//{
		//	// Need to get last move somehow
		//}
		//
		//if (Chess_Tile* rightTile = aChessBoard->GetRelativeTile(anOriginTile, 1, 0))
		//{
		//	// Need to get last move
		//}

		////const int offset = piece->GetColour() == Chess_Pieces_Colour::WHITE ? 1 : -1;
		return outVector;
	}
};