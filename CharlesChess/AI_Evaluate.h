#pragma once

#include <array>

#include "AI_Model.h"

#include "Debug_Readout.h"

class AI_Analysis;
class Chess_Board;
class Chess_Check_Rule;
struct Chess_Move_Simple;

class AI_Evaluate
{
public:
	AI_Evaluate(const Chess_Pieces_Colour aMaximizingColour);
	~AI_Evaluate();

	void EvaluateBestMoves(Chess_Board* const aChessBoard);
	const std::vector<Chess_Move_Simple>& GetBestMoves() const { return myBestMoves; }
	float RequestStaticAnalysis(Chess_Board* const aChessBoard, const Chess_Pieces_Colour aPlayerTurnColour) const;
	void SetOverrideSearchDepth(const int aDepth) { mySearchDepthOverride = aDepth; }
private:
	const float Evaluate(
		Chess_Board* const aChessBoard,
		const Chess_Pieces_Colour aMaximizingColour,
		const int aSearchIndex,
		float anAlphaPrune,
		float aBetaPrune);

	const int GetVariableDepthSearch(const Chess_Board* const aChessBoard) const;
	const bool ShouldAddMoveToBestMoveList(const float anEvaluation, const float aCurrentBest, const Chess_Move_Simple& aMove, const Chess_Board* const aChessBoard, const Chess_Pieces_Colour aMaximizingDirection) const;
	void SortPossibleMoves(std::vector<Chess_Move_Simple>& somePossibleMoves, const Chess_Board* const aChessBoard) const;

#if DEBUG_ENABLED_AI_INTENSIVE
	void DebugSteppedEvaluationPrintOut(const Chess_Move_Simple& aMove, const int aSearchIndex, const float anEvaluation);
#endif

	Chess_Pieces_Colour myMaximizingColour;
	AI_Analysis* myBoardAnalyser;
	std::vector<Chess_Move_Simple> myBestMoves;
	Chess_Check_Rule* myCheckRuleHandler;
	int mySearchDepth = 0;
	int mySearchDepthOverride = 0;
};

