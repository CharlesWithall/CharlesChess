#pragma once

#include "Chess_Piece.h"

#include "Chess_Rule_Bishop_Move.h"

class Chess_Bishop : public Chess_Piece
{
public:
	Chess_Bishop(Chess_Pieces_Colour aColour, const int aPieceListIndex) : Chess_Piece(aColour, aPieceListIndex)
	{
		myChessRules.push_back(new Chess_Rule_Bishop_Move());
	};

	char GetConsoleIdentifier() const override { return 'B'; };
	std::string GetName() const override { return "Bishop"; };
	Chess_Pieces_EnumType GetType() const override { return Chess_Pieces_EnumType::BISHOP; };
	int GetScore() const override { return 30; };
};

