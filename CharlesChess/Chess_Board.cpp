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
#include "Chess_Move.h"
#include "Chess_Rule_GameOver_NoMoves.h"
#include "Chess_Rule_GameOver_NotEnoughPieces.h"
#include "Chess_Rule_GameOver_StaleMate_50Moves.h"
#include "Chess_Rule_Special_Castle.h"
#include "Chess_Rule_Special_EnPassant.h"
#include "Chess_Rule_Special_PawnPromotion.h"
#include "Chess_Tile.h"

#include "Event_Handler.h"

Chess_Board::Chess_Board()
{
	InitTiles();
	InitPieces();

	Event_Handler::GetInstance()->RegisterPieceSelectedListener(this);
	Event_Handler::GetInstance()->RegisterMovePieceRequestListener(this);
	Event_Handler::GetInstance()->RegisterReplacePieceRequestListener(this);

	mySpecialRules.push_back(new Chess_Rule_Special_Castle());
	mySpecialRules.push_back(new Chess_Rule_Special_PawnPromotion());
	mySpecialRules.push_back(new Chess_Rule_Special_EnPassant());

	myGameOverRules.push_back(new Chess_Rule_GameOver_NoMoves());
	myGameOverRules.push_back(new Chess_Rule_GameOver_NotEnoughPieces());
	myGameOverRules.push_back(new Chess_Rule_GameOver_StaleMate_50Moves());
}

Chess_Board::Chess_Board(const Chess_Board& aChessBoard)
{
	for (size_t i = 0; i < myChessTiles.size(); ++i)
	{
		Chess_RankArray& rank = myChessTiles[i];
		for (size_t j = 0; j < rank.size(); ++j)
		{
			rank[j] = new Chess_Tile(Chess_RankAndFile(i, j));
		}
	}

	for (Chess_Piece* piece : aChessBoard.myWhitePieces)
	{
		Chess_Piece* newPiece = Chess_Piece::CreatePiece(Chess_Pieces_Colour::WHITE, piece->GetType());
		GetTile(piece->GetTile()->GetRankAndFile())->SetPiece(newPiece);
		myWhitePieces.push_back(newPiece);
	}

	for (Chess_Piece* piece : aChessBoard.myBlackPieces)
	{
		Chess_Piece* newPiece = Chess_Piece::CreatePiece(Chess_Pieces_Colour::BLACK, piece->GetType());
		GetTile(piece->GetTile()->GetRankAndFile())->SetPiece(newPiece);
		myBlackPieces.push_back(newPiece);
	}

	mySpecialRules.push_back(new Chess_Rule_Special_Castle());
	//mySpecialRules.push_back(new Chess_Rule_Special_PawnPromotion());
	mySpecialRules.push_back(new Chess_Rule_Special_EnPassant());
}

Chess_Board::~Chess_Board()
{
	for (Chess_RankArray& rank : myChessTiles)
	{
		for (size_t i = 0; i < rank.size(); ++i)
		{
			if (Chess_Tile* tile = rank[i])
			{
				delete tile;
				tile = nullptr;
			}
		}
	}

	for (Chess_Piece* piece : myWhitePieces)
	{
		if (piece)
		{
			delete piece;
			piece = nullptr;
		}
	}

	for (Chess_Piece* piece : myBlackPieces)
	{
		if (piece)
		{
			delete piece;
			piece = nullptr;
		}
	}

	for (Chess_Rule_Special* rule : mySpecialRules)
	{
		if (rule)
		{
			delete rule;
			rule = nullptr;
		}
	}

	Event_Handler::GetInstance()->UnregisterPieceSelectedListener(this);
	Event_Handler::GetInstance()->UnregisterMovePieceRequestListener(this);
	Event_Handler::GetInstance()->UnregisterReplacePieceRequestListener(this);
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
			if (Chess_Piece* newPiece = CreatePiece(Chess_RankAndFile(i, j)))
			{
				rank[j]->SetPiece(newPiece);

				if (newPiece->GetColour() == Chess_Pieces_Colour::WHITE)
					myWhitePieces.push_back(newPiece);
				else
					myBlackPieces.push_back(newPiece);
			}
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
		Event_Handler::GetInstance()->SendEvaluatedPossibleMovesEvent(chessPiece->EvaluateMoves(this, Chess_Source::MOVE_REQUEST));
	}
}

