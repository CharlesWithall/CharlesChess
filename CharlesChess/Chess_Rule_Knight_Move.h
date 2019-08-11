#pragma once

#include "Chess_Defines.h"
#include "Chess_Rule.h"

class Chess_Rule_Knight_Move : public Chess_Rule
{
public:
	std::vector<Chess_Tile*> Evaluate(const Chess_Tile* const anOriginTile, const Chess_Board* const aChessBoard) const override
	{
		std::vector<Chess_Tile*> outVector;
		Chess_Piece* piece = anOriginTile->GetPiece();
		const Chess_Pieces_Colour colour = piece->GetColour();

		const int possibleXs[]{ 2,2,1,1,-1,-1,-2,-2 };
		const int possibleYs[]{ 1,-1,2,-2,2,-2,1,-1 };

		for (int i = 0; i < 8; ++i)
		{
			if (Chess_Tile* possibleTile = aChessBoard->GetRelativeTile(anOriginTile, possibleXs[i], possibleYs[i]))
			{
				if (!possibleTile->GetPiece() || possibleTile->GetPiece()->GetColour() != colour)
				{
					outVector.push_back(possibleTile);
				}
			}
		}

		return outVector;
	}
};