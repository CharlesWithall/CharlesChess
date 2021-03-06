#pragma once

#include "Chess_Piece.h"

#include "Chess_Rule_Pawn_Move.h"
#include "Chess_Rule_Pawn_EnPassant.h"
#include "Chess_Rule_Pawn_Take.h"

class Chess_Pawn : public Chess_Piece
{
public:
	Chess_Pawn(Chess_Pieces_Colour aColour, const int aPieceListIndex) : Chess_Piece(aColour, aPieceListIndex)
	{
		myChessRules.push_back(new Chess_Rule_Pawn_Move());
		myChessRules.push_back(new Chess_Rule_Pawn_EnPassant());
		myChessRules.push_back(new Chess_Rule_Pawn_Take());
	};

	char GetConsoleIdentifier() const override { return 'P'; };
	std::string GetName() const override { return "Pawn"; };
	Chess_Pieces_EnumType GetType() const override { return Chess_Pieces_EnumType::PAWN; };
	int GetScore() const override { return 10; };
};

