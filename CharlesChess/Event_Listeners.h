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

class Event_Listener_RemovePieceRequest
{
public:
	virtual void OnRemovePieceRequested(const Event_RemovePieceRequest& anEvent) = 0;
};

class Event_Listener_ReplacePieceRequest
{
public:
	virtual void OnReplacePieceRequested(const Event_ReplacePieceRequest& anEvent) = 0;
};

class Event_Listener_GameOver
{
public:
	virtual void OnGameOver(const Event_GameOver& anEvent) = 0;
};