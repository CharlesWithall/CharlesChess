#pragma once

#include "Chess_Board.h"
#include "Chess_Tile.h"

class Chess_Rule_Special
{
public:
	virtual void Evaluate(const Chess_Tile* const aFromTile, const Chess_Tile* const aToTile, Chess_Board* const aChessBoard) const = 0;
};