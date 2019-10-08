#pragma once

#include "Chess_Defines.h"
#include "Chess_Rule.h"

class Chess_Rule_Pawn_Take : public Chess_Rule
{
public:
	std::vector<Chess_Tile*> Evaluate(Chess_Tile* const anOriginTile, Chess_Board* const aChessBoard) const override
	{
		std::vector<Chess_Tile*> outVector;
		Chess_Piece* piece = anOriginTile->GetPiece();
		const int offset = piece->GetColour() == Chess_Pieces_Colour::WHITE ? 1 : -1;
		if (Chess_Tile* forwardLeftTile = aChessBoard->GetRelativeTile(anOriginTile, -1, offset))
		{
			Chess_Piece* enemyPiece = forwardLeftTile->GetPiece();
			if (enemyPiece && enemyPiece->GetColour() != piece->GetColour())
			{
				outVector.push_back(forwardLeftTile);
			}
		}

		if (Chess_Tile* forwardRightTile = aChessBoard->GetRelativeTile(anOriginTile, 1, offset))
		{
			Chess_Piece* enemyPiece = forwardRightTile->GetPiece();
			if (enemyPiece && enemyPiece->GetColour() != piece->GetColour())
			{
				outVector.push_back(forwardRightTile);
			}
		}

		return outVector;
	}
};