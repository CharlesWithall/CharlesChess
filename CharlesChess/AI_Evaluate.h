#pragma once


#include "AI_Model.h"

class AI_Analysis;
class Chess_Board;
struct Chess_Move_Simple;

class AI_Evaluate
{
public:
	AI_Evaluate(const Chess_Pieces_Colour aMaximizingcolour);
	~AI_Evaluate();

	void EvaluateBestMoves(const Chess_Board* const aChessBoard);
	const std::vector<Chess_Move_Simple>& GetBestMoves() const { return myBestMoves; }
private:
	const float Evaluate(
		const Chess_Board* const aChessBoard,
		const Chess_Pieces_Colour aMaximizingColour,
		const int aSearchIndex,
		float anAlphaPrune,
		float aBetaPrune);

	const int GetVariableDepthSearch(const Chess_Board* const aChessBoard) const;
	
	Chess_Pieces_Colour myMaximizingColour;
	AI_Analysis* myBoardAnalyser;
	std::vector<Chess_Move_Simple> myBestMoves;
	int mySearchDepth = 0;
};

