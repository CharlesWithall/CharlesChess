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

void Event_Handler::SendPieceSelectedEvent(const Chess_RankAndFile& aRankAndFile)
{
	Event_PieceSelected pieceSelectedEvent = Event_PieceSelected(aRankAndFile);

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
		evaluatedMovesEvent.myPossibleRankAndFiles.push_back(tile->GetRankAndFile());
	}

	for (Event_Listener_EvaluatedPossibleMoves* evaluatedMovesListener : myEvaluatedMovesListeners)
	{
		evaluatedMovesListener->OnMovesEvaluated(evaluatedMovesEvent);
	}
}

void Event_Handler::SendMovePieceRequestEvent(const Chess_RankAndFile& aFromPosition, const Chess_RankAndFile& aToPosition, const bool aShouldEndturn, const Event_Source anEventSource)
{
	Event_MovePieceRequest movePieceRequest = Event_MovePieceRequest(aFromPosition, aToPosition, anEventSource, aShouldEndturn);

	for (Event_Listener_MovePieceRequest* listener : myMovePieceRequestListeners)
	{
		listener->OnMovePieceRequested(movePieceRequest);
	}
}

void Event_Handler::SendReplacePieceRequestEvent(const Chess_RankAndFile& aRankAndFile, const Chess_Pieces_Colour aColour, const Chess_Pieces_EnumType aFromPieceType, const Chess_Pieces_EnumType aToPieceType)
{
	Event_ReplacePieceRequest replacePieceRequest = Event_ReplacePieceRequest(aRankAndFile, aColour, aFromPieceType, aToPieceType);

	for (Event_Listener_ReplacePieceRequest* listener : myReplacePieceRequestListeners)
	{
		listener->OnReplacePieceRequested(replacePieceRequest);
	}
}

void Event_Handler::SendRemovePieceRequestEvent(const Chess_RankAndFile& aRankAndFile, const Event_Source anEventSource)
{
	if (anEventSource == Event_Source::EVALUATION)
	{
		return;
	}

	Event_RemovePieceRequest removePieceRequest = Event_RemovePieceRequest(aRankAndFile);

	for (Event_Listener_RemovePieceRequest* listener : myRemovePieceRequestListeners)
	{
		listener->OnRemovePieceRequested(removePieceRequest);
	}
}

void Event_Handler::SendGameOverEvent(const Chess_GameOverResult aGameOverResult)
{
	Event_GameOver gameOver = Event_GameOver(aGameOverResult);

	for (Event_Listener_GameOver* listener : myGameOverListeners)
	{
		listener->OnGameOver(gameOver);
	}
}