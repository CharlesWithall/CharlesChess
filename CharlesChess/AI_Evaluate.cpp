#include "AI_Evaluate.h"

#include "AI_Analysis.h"
#include "AI_Defines.h"
#include "AI_Model.h"
#include "AI_Openings.h"

#include "Chess_Board.h"
#include "Chess_Check_Rule.h"
#include "Chess_Model.h"
#include "Chess_Move.h"
#include "Chess_Piece.h"
#include "Chess_Tile.h"

#include "Debug_Profiler.h"

AI_Evaluate::AI_Evaluate(const Chess_Pieces_Colour aMaximizingColour)
	: myMaximizingColour(aMaximizingColour)
	, mySearchDepthOverride(0)
{
	myBoardAnalyser = new AI_Analysis(aMaximizingColour);
	myCheckRuleHandler = new Chess_Check_Rule();
}

AI_Evaluate::~AI_Evaluate()
{
	delete myBoardAnalyser;
	delete myCheckRuleHandler;
}

void AI_Evaluate::EvaluateBestMoves(Chess_Board* const aChessBoard)
{
	myBestMoves.clear();

	// Early Game
	Chess_Move_Simple openingMove;
	if (AI_Model::GetInstance()->GetOpenings()->SelectMoveFromOpeningCache(aChessBoard, openingMove))
	{
		myBestMoves.push_back(openingMove);
		return;
	}

	// End Game


	// Middle Game
	mySearchDepth = GetVariableDepthSearch(aChessBoard);

	DEBUG_PRINT("Executing Minimax with Search Depth: %d", mySearchDepth);

	Evaluate(aChessBoard, aChessBoard->GetActivePlayer(), mySearchDepth, -theMaxFloatValue * theMaxFloatValue, theMaxFloatValue * theMaxFloatValue);
	for (auto& move : myBestMoves)
	{
		DEBUG_PRINT("%s to %s is considered a Best Move", move.myFromTile.GetReadableName().c_str(), move.myToTile.GetReadableName().c_str());
	}
}

// OTHER MINIMAX OPTIMIZATIONS
// ADD OPENINGS
// GETS CAUGHT IN A LOOP - DOESN'T KNOW HOW TO END
// PAWN PROMOTION - ONLY DOES QUEENS RIGHT NOW
// IF HAS MASSIVE ADVANTAGE - SEARCH FOR A FORCED MATE

const float AI_Evaluate::Evaluate(Chess_Board* const aChessBoard, const Chess_Pieces_Colour aMaximizingColour, const int aSearchIndex, float anAlphaPrune, float aBetaPrune)
{
	if (aSearchIndex == 0)
	{
		return myBoardAnalyser->Analyse(aChessBoard, aMaximizingColour);
	}
	else
	{
		std::vector<Chess_Move_Simple> possibleMoves = aChessBoard->EvaluateAllPossibleMoves(aMaximizingColour);

		if (possibleMoves.size() == 0)
		{
			const bool isCheckMate = myCheckRuleHandler->Evaluate(aChessBoard, aMaximizingColour);
			const int multiplier = aMaximizingColour == Chess_Pieces_Colour::WHITE ? 1 : -1;
			const float endGameReturn = (myCheckRuleHandler->Evaluate(aChessBoard, aMaximizingColour) ? -theMaxFloatValue : 0.f) * aSearchIndex * multiplier;
			DEBUG_PRINT_AI("End Game Found - %s - %s - Value: %.f", aMaximizingColour == Chess_Pieces_Colour::WHITE ? "WHITE" : "BLACK", isCheckMate ? "Check Mates" : "Stale Mates", endGameReturn);
			return endGameReturn;
		}

		SortPossibleMoves(possibleMoves, aChessBoard);

		if (aMaximizingColour == Chess_Pieces_Colour::WHITE)
		{
			for (const Chess_Move_Simple& move : possibleMoves)
			{
				aChessBoard->PerformMovePieceRequest(aChessBoard->GetTile(move.myFromTile), aChessBoard->GetTile(move.myToTile), false);
				const float evaluation = Evaluate(aChessBoard, Chess_Pieces_Colour::BLACK, aSearchIndex - 1, anAlphaPrune, aBetaPrune);
				aChessBoard->OnTakeBackPieceRequested();

				if (evaluation >= aBetaPrune)
				{
					if (aSearchIndex == mySearchDepth)
					{
						myBestMoves.clear();
						myBestMoves.push_back(move);
					}
					return aBetaPrune;
				}

				if (evaluation > anAlphaPrune)
				{
					if (aSearchIndex == mySearchDepth)
					{
						myBestMoves.clear();
						myBestMoves.push_back(move);
					}
					anAlphaPrune = evaluation;
				}
			}

			return anAlphaPrune;
		}
		else
		{
			for (const Chess_Move_Simple& move : possibleMoves)
			{
				aChessBoard->PerformMovePieceRequest(aChessBoard->GetTile(move.myFromTile), aChessBoard->GetTile(move.myToTile), false);
				const float evaluation = Evaluate(aChessBoard, Chess_Pieces_Colour::WHITE, aSearchIndex - 1, anAlphaPrune, aBetaPrune);
				aChessBoard->OnTakeBackPieceRequested();

				if (evaluation <= anAlphaPrune)
				{
					if (aSearchIndex == mySearchDepth)
					{
						myBestMoves.clear();
						myBestMoves.push_back(move);
					}
					return anAlphaPrune;
				}

				if (evaluation < aBetaPrune)
				{
					if (aSearchIndex == mySearchDepth)
					{
						myBestMoves.clear();
						myBestMoves.push_back(move);
					}
					aBetaPrune = evaluation;
				}
			}

			return aBetaPrune;
		}
	}
}

