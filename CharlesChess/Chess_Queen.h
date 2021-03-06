#pragma once

#include "Chess_Piece.h"

#include "Chess_Rule_Queen_Move.h"

class Chess_Queen : public Chess_Piece
{
public:
	Chess_Queen(Chess_Pieces_Colour aColour, const int aPieceListIndex) : Chess_Piece(aColour, aPieceListIndex)
	{
		myChessRules.push_back(new Chess_Rule_Queen_Move());
	};

	char GetConsoleIdentifier() const override { return 'Q'; };
	std::string GetName() const override { return "Queen"; };
	Chess_Pieces_EnumType GetType() const override { return Chess_Pieces_EnumType::QUEEN; };
	int GetScore() const override { return 90; };
};