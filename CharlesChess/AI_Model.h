#pragma once

#include "Event_Listeners.h"

class AI_Evaluate;

class Chess_Board;
class Chess_Tile;

class AI_Model
{
public:
	static AI_Model* GetInstance();

	void RequestAndMakeMove() const;
private:
	AI_Model();
	~AI_Model();

	AI_Evaluate* myBoardEvaluator;

	static AI_Model* myInstance;
};

