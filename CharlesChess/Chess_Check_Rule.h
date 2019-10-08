#pragma once

#include "Chess_Defines.h"

#include <vector>

class Chess_Board;
class Chess_Piece;
class Chess_Tile;

class Chess_Check_Rule
{
public:
	bool Evaluate(const Chess_Board* const aChessBoard, const Chess_Pieces_Colour aKingColour) const;
	bool EvaluateTheoretical(Chess_Board* const aChessBoard, const Chess_Pieces_Colour aKingColour, Chess_Tile* const aToTile, Chess_Tile* const aFromTile) const;
private:
	const bool ReverseSearchBishopStyle(const Chess_Board* const aChessBoard, const Chess_Tile* aKingTile, const Chess_Pieces_Colour aKingColour, const int anXOffset, const int aYOffset, const bool aPawnSearch) const;
	const bool ReverseSearchRookStyle(const Chess_Board* const aChessBoard, const Chess_Tile* aKingTile, const Chess_Pieces_Colour aKingColour, const int anXOffset, const int aYOffset) const;
	const bool ReverseSearchKnightStyle(const Chess_Board* const aChessBoard, const Chess_Tile* aKingTile, const Chess_Pieces_Colour aKingColour) const;
};