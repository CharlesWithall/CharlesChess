#pragma once

#include "Chess_Board.h"
#include "Chess_Check_Rule.h"
#include "Chess_Defines.h"

#include "Event_Handler.h"

class Chess_Rule_GameOver
{
public:
	virtual void Evaluate(Chess_Board* const aChessBoard) const = 0;

protected:
	Chess_Check_Rule myCheckRuleHandler;
};