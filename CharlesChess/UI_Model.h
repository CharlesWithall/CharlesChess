#pragma once

#include "Chess_Model.h"

#include "UI_Board.h"
#include "UI_MainMenu.h"
#include "UI_PawnPromotion.h"

#include "Event_Listeners.h"

class UI_Pieces;

class UI_Model
	: Event_Listener_ReplacePieceRequest
	, Event_Listener_GameOver
{
public:

	static UI_Model* GetInstance();

	UI_Pieces* const GetPieces() const { return myPieces; }

	const Chess_Pieces_Colour GetTurn() const { return myTurn; }
	void SetTurnTaken() { myTurn = myTurn == Chess_Pieces_Colour::WHITE ? Chess_Pieces_Colour::BLACK : Chess_Pieces_Colour::WHITE; }
	void Run();

	void OnReplacePieceRequested(const Event_ReplacePieceRequest& anEvent) override;
	void OnGameOver(const Event_GameOver& anEvent) override;

	void RequestCloseMenu() { myShouldCloseMenu = true; }
	void RequestReset() { myShouldReset = true; }

	const bool GetIsPlayingAgainstComputer() const { return myIsPlayingAgainstComputer; }
	void SetIsPlayingAgainstComputer(const bool aIsComputer) { myIsPlayingAgainstComputer = aIsComputer; }

	const UI_MainMenu_NewGameColour GetNewGameColour() const { return myNewGameColour; }
	void SetNewGameColour(const UI_MainMenu_NewGameColour aNewGameColour) { myNewGameColour = aNewGameColour; }

	const bool IsMenuOpen() { return myMainMenu || myPawnPromotion; }

	const Chess_Pieces_Colour GetMyAIColour() const { return myAIColour; }
	void SetAIColourFromSelection();
private:
	UI_Model();
	~UI_Model();

	void CloseMenu() { delete myMainMenu; myMainMenu = nullptr; myShouldCloseMenu = false; }
	void HandleEscapeKeyPress();
	void Reset();

	UI_Board* myUIBoard;
	UI_Pieces* myPieces;
	Chess_Pieces_Colour myTurn;
	UI_PawnPromotion* myPawnPromotion;
	UI_MainMenu* myMainMenu;

	static UI_Model* myInstance;

	bool myShouldReset;
	bool myShouldCloseMenu;

	bool myIsPlayingAgainstComputer;
	Chess_Pieces_Colour myAIColour;
	UI_MainMenu_NewGameColour myNewGameColour;
};