const int AI_Evaluate::GetVariableDepthSearch(const Chess_Board* const aChessBoard) const
{
	if (mySearchDepthOverride > 0)
	{
		return mySearchDepthOverride;
	}

	const int defaultDepth = 5;

	const Chess_Pieces_Colour playerColour = aChessBoard->GetActivePlayer();
	const std::array<Chess_Piece*, 16>& pieces = playerColour == Chess_Pieces_Colour::WHITE ? aChessBoard->GetWhitePieces() : aChessBoard->GetBlackPieces();
	int numberOfNonPawnOrKingPieces = 0;

	for (Chess_Piece* piece : pieces)
	{
		if (!piece)
			continue;

		Chess_Pieces_EnumType type = piece->GetType();
		if (type != Chess_Pieces_EnumType::PAWN && type != Chess_Pieces_EnumType::KING)
		{
			numberOfNonPawnOrKingPieces++;
		}
	}

	if (numberOfNonPawnOrKingPieces == 0)
	{
		return defaultDepth + 2;
	}
	else if (numberOfNonPawnOrKingPieces <= 1)
	{
		return defaultDepth + 1;
	}

	return defaultDepth;
}

float AI_Evaluate::RequestStaticAnalysis(Chess_Board* const aChessBoard, const Chess_Pieces_Colour aPlayerTurnColour) const
{
	return myBoardAnalyser->Analyse(aChessBoard, aPlayerTurnColour);
}

const bool AI_Evaluate::ShouldAddMoveToBestMoveList(const float anEvaluation, const float aCurrentBest, const Chess_Move_Simple& aMove, const Chess_Board* const aChessBoard, const Chess_Pieces_Colour aMaximizingDirection) const
{
	const bool maximizingForWhite = aMaximizingDirection == Chess_Pieces_Colour::WHITE;
	const bool moveIsBetter = maximizingForWhite ? anEvaluation >= aCurrentBest : anEvaluation <= aCurrentBest;
	if (moveIsBetter)
	{
		// Discourage Early Queen Moves
		const Chess_Piece* const piece = aChessBoard->GetTile(aMove.myFromTile)->GetPiece();
		if (piece && piece->GetType() == Chess_Pieces_EnumType::QUEEN && !piece->GetHasMoved())
		{
			return maximizingForWhite ? anEvaluation - 0.75f >= aCurrentBest : anEvaluation + 0.75f >= aCurrentBest;
		}

		return true;
	}

	return false;
}

void AI_Evaluate::SortPossibleMoves(std::vector<Chess_Move_Simple>& possibleMoves, const Chess_Board* const aChessBoard) const
{
	// Taking a piece with equal or higher value is good
	int bestMove = 0;
	int bestMoveScore = -1;

	for (unsigned int i = 0; i < possibleMoves.size(); ++i)
	{
		const Chess_Piece* const takingPiece = aChessBoard->GetTile(possibleMoves[i].myFromTile)->GetPiece();
		const Chess_Piece* const takenPiece = aChessBoard->GetTile(possibleMoves[i].myToTile)->GetPiece();

		if (takingPiece && takenPiece)
		{
			if (takenPiece->GetScore() >= takingPiece->GetScore())
			{
				bestMoveScore = takenPiece->GetScore() - takingPiece->GetScore();
				bestMove = i;
			}
		}
	}

	if (bestMove > 0)
	{
		std::rotate(possibleMoves.begin(), possibleMoves.begin() + bestMove, possibleMoves.end());
		DEBUG_PRINT_AI_INTENSIVE("Sort Possible Moves First Move Is Now %s to %s", possibleMoves[0].myFromTile.GetReadableName().c_str(), possibleMoves[0].myToTile.GetReadableName().c_str());
	}
}

#if DEBUG_ENABLED_AI_INTENSIVE
void AI_Evaluate::DebugSteppedEvaluationPrintOut(const Chess_Move_Simple& aMove, const int aSearchIndex, const float anEvaluation)
{
	std::string spacing = "";

	for (int i = aSearchIndex; i < mySearchDepth; ++i)
	{
		spacing.append("-----");
	}

	DEBUG_PRINT_AI("%s moving from %s to %s evaluates to %.2f", spacing.c_str(), aMove.myFromTile.GetReadableName().c_str(), aMove.myToTile.GetReadableName().c_str(), anEvaluation);
}
#endif