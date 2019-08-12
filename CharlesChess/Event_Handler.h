#pragma once

#include "Chess_Defines.h"

#include <vector>

class Chess_Tile;

class Event_Listener_EvaluatedPossibleMoves;
class Event_Listener_PieceSelected;
class Event_Listener_MovePieceRequest;

class Event_Handler
{
public:

	static Event_Handler* GetInstance();

	// Registration
	void RegisterPieceSelectedListener(Event_Listener_PieceSelected* aListener) { myPieceSelectedListeners.push_back(aListener); }
	void RegisterEvaluatedPossibleMovesListener(Event_Listener_EvaluatedPossibleMoves* aListener) { myEvaluatedMovesListeners.push_back(aListener); }
	void RegisterMovePieceRequestListener(Event_Listener_MovePieceRequest* aListener) { myMovePieceRequestListeners.push_back(aListener); }

	// Send Events
	void SendEvaluatedPossibleMovesEvent(const std::vector<Chess_Tile*>& somePossibleMoves);
	void SendPieceSelectedEvent(const Chess_RankAndFile& aRankAndFile);
	void SendMovePieceRequestEvent(const Chess_RankAndFile& aFromPosition, const Chess_RankAndFile& aToPosition);

private:
	std::vector<Event_Listener_PieceSelected*> myPieceSelectedListeners;
	std::vector<Event_Listener_EvaluatedPossibleMoves*> myEvaluatedMovesListeners;
	std::vector<Event_Listener_MovePieceRequest*> myMovePieceRequestListeners;

	static Event_Handler* myInstance;
};