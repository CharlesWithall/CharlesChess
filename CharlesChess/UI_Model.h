#pragma once

#include "Chess_Model.h"

#include "UI_Board.h"

class UI_Pieces;

class UI_Model
{
public:

	static UI_Model* GetInstance();

	UI_Pieces* const GetPieces() const { return myPieces; }
	void Run();

private:
	UI_Model();

	UI_Pieces* myPieces;

	static UI_Model* myInstance;
};

