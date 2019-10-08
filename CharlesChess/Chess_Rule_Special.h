#pragma once

#include "Chess_Board.h"
#include "Chess_Piece.h"
#include "Chess_Tile.h"

class Chess_Rule_Special
{
public:
	virtual void Evaluate(const Chess_Tile* const aFromTile, const Chess_Tile* const aToTile, const Chess_Pieces_EnumType aTakenPieceType, Chess_Board* const aChessBoard, const Event_Source anEventSource) const = 0;
	virtual void Revert(Chess_Board* aChessBoard, const Chess_Move* aMove) const = 0;
	virtual const Chess_Special_Move_Type GetSpecialRuleType() const { return Chess_Special_Move_Type::NONE; }
};