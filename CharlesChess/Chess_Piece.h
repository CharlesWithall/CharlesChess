#pragma once

#include "Chess_Defines.h"
#include "Chess_Rule.h"

#include <string>
#include <vector>

class Chess_Tile;

class Chess_Piece
{
public:
	Chess_Piece(Chess_Pieces_Colour aColour) : myColour(aColour), myHasMoved(false) {};

	virtual char GetConsoleIdentifier() const = 0;
	virtual std::string GetName() const = 0;
	virtual Chess_Pieces_EnumType GetType() const = 0;
	virtual int GetScore() const = 0;

	Chess_Pieces_Colour GetColour() const { return myColour; }
	bool GetHasMoved() const { return myHasMoved; }
	void SetHasMoved() { myHasMoved = true; }
	const std::vector<Chess_Tile*> EvaluateMoves(const Chess_Tile* const anOriginTile, const Chess_Board* const aChessBoard) const
	{
		std::vector<Chess_Tile*> outMoves;

		for (Chess_Rule* rule : myChessRules)
		{
			const std::vector<Chess_Tile*>& possibleMoves = rule->Evaluate(anOriginTile, aChessBoard);
			outMoves.insert(outMoves.end(), possibleMoves.begin(), possibleMoves.end());
		}

		return outMoves;
	}

protected:
	std::vector<Chess_Rule*> myChessRules;
	Chess_Pieces_Colour myColour;
	bool myHasMoved;
};

