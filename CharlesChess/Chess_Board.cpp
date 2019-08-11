#include "Chess_Board.h"

// Pieces
#include "Chess_Bishop.h"
#include "Chess_King.h"
#include "Chess_Knight.h"
#include "Chess_Pawn.h"
#include "Chess_Queen.h"
#include "Chess_Rook.h"
//

#include "Chess_Defines.h"
#include "Chess_Tile.h"

#include "Event_Handler.h"

Chess_Board::Chess_Board()
{
	InitTiles();
	InitPieces();

	Event_Handler::GetInstance()->RegisterPieceSelectedListener(this);
}

Chess_Board::~Chess_Board()
{
	for (Chess_RankArray& rank : myChessTiles)
	{
		for (size_t i = 0; i < rank.size(); ++i)
		{
			if (Chess_Tile* tile = rank[i])
			{
				if (Chess_Piece* piece = tile->GetPiece())
				{
					delete piece;
				}

				delete tile;
			}
		}
	}
}

Chess_Piece* Chess_Board::GetPiece(const Chess_File aFile, const int aRank) const
{
	return GetTile(aFile, aRank)->GetPiece();
}

void Chess_Board::InitTiles()
{
	for (size_t i = 0; i < myChessTiles.size(); ++i)
	{
		Chess_RankArray& rank = myChessTiles[i];
		for (size_t j = 0; j < rank.size(); ++j)
		{
			rank[j] = new Chess_Tile(Chess_File(i + 1), j + 1);
		}
	}
}

void Chess_Board::InitPieces()
{
	for (size_t i = 0; i < myChessTiles.size(); ++i)
	{
		Chess_RankArray& rank = myChessTiles[i];
		for (size_t j = 0; j < rank.size(); ++j)
		{
			rank[j]->SetPiece(CreatePiece(Chess_File(i + 1), j + 1));
		}
	}
}

Chess_Piece* Chess_Board::CreatePiece(const Chess_File aFile, const int aRank)
{
	if (aRank == 2)	return new Chess_Pawn(Chess_Pieces_Colour::WHITE);
	if (aRank == 7)	return new Chess_Pawn(Chess_Pieces_Colour::BLACK);

	if (aRank == 1 || aRank == 8)
	{
		const Chess_Pieces_Colour pieceColour = aRank == 1 ? Chess_Pieces_Colour::WHITE : Chess_Pieces_Colour::BLACK;

		if (aFile == Chess_File::A || aFile == Chess_File::H) return new Chess_Rook(pieceColour);
		if (aFile == Chess_File::B || aFile == Chess_File::G) return new Chess_Knight(pieceColour);
		if (aFile == Chess_File::C || aFile == Chess_File::F) return new Chess_Bishop(pieceColour);
		if (aFile == Chess_File::D) return new Chess_Queen(pieceColour);
		if (aFile == Chess_File::E) return new Chess_King(pieceColour);
	}

	return nullptr;
}

Chess_Tile* Chess_Board::GetRelativeTile(const Chess_Tile* const aTile, const int xOffset, const int yOffset) const
{
	const int rankIndex = aTile->GetRank() + yOffset - 1;
	const int fileIndex = aTile->GetFile() + xOffset - 1;

	if (rankIndex >= 0 && rankIndex < myChessTiles.size() && fileIndex >= 0 && fileIndex < myChessTiles.size())
	{
		return myChessTiles[fileIndex][rankIndex];
	}

	return nullptr;
}

void Chess_Board::OnPieceSelected(const Event_PieceSelected& anEvent)
{
	const Chess_Tile* const chessTile = myChessTiles[anEvent.myFileIndex][anEvent.myRankIndex];
	if (const Chess_Piece* const chessPiece = chessTile->GetPiece())
	{
		Event_Handler::GetInstance()->SendEvaluatedPossibleMovesEvent(chessPiece->EvaluateMoves(chessTile, this));
	}
}