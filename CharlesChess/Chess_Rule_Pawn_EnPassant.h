#pragma once

#include "Chess_Defines.h"
#include "Chess_Move.h"
#include "Chess_Rule.h"

class Chess_Rule_Pawn_EnPassant : public Chess_Rule
{
public:
	std::vector<Chess_Tile*> Evaluate(Chess_Tile* const anOriginTile, Chess_Board* const aChessBoard) const override
	{
		std::vector<Chess_Tile*> outVector;
		Chess_Piece* piece = anOriginTile->GetPiece();

		Chess_Move* lastMove = aChessBoard->GetLatestMove();
		const bool lastMoveWasPawnDoubleMove = lastMove && lastMove->myTakingPiece->GetType() == Chess_Pieces_EnumType::PAWN && std::abs(lastMove->myFromTile->GetRankAndFile().myRank - lastMove->myToTile->GetRankAndFile().myRank) == 2;
		if (lastMoveWasPawnDoubleMove)
		{
			const int offset = piece->GetColour() == Chess_Pieces_Colour::WHITE ? 1 : -1;

			if (Chess_Tile* leftTile = aChessBoard->GetRelativeTile(anOriginTile, -1, 0))
			{
				if (leftTile == lastMove->myToTile)
				{
					outVector.push_back(aChessBoard->GetRelativeTile(anOriginTile, -1, offset));
				}
			}

			if (Chess_Tile* rightTile = aChessBoard->GetRelativeTile(anOriginTile, 1, 0))
			{
				if (rightTile == lastMove->myToTile)
				{
					outVector.push_back(aChessBoard->GetRelativeTile(anOriginTile, 1, offset));
				}
			}
		}

		return outVector;
	}
};
		
