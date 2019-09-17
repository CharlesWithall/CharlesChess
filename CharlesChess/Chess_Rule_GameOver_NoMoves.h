#pragma once

#include "Chess_Rule_GameOver.h"

class Chess_Rule_GameOver_NoMoves : public Chess_Rule_GameOver
{
public:
	void Evaluate(const Chess_Board* const aChessBoard) const override
	{
		const Chess_GameOverResult winResult = aChessBoard->GetActivePlayer() == Chess_Pieces_Colour::WHITE ? Chess_GameOverResult::BLACKWINS : Chess_GameOverResult::WHITEWINS;
		const Chess_GameOverResult gameOverResult = myCheckRuleHandler.Evaluate(aChessBoard, aChessBoard->GetActivePlayer()) ? winResult : Chess_GameOverResult::STALEMATE;
			
		if (aChessBoard->EvaluateAllPossibleMoves(aChessBoard->GetActivePlayer()).size() == 0)
		{
			Event_Handler::GetInstance()->SendGameOverEvent(gameOverResult);
		}
	}
};