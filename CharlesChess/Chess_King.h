#pragma once

#include "Chess_Piece.h"

#include "Chess_Rule_King_Castle.h"
#include "Chess_Rule_King_Move.h"

class Chess_King : public Chess_Piece
{
public:
	Chess_King(Chess_Pieces_Colour aColour, const int aPieceListIndex) : Chess_Piece(aColour, aPieceListIndex)
	{
		myChessRules.push_back(new Chess_Rule_King_Move());
		myChessRules.push_back(new Chess_Rule_King_Castle());
	};

	char GetConsoleIdentifier() const override { return 'K'; };
	std::string GetName() const override { return "King"; };
	Chess_Pieces_EnumType GetType() const override { return Chess_Pieces_EnumType::KING; };
	int GetScore() const override { return 900; };
};

