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
	bool EvaluateTheoretical(const Chess_Board* const aChessBoard, const Chess_Pieces_Colour aKingColour, const Chess_Tile* const aToTile, const Chess_Tile* const aFromTile) const;
private:
	const Chess_Tile* GetKingTile(const std::vector<Chess_Piece*>& somePieces) const;
	const bool ReverseSearchBishopStyle(const Chess_Board* const aChessBoard, const Chess_Pieces_Colour aKingColour) const;
	const bool ReverseSearchRookStyle(const Chess_Board* const aChessBoard, const Chess_Pieces_Colour aKingColour) const;
	const bool ReverseSearchKnightStyle(const Chess_Board* const aChessBoard, const Chess_Pieces_Colour aKingColour) const;
};