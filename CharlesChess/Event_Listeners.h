#pragma once

#include "Event_Events.h"

class Event_Listener_PieceSelected
{
public:
	virtual void OnPieceSelected(const Event_PieceSelected& anEvent) = 0;
};

class Event_Listener_EvaluatedPossibleMoves
{
public:
	virtual void OnMovesEvaluated(const Event_EvaulatedPossibleMoves& anEvent) = 0;
};

class Event_Listener_MovePieceRequest
{
public:
	virtual void OnMovePieceRequested(const Event_MovePieceRequest& anEvent) = 0;
};
