#include "Chess_Board.h"

// Pieces
#include "Chess_Bishop.h"
#include "Chess_King.h"
#include "Chess_Knight.h"
#include "Chess_Pawn.h"
#include "Chess_Queen.h"
#include "Chess_Rook.h"
//

// Rules
#include "Chess_Rule_GameOver_NoMoves.h"
#include "Chess_Rule_GameOver_NotEnoughPieces.h"
#include "Chess_Rule_GameOver_StaleMate_50Moves.h"
#include "Chess_Rule_Special_Castle.h"
#include "Chess_Rule_Special_EnPassant.h"
#include "Chess_Rule_Special_PawnPromotion.h"
//

#include "Chess_Defines.h"
#include "Chess_Helpers.h"
#include "Chess_Move.h"
#include "Chess_Tile.h"

#include "Audio_Model.h"

#include "Debug_Defines.h"

#include "Event_Handler.h"

static const int pawnIndexOffset = 8;

#pragma region Construction and Destruction
Chess_Board::Chess_Board()
{
	InitTiles();
	InitPieces();

	Event_Handler::GetInstance()->RegisterPieceSelectedListener(this);
	Event_Handler::GetInstance()->RegisterMovePieceRequestListener(this);

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

	for (Chess_Rule_GameOver* rule : myGameOverRules)
		if (rule) { delete rule; rule = nullptr; }

	Event_Handler::GetInstance()->UnregisterPieceSelectedListener(this);
	Event_Handler::GetInstance()->UnregisterMovePieceRequestListener(this);
}

void Chess_Board::InitTiles()
{
	for (size_t i = 0; i < myChessTiles.size(); ++i)
	{
		Chess_RankArray& rank = myChessTiles[i];
		for (size_t j = 0; j < rank.size(); ++j)
		{
			rank[j] = new Chess_Tile(Chess_RankAndFile(i, j));
		}
	}
}

void Chess_Board::InitPieces()
{
#if DEBUG_USE_TEST_BOARD
	Init_Debug_Test_Pieces();
	return;
#endif

	// Init Kings to position [0]
	const int kingX = 4;
	const int kingWhiteY = 0;
	const int kingBlackY = 7;

	myChessTiles[kingX][kingWhiteY]->SetPiece(CreatePiece(Chess_RankAndFile(kingX, kingWhiteY)));
	myChessTiles[kingX][kingBlackY]->SetPiece(CreatePiece(Chess_RankAndFile(kingX, kingBlackY)));

	myWhitePieces[0] = myChessTiles[kingX][kingWhiteY]->GetPiece();
	myBlackPieces[0] = myChessTiles[kingX][kingBlackY]->GetPiece();

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
					myWhitePieces[newPiece->GetPieceListIndex()] = newPiece;
				else
					myBlackPieces[newPiece->GetPieceListIndex()] = newPiece;
			}
		}
	}
}

