#include "AI_Evaluate.h"

#include "AI_Analysis.h"
#include "AI_Defines.h"

#include "Chess_Board.h"
#include "Chess_Model.h"
#include "Chess_Move.h"
#include "Chess_Piece.h"
#include "Chess_Tile.h"

#include "Debug_Profiler.h"

AI_Evaluate::AI_Evaluate(const Chess_Pieces_Colour aMaximizingcolour)
	: myMaximizingColour(aMaximizingcolour)
{
	myBoardAnalyser = new AI_Analysis(aMaximizingcolour);
}


AI_Evaluate::~AI_Evaluate()
{
}

void AI_Evaluate::EvaluateBestMoves(Chess_Board* const aChessBoard)
{
	myBestMoves.clear();
	mySearchDepth = 3;// theMinimumEvaluationDepth;// GetVariableDepthSearch(aChessBoard);
	Evaluate(aChessBoard, aChessBoard->GetActivePlayer(), mySearchDepth, -theMaxFloatValue, theMaxFloatValue);
	Debug_Profiler::QueryAverageTimes();
	Debug_Profiler::QueryTotalTimes();
}

// Check Mate ignored sometimes?!
// castling  not workinh

const float AI_Evaluate::Evaluate(Chess_Board* const aChessBoard, const Chess_Pieces_Colour aMaximizingColour, const int aSearchIndex, float anAlphaPrune, float aBetaPrune)
{
	if (aSearchIndex == 0)
	{
		return myBoardAnalyser->Analyse(aChessBoard);
	}
	//else if () //isCheckMate
	//{
	//	if (aMaximizingColour == myMaximizingColour)
	//	{
	//		return 1000.f;
	//	}
	//	else
	//	{
	//		return -1000.f;
	//	}
	//}
	//else if (true) // isStalemate (add to Chess_Board copy constructor)
	//{
	//	return 0.0f;
	//}
	else if (aMaximizingColour == myMaximizingColour)
	{
		float value = -theMaxFloatValue;
		const Chess_Pieces_Colour evalMaximizeColour = aMaximizingColour == Chess_Pieces_Colour::WHITE ? Chess_Pieces_Colour::BLACK : Chess_Pieces_Colour::WHITE;
		const std::vector<Chess_Move_Simple>& possibleMoves = aChessBoard->EvaluateAllPossibleMoves(aMaximizingColour);
		for (const Chess_Move_Simple& move : possibleMoves)
		{
			// Evaluate Move
			aChessBoard->MovePiece(aChessBoard->GetTile(move.myToTile), aChessBoard->GetTile(move.myFromTile), Event_Source::EVALUATION, true);
			const float evaluation = Evaluate(aChessBoard, evalMaximizeColour, aSearchIndex - 1, anAlphaPrune, aBetaPrune);
			aChessBoard->TakeBackLastMove(Event_Source::EVALUATION);

			// Add Best Moves To List
			if (evaluation > value)
			{
				const int oldValue = value;
				value = evaluation;
				if (aSearchIndex == mySearchDepth)
				{
					if (evaluation != oldValue)
					{
						myBestMoves.clear();
					}
					
					myBestMoves.push_back(move);
				}
			}

			// Do Pruning
			anAlphaPrune = std::max(anAlphaPrune, evaluation);
			if (anAlphaPrune >= aBetaPrune)
			{
				break;
			}
		}
		
		return value;
	}
	else // if (aMaximizingColour != myMaximizingColour)
	{
		float value = theMaxFloatValue;
		const Chess_Pieces_Colour evalMaximizeColour = aMaximizingColour == Chess_Pieces_Colour::WHITE ? Chess_Pieces_Colour::BLACK : Chess_Pieces_Colour::WHITE;
		const std::vector<Chess_Move_Simple>& possibleMoves = aChessBoard->EvaluateAllPossibleMoves(aMaximizingColour);
		for (const Chess_Move_Simple& move : possibleMoves)
		{
			// Evaluate Move
			aChessBoard->MovePiece(aChessBoard->GetTile(move.myToTile), aChessBoard->GetTile(move.myFromTile), Event_Source::EVALUATION, true);
			value = std::min(value, Evaluate(aChessBoard, evalMaximizeColour, aSearchIndex - 1, anAlphaPrune, aBetaPrune));
			aChessBoard->TakeBackLastMove(Event_Source::EVALUATION);

			// Do Pruning
			aBetaPrune = std::min(aBetaPrune, value);
			if (anAlphaPrune >= aBetaPrune)
			{
				break;
			}
		}

		return value;
	}
}

const int AI_Evaluate::GetVariableDepthSearch(const Chess_Board* const aChessBoard) const
{
	const int numberOfPieces = aChessBoard->GetWhitePieces().size() + aChessBoard->GetBlackPieces().size();
	static const int numberOfPiecesAtStart = 32;
	
	if (numberOfPieces >= 16)
		return 2;
	else if (numberOfPieces >= 8)
		return 4;
	else if (numberOfPieces >= 6)
		return 8;
	else if (numberOfPieces >= 4)
		return 10;
	else
		return 12;
}
