#pragma once

#include "Chess_Defines.h"
#include "Chess_Piece.h"

#include "Debug_Readout.h"

#include <iostream>

struct Chess_Move_Simple
{
	Chess_Move_Simple()
		: myFromTile(Chess_RankAndFile(-1,-1))
		, myToTile(Chess_RankAndFile(-1,-1))
	{

	}
	Chess_Move_Simple(const Chess_RankAndFile& aFromTile, const Chess_RankAndFile& aToTile)
		: myFromTile(aFromTile)
		, myToTile(aToTile)
	{
	}

	Chess_Move_Simple(char* aFromName, char* aToName)
		: myFromTile(Chess_RankAndFile(aFromName))
		, myToTile(Chess_RankAndFile(aToName))
	{
	}

	bool operator==(const Chess_Move_Simple& rhs) const
	{
		if (myFromTile == rhs.myFromTile && myToTile == rhs.myToTile)
		{
			return true;
		}

		return false;
	}

	bool operator!=(const Chess_Move_Simple& rhs) const
	{
		if (myFromTile == rhs.myFromTile && myToTile == rhs.myToTile)
		{
			return false;
		}

		return true;
	}

	Chess_RankAndFile myFromTile;
	Chess_RankAndFile myToTile;
}; // CPW: To Do - Change to Chess_Tile*s

struct Chess_Move
{
	Chess_Move()
		: myTakingPiece(nullptr)
		, myTakenPiece(nullptr)
		, myPromotedPawn(nullptr)
		, myFromTile(nullptr)
		, myToTile(nullptr)
		, mySpecialMoveType(Chess_Special_Move_Type::NONE)
	{
	}

	Chess_Move(Chess_Piece* aTakingPiece,
		Chess_Piece* aTakenPiece,
		Chess_Tile* aFromTile,
		Chess_Tile* aToTile)
		: myTakingPiece(aTakingPiece)
		, myTakenPiece(aTakenPiece)
		, myPromotedPawn(nullptr)
		, myFromTile(aFromTile)
		, myToTile(aToTile)
		, mySpecialMoveType(Chess_Special_Move_Type::NONE)
	{
#if DEBUG_ENABLED
		/*std::string pieceName;
		switch (aPiece)
		{
		case Chess_Pieces_EnumType::BISHOP: pieceName = "Bishop"; break;
		case Chess_Pieces_EnumType::KING: pieceName = "King"; break;
		case Chess_Pieces_EnumType::KNIGHT: pieceName = "Knight"; break;
		case Chess_Pieces_EnumType::PAWN: pieceName = "Pawn"; break;
		case Chess_Pieces_EnumType::QUEEN: pieceName = "Queen"; break;
		case Chess_Pieces_EnumType::ROOK: pieceName = "Rook"; break;
		default: pieceName = "Invalid"; break;
		}

		std::string conjunctionString = myTakenPiece != Chess_Pieces_EnumType::INVALID ? " takes " : " to ";

		std::cout << pieceName << ": " << myFromTile->GetRankAndFile().GetReadableName() << conjunctionString << myToTile->GetRankAndFile().GetReadableName() << std::endl;*/
#endif	
	}

	void SetSpecialMoveType(const Chess_Special_Move_Type aSpecialMoveType) { mySpecialMoveType = aSpecialMoveType; }

	Chess_Piece* myTakingPiece;
	Chess_Piece* myTakenPiece;
	Chess_Piece* myPromotedPawn;
	Chess_Tile* myFromTile;
	Chess_Tile* myToTile;
	Chess_Special_Move_Type mySpecialMoveType;
};