Chess_Piece* Chess_Board::CreatePiece(const Chess_RankAndFile& aRankAndFile)
{
	const int rank = aRankAndFile.myRank;
	const int file = aRankAndFile.myFile;

	if (rank == 1 || rank == 6)
	{
		const Chess_Pieces_Colour pieceColour = rank == 1 ? Chess_Pieces_Colour::WHITE : Chess_Pieces_Colour::BLACK;
		std::array<Chess_Piece*, 16>& pieceList = pieceColour == Chess_Pieces_Colour::WHITE ? myWhitePieces : myBlackPieces;

		int pawnIndex = 0;
		for (int i = pawnIndexOffset; i < 16; ++i)
		{
			if (pieceList[i])
				++pawnIndex;
			else
				break;
		}

		return new Chess_Pawn(pieceColour, pawnIndexOffset + pawnIndex);
	}	

	if (rank == 0 || rank == 7)
	{
		const Chess_Pieces_Colour pieceColour = rank == 0 ? Chess_Pieces_Colour::WHITE : Chess_Pieces_Colour::BLACK;
		std::array<Chess_Piece*, 16>& pieceList = pieceColour == Chess_Pieces_Colour::WHITE ? myWhitePieces : myBlackPieces;

		if (file == 0 || file == 7) return new Chess_Rook(pieceColour, pieceList[2] == nullptr ? 2 : 3);
		if (file == 1 || file == 6) return new Chess_Knight(pieceColour, pieceList[6] == nullptr ? 6 : 7);
		if (file == 2 || file == 5) return new Chess_Bishop(pieceColour, pieceList[4] == nullptr ? 4 : 5);
		if (file == 3) return new Chess_Queen(pieceColour, 1);
		if (file == 4) return new Chess_King(pieceColour, 0);
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

const bool Chess_Board::GetIsPassedPawn(const Chess_Pawn* aPawn) const // CPW: TODO: THIS IS BORKED - WE'RE ASSUMING PAWNS NEVER TAKE THROUGH THE WHOLE GAME FOR THIS TO WORK!!!
{
	const int file = aPawn->GetTile()->GetRankAndFile().myFile;
	const int rank = aPawn->GetTile()->GetRankAndFile().myRank;

	if (aPawn->GetColour() == Chess_Pieces_Colour::WHITE)
	{
		for (int i = file - 1; i <= file + 1; ++i)
		{
			if (i < 0 || i >= 8)
				continue;

			for (int j = rank + 1; j < 8; ++j)
			{
				Chess_Piece* piece = GetTile(Chess_RankAndFile(i, j))->GetPiece();
				if (piece && piece->GetColour() == Chess_Pieces_Colour::BLACK && piece->GetType() == Chess_Pieces_EnumType::PAWN)
				{
					return false;
				}
			}
		}
	}

	if (aPawn->GetColour() == Chess_Pieces_Colour::BLACK)
	{
		for (int i = file - 1; i <= file + 1; ++i)
		{
			if (i < 0 || i >= 8)
				continue;

			for (int j = rank - 1; j >= 0; --j)
			{
				Chess_Piece* piece = GetTile(Chess_RankAndFile(i, j))->GetPiece();
				if (piece && piece->GetColour() == Chess_Pieces_Colour::WHITE && piece->GetType() == Chess_Pieces_EnumType::PAWN)
				{
					return false;
				}
			}
		}
	}

	return true;
}

#pragma region Event Responses
void Chess_Board::OnPieceSelected(const Event_PieceSelected& anEvent)
{
	const Chess_Tile* const chessTile = GetTile(anEvent.myRankAndFile);
	if (const Chess_Piece* const chessPiece = chessTile->GetPiece())
	{
		Event_Handler::GetInstance()->SendEvaluatedPossibleMovesEvent(chessPiece->EvaluateMoves(this));
	}
}

void Chess_Board::OnMovePieceRequested(const Event_MovePieceRequest& anEvent)
{
	Chess_Tile* toTile = GetTile(anEvent.myToPosition);
	Chess_Tile* fromTile = GetTile(anEvent.myFromPosition);

	PerformMovePieceRequest(fromTile, toTile, true);
}

void Chess_Board::PerformMovePieceRequest(Chess_Tile* const aFromTile, Chess_Tile* const aToTile, const bool aShouldPerformGameOverCheck)
{
	AddMoveToHistory(aFromTile, aToTile);

	const Chess_Special_Move_Type specialMoveType = HandleSpecialMove_MovePiece(aFromTile, aToTile);
	if (specialMoveType == Chess_Special_Move_Type::NONE)
	{
		MovePiece(aFromTile, aToTile);
	}
	else
	{
		myMoveHistory.back().mySpecialMoveType = specialMoveType;
	}

	if (aShouldPerformGameOverCheck)
	{
		CheckGameOver();
		Audio_Model::GetInstance()->Play_MovePiece();
	}
}

void Chess_Board::OnTakeBackPieceRequested()
{
	if (myMoveHistory.empty())
		return;

	const Chess_Move& previousMove = myMoveHistory.back();

	if (!HandleSpecialMove_TakeBack(previousMove))
	{
		Chess_Tile* fromTile = previousMove.myFromTile;
		Chess_Tile* toTile = previousMove.myToTile;

		MovePiece(toTile, fromTile);
		fromTile->GetPiece()->ResetHasMoved(GetHistoryIndex());
		if (previousMove.myTakenPiece)
			PlacePiece(previousMove.myTakenPiece, toTile);
	}
	
	myMoveHistory.pop_back();
}
#pragma endregion

#pragma region Piece Locomotion
void Chess_Board::MovePiece(Chess_Tile* const aFromTile, Chess_Tile* const aToTile)
{
	Chess_Piece* const takenPiece = aToTile->GetPiece();
	Chess_Piece* const takingPiece = aFromTile->GetPiece();
	const Chess_Pieces_EnumType takenPieceType = takenPiece ? takenPiece->GetType() : Chess_Pieces_EnumType::INVALID;

	RemovePiece(takenPiece);

	aToTile->SetPiece(takingPiece);
	takingPiece->SetHasMoved(myMoveHistory.size());
	aFromTile->SetPiece(nullptr);
}

void Chess_Board::RemovePiece(Chess_Piece* aPiece)
{
	if (aPiece == nullptr)
		return;

	if (aPiece->GetType() == Chess_Pieces_EnumType::KING)
	{
		DEBUG_PRINT_CHESS("ATTEMPTING TO REMOVE KING - THIS SHOULDN'T HAPPEN UNDER ANY CIRCUMSTANCES - TREASON!!!");
		return;
	}

	const bool isWhite = aPiece->GetColour() == Chess_Pieces_Colour::WHITE;
	std::array<Chess_Piece*, 16>& pieceList = isWhite ? myWhitePieces : myBlackPieces;
	std::vector<Chess_Piece*>& extraPieceList = isWhite ? myExtraWhitePieces : myExtraBlackPieces;

	for (int i = pieceList.size() - 1; i >= 0; --i)
	{
		if (pieceList[i] == aPiece)
		{
			pieceList[aPiece->GetPieceListIndex()] = nullptr;
			const_cast<Chess_Tile*>(aPiece->GetTile())->SetPiece(nullptr);
			aPiece->SetTile(nullptr);
			return;
		}
	}

	for (int i = extraPieceList.size() - 1; i >= 0; --i)
	{
		if (extraPieceList[i] == aPiece)
		{
			// Don't set this reference to nullptr - we want to keep the extra piece around to be reused.
			const_cast<Chess_Tile*>(aPiece->GetTile())->SetPiece(nullptr);
			aPiece->SetTile(nullptr);
			return;
		}
	}
}

void Chess_Board::PlacePiece(Chess_Piece* aPiece, Chess_Tile* aTile)
{
	const bool isWhite = aPiece->GetColour() == Chess_Pieces_Colour::WHITE;
	std::array<Chess_Piece*, 16>& pieceList = isWhite ? myWhitePieces : myBlackPieces;

	aTile->SetPiece(aPiece);

	if (aPiece->GetPieceListIndex() >= 0) // Else - this piece is an 'extra piece' and should already be in the extras list
		pieceList[aPiece->GetPieceListIndex()] = aPiece;
}

void Chess_Board::PlacePiece(const Chess_RankAndFile& aRankAndFile, const Chess_Pieces_Colour aColour, const Chess_Pieces_EnumType aType)
{
	if (Chess_Tile* replaceTile = GetTile(aRankAndFile))
	{
		Chess_Piece* newPiece = nullptr;
		std::vector<Chess_Piece*>& extraPieces = aColour == Chess_Pieces_Colour::WHITE ? myExtraWhitePieces : myExtraBlackPieces;
		for (Chess_Piece* extraPiece : extraPieces)
		{
			if (!extraPiece->GetTile() && extraPiece->GetType() == aType)
			{
				newPiece = extraPiece;
			}
		}

		if (!newPiece)
		{
			newPiece = Chess_Piece::CreatePiece(aColour, aType, -1);
			extraPieces.push_back(newPiece);
		}

		RemovePiece(replaceTile->GetPiece());
		PlacePiece(newPiece, replaceTile);
	}
}
#pragma endregion

#pragma region Special Moves
Chess_Special_Move_Type Chess_Board::HandleSpecialMove_MovePiece(Chess_Tile* const aFromTile, Chess_Tile* const aToTile)
{
	for (Chess_Rule_Special* rule : mySpecialRules)
	{
		if (rule->Evaluate(aFromTile, aToTile, this))
		{
			rule->Execute(aFromTile, aToTile, this);
			return rule->GetSpecialRuleType();
		}
	}

	return Chess_Special_Move_Type::NONE;
}

bool Chess_Board::HandleSpecialMove_TakeBack(const Chess_Move& aMove)
{
	const Chess_Special_Move_Type specialMoveType = aMove.mySpecialMoveType;

	for (Chess_Rule_Special* specialRule : mySpecialRules)
	{
		if (specialRule->GetSpecialRuleType() == specialMoveType)
		{
			specialRule->Revert(aMove, this);
			return true;
		}
	}

	return false;
}
#pragma endregion

void Chess_Board::AddMoveToHistory(Chess_Tile* aFromTile, Chess_Tile* aToTile)
{
	Chess_Piece* takingPiece = aFromTile->GetPiece();
	Chess_Piece* takenPiece = aToTile->GetPiece();
	myMoveHistory.push_back(Chess_Move(takingPiece, takenPiece, aFromTile, aToTile));

	myNumberOfMovesSincePawnMoveOrPieceTaken = (takingPiece->GetType() == Chess_Pieces_EnumType::PAWN || takenPiece) ? 0 : myNumberOfMovesSincePawnMoveOrPieceTaken + 1;
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
	const std::array<Chess_Piece*, 16>& pieceList = aColour == Chess_Pieces_Colour::WHITE ? myWhitePieces : myBlackPieces;
	for (const Chess_Piece* const piece : pieceList)
	{
		if (!piece)
			continue;

		const std::vector<Chess_Tile*>& possibleMoves = piece->EvaluateMoves(this);
		for (const Chess_Tile* const tile : possibleMoves)
		{
			outMoves.push_back(Chess_Move_Simple(piece->GetTile()->GetRankAndFile(), tile->GetRankAndFile()));
		}
	}

	return outMoves;
}

// DEBUG

void Chess_Board::Init_Debug_Test_Pieces()
{
	//// In this test position, black to play, the algorithm chose Bishop H6 to E3 which is a shit move.
	//std::array<std::array<std::string, 8>, 8> testPositionMap;
	//testPositionMap[7] = { "bR", "xX", "bB", "xX", "xX", "bK", "xX", "bR" };
	//testPositionMap[6] = { "bP", "bP", "bP", "bQ", "xX", "xX", "xX", "bP" };
	//testPositionMap[5] = { "bN", "xX", "xX", "xX", "xX", "bP", "xX", "bB" };
	//testPositionMap[4] = { "xX", "xX", "xX", "xX", "xX", "xX", "xX", "xX" };
	//testPositionMap[3] = { "xX", "xX", "wB", "xX", "xX", "xX", "xX", "wN" };
	//testPositionMap[2] = { "wP", "xX", "xX", "xX", "xX", "xX", "xX", "xX" };
	//testPositionMap[1] = { "xX", "wP", "wP", "xX", "wQ", "wP", "wP", "wP" };
	//testPositionMap[0] = { "wR", "wN", "xX", "xX", "xX", "wR", "xX", "wK" };

	//for (int i = 0; i < 8; ++i)
	//{
	//	for (int j = 0; j < 8; ++j)
	//	{
	//		Chess_Pieces_Colour colour;
	//		Chess_Piece* piece;
	//		switch (testPositionMap[i][j][0])
	//		{
	//		case 'b':
	//			colour = Chess_Pieces_Colour::BLACK;
	//			break;
	//		case 'w':
	//			colour = Chess_Pieces_Colour::WHITE;
	//			break;
	//		default:
	//			continue;
	//		}

	//		switch (testPositionMap[i][j][1])
	//		{
	//		case 'P':
	//			piece = new Chess_Pawn(colour);
	//			break;
	//		case 'N':
	//			piece = new Chess_Knight(colour);
	//			break;
	//		case 'B':
	//			piece = new Chess_Bishop(colour);
	//			break;
	//		case 'R':
	//			piece = new Chess_Rook(colour);
	//			break;
	//		case 'K':
	//			piece = new Chess_King(colour);
	//			break;
	//		case 'Q':
	//			piece = new Chess_Queen(colour);
	//			break;
	//		default:
	//			continue;
	//		}

	//		myChessTiles[j][i]->SetPiece(piece);
	//		if (colour == Chess_Pieces_Colour::WHITE)
	//		{
	//			myWhitePieces.push_back(piece);
	//			if (piece->GetType() == Chess_Pieces_EnumType::PAWN)
	//			{
	//				myWhitePawns[j] = static_cast<Chess_Pawn*>(piece);
	//			}
	//		}
	//		else
	//		{
	//			myBlackPieces.push_back(piece);
	//			if (piece->GetType() == Chess_Pieces_EnumType::PAWN)
	//			{
	//				myBlackPawns[j] = static_cast<Chess_Pawn*>(piece);
	//			}
	//		}
	//	}
	//}
}