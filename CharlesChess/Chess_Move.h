#pragma once

#include "Chess_Defines.h"
#include "Chess_Piece.h"

#include <iostream>

struct Chess_Move_Simple
{
	Chess_Move_Simple(const Chess_RankAndFile& aFromTile, const Chess_RankAndFile& aToTile)
		: myFromTile(aFromTile)
		, myToTile(aToTile)
	{
	}

	Chess_RankAndFile myFromTile;
	Chess_RankAndFile myToTile;
};

struct Chess_Move
{
	Chess_Move(const Chess_Pieces_EnumType aPiece,
		const Chess_RankAndFile& aToTile,
		const Chess_RankAndFile& aFromTile,
		const bool anIsTakingPiece,
		const bool anIsCheck)
		: myPiece(aPiece)
		, myToTile(aToTile)
		, myFromTile(aFromTile)
		, myIsTakingPiece(anIsTakingPiece)
		, myIsCheck(anIsCheck)
	{
		std::string pieceName;
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

		std::string conjunctionString = myIsTakingPiece ? " takes " : " to ";

		std::cout << pieceName << ": " << myFromTile.GetReadableName() << conjunctionString << myToTile.GetReadableName() << std::endl;
	}

	Chess_Pieces_EnumType myPiece;
	Chess_RankAndFile myToTile;
	Chess_RankAndFile myFromTile;
	bool myIsTakingPiece;
	bool myIsCheck;
};