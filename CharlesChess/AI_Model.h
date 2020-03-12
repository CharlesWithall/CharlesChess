#pragma once

#include "Event_Listeners.h"

class AI_Evaluate;
class AI_Openings;

class Chess_Board;
class Chess_Tile;

class AI_Model
{
public:
	static AI_Model* GetInstance();

	float RequestStaticAnalysis(Chess_Board* const aChessBoard, const Chess_Pieces_Colour aPlayerTurnColour) const;
	void RequestAndMakeMove() const;
	void SetOverrideSearchDepth(const int aDepth);

	AI_Openings* GetOpenings() { return myOpenings; }
private:
	AI_Model();
	~AI_Model();

	AI_Evaluate* myBoardEvaluator;
	AI_Openings* myOpenings;

	static AI_Model* myInstance;
};

