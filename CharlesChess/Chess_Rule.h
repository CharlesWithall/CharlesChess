#pragma once

#include <vector>

#include "Chess_Board.h"
#include "Chess_Tile.h"

class Chess_Rule
{
public:
	virtual std::vector<Chess_Tile*> Evaluate(Chess_Tile* const anOriginTile, Chess_Board* const aChessBoard) const = 0;
	virtual bool ShouldEvaluateForCheck() const { return true; }
};