#pragma once

#include "Chess_Piece.h"

#include "Chess_Rule_Knight_Move.h"

class Chess_Knight : public Chess_Piece
{
public:
	Chess_Knight(Chess_Pieces_Colour aColour, const int aPieceListIndex) : Chess_Piece(aColour, aPieceListIndex)
	{
		myChessRules.push_back(new Chess_Rule_Knight_Move());
	};

	char GetConsoleIdentifier() const override { return 'N'; };
	std::string GetName() const override { return "Knight"; };
	Chess_Pieces_EnumType GetType() const override { return Chess_Pieces_EnumType::KNIGHT; };
	int GetScore() const override { return 30; };
};

