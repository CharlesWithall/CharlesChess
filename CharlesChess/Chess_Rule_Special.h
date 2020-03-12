#pragma once

#include "Chess_Board.h"
#include "Chess_Piece.h"
#include "Chess_Tile.h"

class Chess_Rule_Special
{
public:
	virtual bool Evaluate(const Chess_Tile* const aFromTile, const Chess_Tile* const aToTile, Chess_Board* const aChessBoard) const = 0;
	virtual void Execute(Chess_Tile* const aFromTile, Chess_Tile* const aToTile, Chess_Board* const aChessBoard) = 0;
	virtual void Revert(const Chess_Move& aMove, Chess_Board* const aChessBoard) = 0;
	virtual const Chess_Special_Move_Type GetSpecialRuleType() const { return Chess_Special_Move_Type::NONE; }
};