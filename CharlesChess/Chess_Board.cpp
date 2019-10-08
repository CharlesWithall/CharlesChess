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

#pragma region Construction and Destruction
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

Chess_Board::~Chess_Board()
{
	for (Chess_RankArray& rank : myChessTiles)
		for (size_t i = 0; i < rank.size(); ++i)
			if (Chess_Tile* tile = rank[i]) { delete tile; tile = nullptr; }

	for (Chess_Piece* piece : myWhitePieces)
		if (piece) { delete piece; piece = nullptr; }

	for (Chess_Piece* piece : myBlackPieces)
		if (piece) { delete piece; piece = nullptr; }

	for (Chess_Rule_Special* rule : mySpecialRules)
		if (rule) { delete rule; rule = nullptr; }

	Event_Handler::GetInstance()->UnregisterPieceSelectedListener(this);
	Event_Handler::GetInstance()->UnregisterMovePieceRequestListener(this);
	Event_Handler::GetInstance()->UnregisterReplacePieceRequestListener(this);
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
	// Init Kings to position [0]
	const int kingX = 4;
	const int kingWhiteY = 0;
	const int kingBlackY = 7;

	myChessTiles[kingX][kingWhiteY]->SetPiece(CreatePiece(Chess_RankAndFile(kingX, kingWhiteY)));
	myChessTiles[kingX][kingBlackY]->SetPiece(CreatePiece(Chess_RankAndFile(kingX, kingBlackY)));

	myWhitePieces.push_back(myChessTiles[kingX][kingWhiteY]->GetPiece());
	myBlackPieces.push_back(myChessTiles[kingX][kingBlackY]->GetPiece());

	for (size_t i = 0; i < myChessTiles.size(); ++i)
	{
		Chess_RankArray& rank = myChessTiles[i];
		for (size_t j = 0; j < rank.size(); ++j)
		{
			if ((j == kingWhiteY || j == kingBlackY) && i == kingX)
			{
				continue;
			}

			if (Chess_Piece* newPiece = CreatePiece(Chess_RankAndFile(i, j)))
			{
				rank[j]->SetPiece(newPiece);

				if (newPiece->GetColour() == Chess_Pieces_Colour::WHITE)
					myWhitePieces.push_back(newPiece);
				else
					myBlackPieces.push_back(newPiece);

				if (newPiece->GetType() == Chess_Pieces_EnumType::PAWN)
				{
					if (newPiece->GetColour() == Chess_Pieces_Colour::WHITE)
						myWhitePawns[i] = static_cast<Chess_Pawn*>(newPiece);
					else
						myBlackPawns[i] = static_cast<Chess_Pawn*>(newPiece);
				}
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
#pragma endregion

Chess_Tile* Chess_Board::GetRelativeTile(const Chess_Tile* const aTile, const int xOffset, const int yOffset) const
{
	const Chess_RankAndFile rankAndFile = aTile->GetRankAndFile().GetOffset(xOffset, yOffset);

	if (rankAndFile.IsValid())
	{
		return GetTile(rankAndFile);
	}

	return nullptr;
}

const bool Chess_Board::GetIsPassedPawn(const Chess_Pawn* aPawn) const
{
	const int pawnIndex = aPawn->GetTile()->GetRankAndFile().myFile;
	const std::array<Chess_Pawn*, 8>& searchPawns = aPawn->GetColour() == Chess_Pieces_Colour::WHITE ? myBlackPawns : myWhitePawns;
	
	for (int i = pawnIndex - 1; i <= pawnIndex + 1; ++i)
	{
		if (i >= 0 && i < 8 && searchPawns[i] != nullptr)
		{
			return false;
		}
	}

	return true;
}

void Chess_Board::OnPieceSelected(const Event_PieceSelected& anEvent)
{
	const Chess_Tile* const chessTile = GetTile(anEvent.myRankAndFile);
	if (const Chess_Piece* const chessPiece = chessTile->GetPiece())
	{
		Event_Handler::GetInstance()->SendEvaluatedPossibleMovesEvent(chessPiece->EvaluateMoves(this, Chess_Source::MOVE_REQUEST));
	}
}

#pragma region Event Responses
void Chess_Board::OnMovePieceRequested(const Event_MovePieceRequest& anEvent)
{
	Chess_Tile* toTile = GetTile(anEvent.myToPosition);
	Chess_Tile* fromTile = GetTile(anEvent.myFromPosition);
	
	MovePiece(toTile, fromTile, anEvent.myEventSource, anEvent.myShouldEndTurn);

	Debug_Readout::WriteBoard(this);
	
	CheckGameOver();
}

void Chess_Board::OnReplacePieceRequested(const Event_ReplacePieceRequest& anEvent)
{
	if (anEvent.myFromPieceType != Chess_Pieces_EnumType::INVALID && anEvent.myToPieceType != Chess_Pieces_EnumType::INVALID)
	{
		ReplacePiece(anEvent.myRankAndFile, anEvent.myColour, anEvent.myToPieceType);
	}
}
#pragma endregion

#pragma region Piece Placement
void Chess_Board::MovePiece(Chess_Tile* const aToTile, Chess_Tile* const aFromTile, const Event_Source anEventSource, const bool aShouldAddMoveToHistory, const bool anEvaluateSpecial /*= true*/)
{
	if (aShouldAddMoveToHistory)
	{
		AddMoveToHistory(aFromTile, aToTile);
	}

	Chess_Piece* const takenPiece = aToTile->GetPiece();
	Chess_Piece* const takingPiece = aFromTile->GetPiece();
	const Chess_Pieces_EnumType takenPieceType = takenPiece ? takenPiece->GetType() : Chess_Pieces_EnumType::INVALID;

	RemovePiece(takenPiece);

	aToTile->SetPiece(takingPiece);
	takingPiece->SetHasMoved(myMoveHistory.size());
	aFromTile->SetPiece(nullptr);

	if (anEvaluateSpecial)
	{
		for (Chess_Rule_Special* rule : mySpecialRules)
		{
			rule->Evaluate(aFromTile, aToTile, takenPieceType, this, anEventSource);
		}
	}
}

void Chess_Board::RemovePiece(Chess_Piece* aPiece)
{
	if (aPiece == nullptr)
		return;

	const bool isWhite = aPiece->GetColour() == Chess_Pieces_Colour::WHITE;
	std::vector<Chess_Piece*>& pieceList = isWhite ? myWhitePieces : myBlackPieces;
	
	if (aPiece->GetType() == Chess_Pieces_EnumType::PAWN)
	{
		std::array<Chess_Pawn*, 8>& pawnList = isWhite ? myWhitePawns : myBlackPawns;
		pawnList[aPiece->GetTile()->GetRankAndFile().myFile] = nullptr;
	}

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

void Chess_Board::TakeBackLastMove(const Event_Source anEventSource)
{
	if (Chess_Move* previousMove = myMoveHistory.top())
	{
		const Chess_Special_Move_Type specialMoveType = previousMove->mySpecialMoveType;
		switch (specialMoveType)
		{
		case Chess_Special_Move_Type::CASTLE:
		case Chess_Special_Move_Type::ENPASSANT:
		case Chess_Special_Move_Type::PAWNPROMOTION:
		{
			for (Chess_Rule_Special* specialRule : mySpecialRules)
			{
				if (specialRule->GetSpecialRuleType() == specialMoveType)
				{
					specialRule->Revert(this, previousMove);
					break;
				}
			}
			break;
		}
		default:
		{
			Chess_Tile* fromTile = previousMove->myFromTile;
			Chess_Tile* toTile = previousMove->myToTile;

			MovePiece(fromTile, toTile, anEventSource, false, false);
			if (previousMove->myTakenPiece != Chess_Pieces_EnumType::INVALID)
			{
				const Chess_Pieces_Colour restoreColour = fromTile->GetPiece()->GetColour() == Chess_Pieces_Colour::WHITE ? Chess_Pieces_Colour::BLACK : Chess_Pieces_Colour::WHITE;
				Chess_Piece* restorePiece = Chess_Piece::CreatePiece(restoreColour, previousMove->myTakenPiece);
				PlacePiece(restorePiece, toTile);
				// CPW: To Do: the piece shouldn't get deleted and then newed again. Wherever it is - we need to add the piece to the old tile here.
			}

			fromTile->GetPiece()->ResetHasMoved(myMoveHistory.size());
			break;
		}
		}

		myMoveHistory.pop();
	}
}

void Chess_Board::PlacePiece(Chess_Piece* aPiece, Chess_Tile* aTile)
{
	const bool isWhite = aPiece->GetColour() == Chess_Pieces_Colour::WHITE;
	std::vector<Chess_Piece*>& pieceList = isWhite ? myWhitePieces : myBlackPieces;

	aTile->SetPiece(aPiece);
	pieceList.push_back(aPiece);

	if (aPiece->GetType() == Chess_Pieces_EnumType::PAWN)
	{
		std::array<Chess_Pawn*, 8>& pawnList = isWhite ? myWhitePawns : myBlackPawns;
		pawnList[aTile->GetRankAndFile().myFile] = nullptr;
	}
}

void Chess_Board::ReplacePiece(const Chess_RankAndFile& aRankAndFile, const Chess_Pieces_Colour aColour, const Chess_Pieces_EnumType aType)
{
	if (Chess_Tile* replaceTile = GetTile(aRankAndFile))
	{
		Chess_Piece* piece = Chess_Piece::CreatePiece(aColour, aType);

		RemovePiece(replaceTile->GetPiece());
		PlacePiece(piece, replaceTile);
	}
}
#pragma endregion

void Chess_Board::AddMoveToHistory(Chess_Tile* aFromTile, Chess_Tile* aToTile)
{
	const Chess_Piece* takingPiece = aFromTile->GetPiece();
	const Chess_Piece* takenPiece = aToTile->GetPiece();
	const Chess_Pieces_EnumType takingPieceType = takingPiece ? takingPiece->GetType() : Chess_Pieces_EnumType::INVALID;
	const Chess_Pieces_EnumType takenPieceType = takenPiece ? takenPiece->GetType() : Chess_Pieces_EnumType::INVALID;
	myMoveHistory.push(new Chess_Move(takingPieceType, takenPieceType, aFromTile, aToTile));

	myNumberOfMovesSincePawnMoveOrPieceTaken = (takingPieceType == Chess_Pieces_EnumType::PAWN || takenPiece) ? 0 : myNumberOfMovesSincePawnMoveOrPieceTaken + 1;
}

void Chess_Board::CheckGameOver()
{
	for (const Chess_Rule_GameOver* const gameOverRule : myGameOverRules)
	{
		gameOverRule->Evaluate(this);
	}
}

const std::vector<Chess_Move_Simple> Chess_Board::EvaluateAllPossibleMoves(const Chess_Pieces_Colour aColour)
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