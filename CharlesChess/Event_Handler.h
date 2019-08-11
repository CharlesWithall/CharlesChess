#pragma once

#include "Chess_Defines.h"

#include <vector>

class Chess_Tile;

class Event_Listener_EvaluatedPossibleMoves;
class Event_Listener_PieceSelected;

class Event_Handler
{
public:

	static Event_Handler* GetInstance();

	void RegisterPieceSelectedListener(Event_Listener_PieceSelected* aListener) { myPieceSelectedListeners.push_back(aListener); }
	void RegisterEvaluatedPossibleMovesListener(Event_Listener_EvaluatedPossibleMoves* aListener) { myEvaluatedMovesListeners.push_back(aListener); }

	void SendEvaluatedPossibleMovesEvent(const std::vector<Chess_Tile*>& somePossibleMoves);
	void SendPieceSelectedEvent(const int aRankIndex, const int aFilexIndex);
private:
	std::vector<Event_Listener_PieceSelected*> myPieceSelectedListeners;
	std::vector<Event_Listener_EvaluatedPossibleMoves*> myEvaluatedMovesListeners;

	static Event_Handler* myInstance;
};