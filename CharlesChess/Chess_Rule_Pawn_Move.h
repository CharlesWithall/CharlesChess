#pragma once

#include "Chess_Defines.h"
#include "Chess_Rule.h"

class Chess_Rule_Pawn_Move : public Chess_Rule
{
public:
	std::vector<Chess_Tile*> Evaluate(const Chess_Tile* const anOriginTile, const Chess_Board* const aChessBoard) const override
	{
		std::vector<Chess_Tile*> outVector;
		Chess_Piece* piece = anOriginTile->GetPiece();

		const int offset = piece->GetColour() == Chess_Pieces_Colour::WHITE ? 1 : -1;
		Chess_Tile* forwardTile = aChessBoard->GetRelativeTile(anOriginTile, 0, offset);
		
		if (forwardTile && !forwardTile->GetPiece())
		{
			outVector.push_back(forwardTile);

			if (!piece->GetHasMoved())
			{
				Chess_Tile* doubleForwardTile = aChessBoard->GetRelativeTile(forwardTile, 0, offset);
				if (doubleForwardTile && !doubleForwardTile->GetPiece())
				{
					outVector.push_back(doubleForwardTile);
				}
			}
		}

		return outVector;
	}
};