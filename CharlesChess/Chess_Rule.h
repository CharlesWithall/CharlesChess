#pragma once

#include <vector>

#include "Chess_Board.h"
#include "Chess_Tile.h"

class Chess_Rule
{
public:
	virtual std::vector<Chess_Tile*> Evaluate(const Chess_Tile* const anOriginTile, const Chess_Board* const aChessBoard) const = 0;
};