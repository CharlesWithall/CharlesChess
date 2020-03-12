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
class Event_Listener_PawnPromotionRequest;

class Event_Handler
{
public:

	static Event_Handler* GetInstance();

	// Registration
	void RegisterPieceSelectedListener(Event_Listener_PieceSelected* aListener) { myPieceSelectedListeners.push_back(aListener); }
	void RegisterEvaluatedPossibleMovesListener(Event_Listener_EvaluatedPossibleMoves* aListener) { myEvaluatedMovesListeners.push_back(aListener); }
	void RegisterMovePieceRequestListener(Event_Listener_MovePieceRequest* aListener) { myMovePieceRequestListeners.push_back(aListener); }
	void RegisterGameOverListener(Event_Listener_GameOver* aListener) { myGameOverListeners.push_back(aListener); }
	void RegisterPawnPromotionRequestListener(Event_Listener_PawnPromotionRequest* aListener) { myPawnPromotionRequestListeners.push_back(aListener); }

	void UnregisterPieceSelectedListener(Event_Listener_PieceSelected* aListener) { myPieceSelectedListeners.erase(std::remove(myPieceSelectedListeners.begin(), myPieceSelectedListeners.end(), aListener), myPieceSelectedListeners.end()); }
	void UnregisterEvaluatedPossibleMovesListener(Event_Listener_EvaluatedPossibleMoves* aListener) { myEvaluatedMovesListeners.erase(std::remove(myEvaluatedMovesListeners.begin(), myEvaluatedMovesListeners.end(), aListener), myEvaluatedMovesListeners.end()); }
	void UnregisterMovePieceRequestListener(Event_Listener_MovePieceRequest* aListener) { myMovePieceRequestListeners.erase(std::remove(myMovePieceRequestListeners.begin(), myMovePieceRequestListeners.end(), aListener), myMovePieceRequestListeners.end()); }
	void UnregisterGameOverListener(Event_Listener_GameOver* aListener) { myGameOverListeners.erase(std::remove(myGameOverListeners.begin(), myGameOverListeners.end(), aListener), myGameOverListeners.end()); }
	void UnregisterPawnPromotionRequestListener(Event_Listener_PawnPromotionRequest* aListener) { myPawnPromotionRequestListeners.erase(std::remove(myPawnPromotionRequestListeners.begin(), myPawnPromotionRequestListeners.end(), aListener), myPawnPromotionRequestListeners.end()); }
	
	// Send Events
	void SendEvaluatedPossibleMovesEvent(const std::vector<Chess_Tile*>& somePossibleMoves);
	void SendPieceSelectedEvent(const Chess_RankAndFile& aRankAndFile);
	void SendMovePieceRequestEvent(const Chess_RankAndFile& aFromPosition, const Chess_RankAndFile& aToPosition);
	void SendGameOverEvent(const Chess_GameOverResult aGameOverResult);
	void SendPawnPromotionRequestEvent(const Chess_RankAndFile& aFromPosition, const Chess_RankAndFile& aToPosition, const Chess_Pieces_EnumType aPromotionPiece);

private:
	std::vector<Event_Listener_PieceSelected*> myPieceSelectedListeners;
	std::vector<Event_Listener_EvaluatedPossibleMoves*> myEvaluatedMovesListeners;
	std::vector<Event_Listener_MovePieceRequest*> myMovePieceRequestListeners;
	std::vector<Event_Listener_GameOver*> myGameOverListeners;
	std::vector<Event_Listener_PawnPromotionRequest*> myPawnPromotionRequestListeners;

	static Event_Handler* myInstance;
};