void Chess_Board::OnMovePieceRequested(const Event_MovePieceRequest& anEvent)
{
	Chess_Tile* toTile = GetTile(anEvent.myToPosition);
	Chess_Tile* fromTile = GetTile(anEvent.myFromPosition);
	
	if (anEvent.myShouldEndTurn)
	{
		const Chess_Pieces_EnumType takingPieceType = fromTile ? (fromTile->GetPiece() ? fromTile->GetPiece()->GetType() : Chess_Pieces_EnumType::INVALID) : Chess_Pieces_EnumType::INVALID;
		const bool isTakingPiece = toTile && toTile->GetPiece();
		myMoveHistory.push(new Chess_Move(takingPieceType, anEvent.myToPosition, anEvent.myFromPosition, isTakingPiece, false)); //CPW: CHECK TODO

		if (takingPieceType == Chess_Pieces_EnumType::PAWN || isTakingPiece)
			myNumberOfMovesSincePawnMoveOrPieceTaken = 0;
		else
			++myNumberOfMovesSincePawnMoveOrPieceTaken;
	}

	MovePiece(toTile, fromTile, Event_Source::DEFAULT);

	for (const Chess_Rule_GameOver* const gameOverRule : myGameOverRules)
	{
		gameOverRule->Evaluate(this);
	}
}

void Chess_Board::MovePiece(Chess_Tile* const aToTile, Chess_Tile* const aFromTile, const Event_Source anEventSource)
{
	Chess_Piece* const takenPiece = aToTile->GetPiece();
	Chess_Piece* const takingPiece = aFromTile->GetPiece();
	const Chess_Pieces_EnumType takenPieceType = takenPiece ? takenPiece->GetType() : Chess_Pieces_EnumType::INVALID;

	TakePiece(takenPiece);

	aToTile->SetPiece(takingPiece);
	takingPiece->SetHasMoved();
	aFromTile->SetPiece(nullptr);

	for (Chess_Rule_Special* rule : mySpecialRules)
	{
		rule->Evaluate(aFromTile, aToTile, takenPieceType, this, anEventSource);
	}
}

void Chess_Board::TakePiece(Chess_Piece* aPiece)
{
	if (aPiece == nullptr)
		return;

	std::vector<Chess_Piece*>& pieceList = aPiece->GetColour() == Chess_Pieces_Colour::WHITE ? myWhitePieces : myBlackPieces;
	for (int i = pieceList.size() - 1; i >= 0; --i)
	{
		if (pieceList[i] == aPiece)
		{
			pieceList.erase(pieceList.begin() + i);
			const_cast<Chess_Tile*>(aPiece->GetTile())->SetPiece(nullptr);
			delete aPiece;
			aPiece = nullptr;
			return;
		}
	}
}

void Chess_Board::PlacePiece(Chess_Piece* aPiece, Chess_Tile* aTile)
{
	aTile->SetPiece(aPiece);
	std::vector<Chess_Piece*>& pieceList = aPiece->GetColour() == Chess_Pieces_Colour::WHITE ? myWhitePieces : myBlackPieces;
	pieceList.push_back(aPiece);
}

void Chess_Board::OnReplacePieceRequested(const Event_ReplacePieceRequest& anEvent)
{
	if (anEvent.myFromPieceType != Chess_Pieces_EnumType::INVALID && anEvent.myToPieceType != Chess_Pieces_EnumType::INVALID)
	{
		if (Chess_Tile* replaceTile = GetTile(anEvent.myRankAndFile))
		{
			TakePiece(replaceTile->GetPiece());

			Chess_Piece* piece = Chess_Piece::CreatePiece(anEvent.myColour, anEvent.myToPieceType);
			PlacePiece(piece, replaceTile);		
		}
	}
}

const std::vector<Chess_Move_Simple> Chess_Board::EvaluateAllPossibleMoves(const Chess_Pieces_Colour aColour) const
{
	std::vector<Chess_Move_Simple> outMoves;
	const std::vector<Chess_Piece*>& pieceList = aColour == Chess_Pieces_Colour::WHITE ? myWhitePieces : myBlackPieces;
	for (const Chess_Piece* const piece : pieceList)
	{
		const std::vector<Chess_Tile*>& possibleMoves = piece->EvaluateMoves(this, Chess_Source::GAME_OVER_EVALUATION);
		for (const Chess_Tile* const tile : possibleMoves)
		{
			outMoves.push_back(Chess_Move_Simple(piece->GetTile()->GetRankAndFile(), tile->GetRankAndFile()));
		}
	}

	return outMoves;
}