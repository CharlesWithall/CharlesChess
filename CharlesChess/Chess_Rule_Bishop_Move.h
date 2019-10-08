#pragma once

#include "Chess_Defines.h"
#include "Chess_Rule.h"

class Chess_Rule_Bishop_Move : public Chess_Rule
{
public:
	std::vector<Chess_Tile*> Evaluate(Chess_Tile* const anOriginTile, Chess_Board* const aChessBoard) const override
	{
		std::vector<Chess_Tile*> outVector;
		Chess_Piece* piece = anOriginTile->GetPiece();
		const Chess_Pieces_Colour colour = piece->GetColour();

		EvaluateTile(outVector, anOriginTile, aChessBoard, colour, 1, 1);
		EvaluateTile(outVector, anOriginTile, aChessBoard, colour, -1, 1);
		EvaluateTile(outVector, anOriginTile, aChessBoard, colour, 1, -1);
		EvaluateTile(outVector, anOriginTile, aChessBoard, colour, -1, -1);

		return outVector;
	}
private:
	void EvaluateTile(std::vector<Chess_Tile*>& somePossibleTiles, const Chess_Tile* const anOriginTile, Chess_Board* const aChessBoard, const Chess_Pieces_Colour aColour, const int xOffset, const int yOffset) const
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