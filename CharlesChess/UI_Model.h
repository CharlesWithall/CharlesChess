#pragma once

#include "Chess_Model.h"

#include "UI_Board.h"

class UI_Pieces;

class UI_Model
{
public:

	static UI_Model* GetInstance();

	UI_Pieces* const GetPieces() const { return myPieces; }
	const Chess_Pieces_Colour GetTurn() const { return myTurn; }
	void SetTurnTaken() { myTurn = myTurn == Chess_Pieces_Colour::WHITE ? Chess_Pieces_Colour::BLACK : Chess_Pieces_Colour::WHITE; }
	void Run();

private:
	UI_Model();

	UI_Pieces* myPieces;
	Chess_Pieces_Colour myTurn;

	static UI_Model* myInstance;
};

