#pragma once

#include "Chess_Defines.h"
#include "Chess_Rule.h"

class Chess_Rule_Queen_Move : public Chess_Rule
{
public:
	std::vector<Chess_Tile*> Evaluate(const Chess_Tile* const anOriginTile, const Chess_Board* const aChessBoard) const override
	{
		std::vector<Chess_Tile*> outVector;
		Chess_Piece* piece = anOriginTile->GetPiece();
		const Chess_Pieces_Colour colour = piece->GetColour();

		for (int i = -1; i <= 1; ++i)
		{
			for (int j = -1; j <= 1; ++j)
			{
				if (i != 0 || j != 0)
				{
					EvaluateTile(outVector, anOriginTile, aChessBoard, colour, i, j);
				}
			}
		}

		return outVector;
	}
private:
	void EvaluateTile(std::vector<Chess_Tile*>& somePossibleTiles, const Chess_Tile* const anOriginTile, const Chess_Board* const aChessBoard, const Chess_Pieces_Colour aColour, const int xOffset, const int yOffset) const
	{
		if (Chess_Tile* possibleTile = aChessBoard->GetRelativeTile(anOriginTile, xOffset, yOffset))
		{
			if (!possibleTile->GetPiece())
			{
				somePossibleTiles.push_back(possibleTile);
				EvaluateTile(somePossibleTiles, possibleTile, aChessBoard, aColour, xOffset, yOffset);
			}
			else if (possibleTile->GetPiece()->GetColour() != aColour)
			{
				somePossibleTiles.push_back(possibleTile);
			}
		}
	}
};