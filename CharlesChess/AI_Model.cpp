#include "AI_Model.h"

#include "AI_Analysis.h"
#include "AI_Evaluate.h"

#include "Chess_Board.h"
#include "Chess_Model.h"
#include "Chess_Move.h"

#include "Event_Handler.h"

AI_Model* AI_Model::myInstance = nullptr;

AI_Model* AI_Model::GetInstance()
{
	if (myInstance == nullptr)	myInstance = new AI_Model();
	return myInstance;
}

AI_Model::AI_Model()
{
	myBoardEvaluator = new AI_Evaluate(Chess_Model::GetInstance()->GetChessBoard()->GetActivePlayer());
}

AI_Model::~AI_Model()
{
	delete myBoardEvaluator;
}

void AI_Model::RequestAndMakeMove() const
{
	myBoardEvaluator->EvaluateBestMoves(Chess_Model::GetInstance()->GetChessBoard());
	const std::vector<Chess_Move_Simple>& bestMoves = myBoardEvaluator->GetBestMoves();
	const int moveSelectionIndex = std::rand() % bestMoves.size();
	const Chess_Move_Simple& selectedMove = bestMoves[moveSelectionIndex];

	Event_Handler::GetInstance()->SendMovePieceRequestEvent(selectedMove.myFromTile, selectedMove.myToTile, true, Event_Source::DEFAULT);
}
