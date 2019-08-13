#pragma once

#include "Chess_Defines.h"
#include "Chess_Rule.h"

class Chess_Rule_King_Castle : public Chess_Rule
{
public:
	std::vector<Chess_Tile*> Evaluate(const Chess_Tile* const anOriginTile, const Chess_Board* const aChessBoard) const override
	{
		std::vector<Chess_Tile*> outVector;
		Chess_Piece* piece = anOriginTile->GetPiece();

		if (piece->GetHasMoved())
		{
			return outVector;
		}

		EvaluateKingSideCastle(outVector, anOriginTile, aChessBoard);
		EvaluateQueenSideCastle(outVector, anOriginTile, aChessBoard);

		return outVector;
	}
private:
	void EvaluateKingSideCastle(std::vector<Chess_Tile*>& anOutMoveList, const Chess_Tile* const anOriginTile, const Chess_Board* const aChessBoard) const
	{
		const int kingSideCastleOffset = 3;
		const int kingSideSelectionOffset = 2;
		Chess_Tile* kingSideCastleTile = aChessBoard->GetRelativeTile(anOriginTile, kingSideCastleOffset, 0);
		Chess_Piece* kingSideCastle = kingSideCastleTile ? kingSideCastleTile->GetPiece() : nullptr;

		if (!kingSideCastle || kingSideCastle->GetHasMoved())
		{
			return;
		}

		for (int i = 1; i < kingSideCastleOffset; ++i)
		{
			if (Chess_Tile* tile = aChessBoard->GetRelativeTile(anOriginTile, i, 0))
			{
				if (tile->GetPiece())
				{
					return;
				}


			}
		}

		anOutMoveList.push_back(aChessBoard->GetRelativeTile(anOriginTile, kingSideSelectionOffset, 0));
	}


	void EvaluateQueenSideCastle(std::vector<Chess_Tile*>& anOutMoveList, const Chess_Tile* const anOriginTile, const Chess_Board* const aChessBoard) const
	{
		const int queenSideCastleOffset = -4;
		const int queenSideSelectionOffset = -2;
		Chess_Tile* queenSideCastleTile = aChessBoard->GetRelativeTile(anOriginTile, queenSideCastleOffset, 0);
		Chess_Piece* queenSideCastle = queenSideCastleTile ? queenSideCastleTile->GetPiece() : nullptr;

		if (!queenSideCastle || queenSideCastle->GetHasMoved())
		{
			return;
		}

		for (int i = -1; i > queenSideCastleOffset; --i)
		{
			if (Chess_Tile* tile = aChessBoard->GetRelativeTile(anOriginTile, i, 0))
			{
				if (tile->GetPiece())
				{
					return;
				}


			}
		}

		anOutMoveList.push_back(aChessBoard->GetRelativeTile(anOriginTile, queenSideSelectionOffset, 0));
	}
};