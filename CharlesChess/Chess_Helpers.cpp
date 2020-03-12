#include "Chess_Helpers.h"

#include "Chess_Board.h"

static const int loc_possibleKnightXs[]{ 2,2,1,1,-1,-1,-2,-2 };
static const int loc_possibleKnightYs[]{ 1,-1,2,-2,2,-2,1,-1 };

const Chess_Piece* loc_ReverseSearchBishopStyle(const Chess_Tile * aTile, const Chess_Board * const aChessBoard, const Chess_Pieces_Colour aColourCheck, const int anXOffset, const int aYOffset, const bool aPawnSearch, const bool aKingSearch)
{
	if (Chess_Tile* possibleTile = aChessBoard->GetRelativeTile(aTile, anXOffset, aYOffset))
	{
		const Chess_Piece* piece = possibleTile->GetPiece();
		if (!piece)
		{
			return loc_ReverseSearchBishopStyle(possibleTile, aChessBoard, aColourCheck, anXOffset, aYOffset, false, false);
		}
		else if (piece->GetColour() == aColourCheck)
		{
			const Chess_Pieces_EnumType type = piece->GetType();
			if (aPawnSearch && type == Chess_Pieces_EnumType::PAWN)
			{
				return piece;
			}

			if (aKingSearch && type == Chess_Pieces_EnumType::KING)
			{
				return piece;
			}

			if (type == Chess_Pieces_EnumType::BISHOP || type == Chess_Pieces_EnumType::QUEEN)
			{
				return piece;
			}
		}
		else
		{
			return nullptr;
		}
	}

	return nullptr;
};

const Chess_Piece* loc_ReverseSearchRookStyle(const Chess_Tile * aTile, const Chess_Board * const aChessBoard, const Chess_Pieces_Colour aColourCheck, const int anXOffset, const int aYOffset, const bool aKingSearch)
{
	if (Chess_Tile* possibleTile = aChessBoard->GetRelativeTile(aTile, anXOffset, aYOffset))
	{
		const Chess_Piece* piece = possibleTile->GetPiece();
		if (!piece)
		{
			return loc_ReverseSearchRookStyle(possibleTile, aChessBoard, aColourCheck, anXOffset, aYOffset, false);
		}
		else if (piece->GetColour() == aColourCheck)
		{
			const Chess_Pieces_EnumType type = piece->GetType();
			if (type == Chess_Pieces_EnumType::ROOK || type == Chess_Pieces_EnumType::QUEEN)
			{
				return piece;
			}

			if (aKingSearch && type == Chess_Pieces_EnumType::KING)
			{
				return piece;
			}
		}
		else
		{
			return nullptr;
		}
	}

	return nullptr;
};

const Chess_Piece* loc_ReverseSearchKnightStyle(const Chess_Tile * aTile, const Chess_Board * const aChessBoard, const Chess_Pieces_Colour aColourCheck)
{
	for (int i = 0; i < 8; ++i)
	{
		if (Chess_Tile* possibleTile = aChessBoard->GetRelativeTile(aTile, loc_possibleKnightXs[i], loc_possibleKnightYs[i]))
		{
			const Chess_Piece* piece = possibleTile->GetPiece();
			if (piece && piece->GetType() == Chess_Pieces_EnumType::KNIGHT && piece->GetColour() == aColourCheck)
			{
				return piece;
			}
		}
	}

	return nullptr;
};

bool Chess_Helpers::IsTileAccessible(const Chess_Tile * aTile, const Chess_Board * const aChessBoard, const Chess_Pieces_Colour anAccessingColour)
{
	const bool isSearchColourBlack = anAccessingColour != Chess_Pieces_Colour::WHITE;

	if (loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour, 1 , 1, isSearchColourBlack, true)) { return true; }
	if (loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour, 1, -1, !isSearchColourBlack, true)) { return true; }
	if (loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour, -1, 1, isSearchColourBlack, true)) { return true; }
	if (loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour, -1, -1, !isSearchColourBlack, true)) { return true; }

	if (loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour, 1, 0, true)) { return true; }
	if (loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour, -1, 0, true)) { return true; }
	if (loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour, 0, 1, true)) { return true; }
	if (loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour, 0, -1, true)) { return true; }

	if (loc_ReverseSearchKnightStyle(aTile, aChessBoard, anAccessingColour)) { return true; }

	return false;
}

