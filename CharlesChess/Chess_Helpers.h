#pragma once

#include <vector>

#include "Chess_Defines.h"

class Chess_Board;
class Chess_Piece;
class Chess_Tile;

namespace Chess_Helpers
{
	bool IsTileAccessible(const Chess_Tile* aTile, const Chess_Board* const aChessBoard, const Chess_Pieces_Colour anAccessingColour);
	bool IsTileAccessible(const Chess_Tile* aTile, const Chess_Board* const aChessBoard, const Chess_Pieces_Colour anAccessingColour, std::vector<const Chess_Piece*>& someAccesiblePieces);

	const Chess_Piece* GetAttackingBishop(const Chess_Tile* aTile, const Chess_Board* aChessBoard, const Chess_Pieces_Colour anAccessingColour);
	const Chess_Piece* GetAttackingKnight(const Chess_Tile* aTile, const Chess_Board* aChessBoard, const Chess_Pieces_Colour anAccessingColour);
	const Chess_Piece* GetAttackingQueen(const Chess_Tile* aTile, const Chess_Board* aChessBoard, const Chess_Pieces_Colour anAccessingColour);
	const Chess_Piece* GetAttackingRook(const Chess_Tile* aTile, const Chess_Board* aChessBoard, const Chess_Pieces_Colour anAccessingColour);
};

