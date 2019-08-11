#pragma once

#include "Chess_Tile.h"

#include "Event_Handler.h"
#include "Event_Events.h"
#include "Event_Listeners.h"

Event_Handler* Event_Handler::myInstance = nullptr;

Event_Handler* Event_Handler::GetInstance()
{
	if (myInstance == nullptr)	myInstance = new Event_Handler();
	return myInstance;
}

void Event_Handler::SendPieceSelectedEvent(const int aRankIndex, const int aFilexIndex)
{
	Event_PieceSelected pieceSelectedEvent = Event_PieceSelected(aRankIndex, aFilexIndex);

	for (Event_Listener_PieceSelected* pieceSelectedListener : myPieceSelectedListeners)
	{
		pieceSelectedListener->OnPieceSelected(pieceSelectedEvent);
	}
}

void Event_Handler::SendEvaluatedPossibleMovesEvent(const std::vector<Chess_Tile*>& somePossibleMoves)
{
	Event_EvaulatedPossibleMoves evaluatedMovesEvent;

	for (Chess_Tile* tile : somePossibleMoves)
	{
		evaluatedMovesEvent.myPossibleMoveIndices.push_back(Event_EvaulatedPossibleMoves::RankFile(tile->GetRank() - 1, (int)tile->GetFile() - 1));
	}

	for (Event_Listener_EvaluatedPossibleMoves* evaluatedMovesListener : myEvaluatedMovesListeners)
	{
		evaluatedMovesListener->OnMovesEvaluated(evaluatedMovesEvent);
	}
}