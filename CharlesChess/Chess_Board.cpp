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
#include "Chess_Rule_Special_Castle.h"
#include "Chess_Tile.h"

#include "Event_Handler.h"

Chess_Board::Chess_Board()
{
	InitTiles();
	InitPieces();

	Event_Handler::GetInstance()->RegisterPieceSelectedListener(this);
	Event_Handler::GetInstance()->RegisterMovePieceRequestListener(this);

	mySpecialRules.push_back(new Chess_Rule_Special_Castle());
	//mySpecialRules.push_back(new Chess_Rule_Special_EnPassant());
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

	for (Chess_Rule_Special* rule : mySpecialRules)
	{
		if (rule) delete rule;
	}
}

Chess_Piece* Chess_Board::GetPiece(const Chess_RankAndFile& aRankAndFile) const
{
	return GetTile(aRankAndFile)->GetPiece();
}

void Chess_Board::InitTiles()
{
	for (size_t i = 0; i < myChessTiles.size(); ++i)
	{
		Chess_RankArray& rank = myChessTiles[i];
		for (size_t j = 0; j < rank.size(); ++j)
		{
			rank[j] = new Chess_Tile(Chess_RankAndFile(i,j));
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
			rank[j]->SetPiece(CreatePiece(Chess_RankAndFile(i, j)));
		}
	}
}

Chess_Piece* Chess_Board::CreatePiece(const Chess_RankAndFile& aRankAndFile)
{
	const int rank = aRankAndFile.myRank;
	const int file = aRankAndFile.myFile;

	if (rank == 1)	return new Chess_Pawn(Chess_Pieces_Colour::WHITE);
	if (rank == 6)	return new Chess_Pawn(Chess_Pieces_Colour::BLACK);

	if (rank == 0 || rank == 7)
	{
		const Chess_Pieces_Colour pieceColour = rank == 0 ? Chess_Pieces_Colour::WHITE : Chess_Pieces_Colour::BLACK;

		if (file == 0 || file == 7) return new Chess_Rook(pieceColour);
		if (file == 1 || file == 6) return new Chess_Knight(pieceColour);
		if (file == 2 || file == 5) return new Chess_Bishop(pieceColour);
		if (file == 3) return new Chess_Queen(pieceColour);
		if (file == 4) return new Chess_King(pieceColour);
	}

	return nullptr;
}

Chess_Tile* Chess_Board::GetRelativeTile(const Chess_Tile* const aTile, const int xOffset, const int yOffset) const
{
	const Chess_RankAndFile rankAndFile = aTile->GetRankAndFile().GetOffset(xOffset, yOffset);

	if (rankAndFile.IsValid())
	{
		return GetTile(rankAndFile);
	}

	return nullptr;
}

void Chess_Board::OnPieceSelected(const Event_PieceSelected& anEvent)
{
	const Chess_Tile* const chessTile = GetTile(anEvent.myRankAndFile);
	if (const Chess_Piece* const chessPiece = chessTile->GetPiece())
	{
		Event_Handler::GetInstance()->SendEvaluatedPossibleMovesEvent(chessPiece->EvaluateMoves(chessTile, this));
	}
}

void Chess_Board::OnMovePieceRequested(const Event_MovePieceRequest& anEvent)
{
	Chess_Tile* toTile = GetTile(anEvent.myToPosition);
	Chess_Tile* fromTile = GetTile(anEvent.myFromPosition);

	toTile->SetPiece(fromTile->GetPiece());
	toTile->GetPiece()->SetHasMoved();
	fromTile->SetPiece(nullptr);

	for (Chess_Rule_Special* rule : mySpecialRules)
	{
		rule->Evaluate(fromTile, toTile, this);
	}
}