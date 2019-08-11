#pragma once

#include "Chess_Piece.h"

#include "Chess_Rule_Rook_Move.h"

class Chess_Rook : public Chess_Piece
{
public:
	Chess_Rook(Chess_Pieces_Colour aColour) : Chess_Piece(aColour)
	{
		myChessRules.push_back(new Chess_Rule_Rook_Move());
	};
	~Chess_Rook();

	char GetConsoleIdentifier() const override { return 'R'; };
	std::string GetName() const override { return "Rook"; };
	Chess_Pieces_EnumType GetType() const override { return Chess_Pieces_EnumType::ROOK; };
	int GetScore() const override { return 5; };
};

