#pragma once

#include "Chess_Rule_GameOver.h"

class Chess_Rule_GameOver_StaleMate_50Moves : public Chess_Rule_GameOver
{
public:
	void Evaluate(Chess_Board* const aChessBoard) const override
	{
		if (aChessBoard->GetNumberOfMovesSincePawnMoveOrPieceTaken() >= 50)
		{
			// Pawn thing is shonky at the moment because of evaluations from AI look ahead etc.
			//Event_Handler::GetInstance()->SendGameOverEvent(Chess_GameOverResult::STALEMATE);
		}
	}
};