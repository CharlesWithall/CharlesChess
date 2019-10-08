#pragma once

#include "Chess_Defines.h"

#include "Event_Defines.h"

#include <algorithm>
#include <vector>

class Chess_Tile;

class Event_Listener_EvaluatedPossibleMoves;
class Event_Listener_GameOver;
class Event_Listener_PieceSelected;
class Event_Listener_MovePieceRequest;
class Event_Listener_RemovePieceRequest;
class Event_Listener_ReplacePieceRequest;

class Event_Handler
{
public:

	static Event_Handler* GetInstance();

	// Registration
	void RegisterPieceSelectedListener(Event_Listener_PieceSelected* aListener) { myPieceSelectedListeners.push_back(aListener); }
	void RegisterEvaluatedPossibleMovesListener(Event_Listener_EvaluatedPossibleMoves* aListener) { myEvaluatedMovesListeners.push_back(aListener); }
	void RegisterMovePieceRequestListener(Event_Listener_MovePieceRequest* aListener) { myMovePieceRequestListeners.push_back(aListener); }
	void RegisterRemovePieceRequestListener(Event_Listener_RemovePieceRequest* aListener) { myRemovePieceRequestListeners.push_back(aListener); }
	void RegisterReplacePieceRequestListener(Event_Listener_ReplacePieceRequest* aListener) { myReplacePieceRequestListeners.push_back(aListener); }
	void RegisterGameOverListener(Event_Listener_GameOver* aListener) { myGameOverListeners.push_back(aListener); }

	void UnregisterPieceSelectedListener(Event_Listener_PieceSelected* aListener) { myPieceSelectedListeners.erase(std::remove(myPieceSelectedListeners.begin(), myPieceSelectedListeners.end(), aListener), myPieceSelectedListeners.end()); }
	void UnregisterEvaluatedPossibleMovesListener(Event_Listener_EvaluatedPossibleMoves* aListener) { myEvaluatedMovesListeners.erase(std::remove(myEvaluatedMovesListeners.begin(), myEvaluatedMovesListeners.end(), aListener), myEvaluatedMovesListeners.end()); }
	void UnregisterMovePieceRequestListener(Event_Listener_MovePieceRequest* aListener) { myMovePieceRequestListeners.erase(std::remove(myMovePieceRequestListeners.begin(), myMovePieceRequestListeners.end(), aListener), myMovePieceRequestListeners.end()); }
	void UnregisterRemovePieceRequestListener(Event_Listener_RemovePieceRequest* aListener) { myRemovePieceRequestListeners.erase(std::remove(myRemovePieceRequestListeners.begin(), myRemovePieceRequestListeners.end(), aListener), myRemovePieceRequestListeners.end());	}
	void UnregisterReplacePieceRequestListener(Event_Listener_ReplacePieceRequest* aListener) { myReplacePieceRequestListeners.erase(std::remove(myReplacePieceRequestListeners.begin(), myReplacePieceRequestListeners.end(), aListener), myReplacePieceRequestListeners.end()); }
	void UnregisterGameOverListener(Event_Listener_GameOver* aListener) { myGameOverListeners.erase(std::remove(myGameOverListeners.begin(), myGameOverListeners.end(), aListener), myGameOverListeners.end()); }
	
	// Send Events
	void SendEvaluatedPossibleMovesEvent(const std::vector<Chess_Tile*>& somePossibleMoves);
	void SendPieceSelectedEvent(const Chess_RankAndFile& aRankAndFile);
	void SendMovePieceRequestEvent(const Chess_RankAndFile& aFromPosition, const Chess_RankAndFile& aToPosition, const bool aShouldEndturn, const Event_Source anEventSource);
	void SendRemovePieceRequestEvent(const Chess_RankAndFile& aRankAndFile, const Event_Source anEventSource);
	void SendReplacePieceRequestEvent(const Chess_RankAndFile& aRankAndFile, const Chess_Pieces_Colour aColour, const Chess_Pieces_EnumType aFromPieceType, const Chess_Pieces_EnumType aToPieceType);
	void SendGameOverEvent(const Chess_GameOverResult aGameOverResult);

private:
	std::vector<Event_Listener_PieceSelected*> myPieceSelectedListeners;
	std::vector<Event_Listener_EvaluatedPossibleMoves*> myEvaluatedMovesListeners;
	std::vector<Event_Listener_MovePieceRequest*> myMovePieceRequestListeners;
	std::vector<Event_Listener_RemovePieceRequest*> myRemovePieceRequestListeners;
	std::vector<Event_Listener_ReplacePieceRequest*> myReplacePieceRequestListeners;
	std::vector<Event_Listener_GameOver*> myGameOverListeners;;

	static Event_Handler* myInstance;
};