bool Chess_Helpers::IsTileAccessible(const Chess_Tile* aTile, const Chess_Board* const aChessBoard, const Chess_Pieces_Colour anAccessingColour, std::vector<const Chess_Piece*>& someAccesiblePieces)
{
	const bool isSearchColourBlack = anAccessingColour != Chess_Pieces_Colour::WHITE;

	if (const Chess_Piece* piece = loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour,  1,  1, isSearchColourBlack, true)) { someAccesiblePieces.push_back(piece); }
	if (const Chess_Piece* piece = loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour,  1, -1, !isSearchColourBlack, true)) { someAccesiblePieces.push_back(piece); }
	if (const Chess_Piece* piece = loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour, -1,  1, isSearchColourBlack, true)) { someAccesiblePieces.push_back(piece); }
	if (const Chess_Piece* piece = loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour, -1, -1, !isSearchColourBlack, true)) { someAccesiblePieces.push_back(piece); }

	if (const Chess_Piece* piece = loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour,  1,  0, true)) { someAccesiblePieces.push_back(piece); }
	if (const Chess_Piece* piece = loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour, -1,  0, true)) { someAccesiblePieces.push_back(piece); }
	if (const Chess_Piece* piece = loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour,  0,  1, true)) { someAccesiblePieces.push_back(piece); }
	if (const Chess_Piece* piece = loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour,  0, -1, true)) { someAccesiblePieces.push_back(piece); }

	if (const Chess_Piece* piece = loc_ReverseSearchKnightStyle(aTile, aChessBoard, anAccessingColour)) { someAccesiblePieces.push_back(piece); }

	return someAccesiblePieces.size() > 0;
}

const Chess_Piece* Chess_Helpers::GetAttackingBishop(const Chess_Tile* aTile, const Chess_Board* aChessBoard, const Chess_Pieces_Colour anAccessingColour)
{
	if (const Chess_Piece* piece = loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour, 1, 1, false, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::BISHOP)
			return piece;
	}

	if (const Chess_Piece* piece = loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour, 1, -1, false, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::BISHOP)
			return piece;
	}

	if (const Chess_Piece* piece = loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour, -1, 1, false, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::BISHOP)
			return piece;
	}

	if (const Chess_Piece* piece = loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour, -1, -1, false, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::BISHOP)
			return piece;
	}

	return nullptr;
}

const Chess_Piece* Chess_Helpers::GetAttackingKnight(const Chess_Tile* aTile, const Chess_Board* aChessBoard, const Chess_Pieces_Colour anAccessingColour)
{
	if (const Chess_Piece* piece = loc_ReverseSearchKnightStyle(aTile, aChessBoard, anAccessingColour))
	{ 
		return piece;
	}

	return nullptr;
}

const Chess_Piece* Chess_Helpers::GetAttackingQueen(const Chess_Tile* aTile, const Chess_Board* aChessBoard, const Chess_Pieces_Colour anAccessingColour)
{
	if (const Chess_Piece* piece = loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour, 1, 1, false, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::QUEEN)
			return piece;
	}

	if (const Chess_Piece* piece = loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour, 1, -1, false, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::QUEEN)
			return piece;
	}

	if (const Chess_Piece* piece = loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour, -1, 1, false, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::QUEEN)
			return piece;
	}

	if (const Chess_Piece* piece = loc_ReverseSearchBishopStyle(aTile, aChessBoard, anAccessingColour, -1, -1, false, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::QUEEN)
			return piece;
	}

	if (const Chess_Piece* piece = loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour, 1, 0, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::QUEEN)
			return piece;
	}

	if (const Chess_Piece* piece = loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour, -1, 0, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::QUEEN)
			return piece;
	}

	if (const Chess_Piece* piece = loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour, 0, 1, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::QUEEN)
			return piece;
	}

	if (const Chess_Piece* piece = loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour, 0, -1, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::QUEEN)
			return piece;
	}

	return nullptr;
}

const Chess_Piece* Chess_Helpers::GetAttackingRook(const Chess_Tile* aTile, const Chess_Board* aChessBoard, const Chess_Pieces_Colour anAccessingColour)
{
	if (const Chess_Piece* piece = loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour, 1, 0, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::ROOK)
			return piece;
	}

	if (const Chess_Piece* piece = loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour, -1, 0, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::ROOK)
			return piece;
	}

	if (const Chess_Piece* piece = loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour, 0, 1, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::ROOK)
			return piece;
	}

	if (const Chess_Piece* piece = loc_ReverseSearchRookStyle(aTile, aChessBoard, anAccessingColour, 0, -1, false))
	{
		if (piece->GetType() == Chess_Pieces_EnumType::ROOK)
			return piece;
	}

	return nullptr;
}