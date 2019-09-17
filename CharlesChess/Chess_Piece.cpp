#include "Chess_Piece.h"

#include "Chess_Bishop.h"
#include "Chess_King.h"
#include "Chess_Knight.h"
#include "Chess_Pawn.h"
#include "Chess_Queen.h"
#include "Chess_Rook.h"

#include "Chess_Check_Rule.h"
#include "Chess_Rule.h"

Chess_Piece::~Chess_Piece()
{
	for (Chess_Rule* rule : myChessRules)
	{
		delete rule;
		rule = nullptr;
	}
}

Chess_Piece* Chess_Piece::CreatePiece(const Chess_Pieces_Colour aColour, const Chess_Pieces_EnumType aType)
{
	switch (aType)
	{
	case Chess_Pieces_EnumType::BISHOP:
		return new Chess_Bishop(aColour);
	case Chess_Pieces_EnumType::KING:
		return new Chess_King(aColour);
	case Chess_Pieces_EnumType::KNIGHT:
		return new Chess_Knight(aColour);
	case Chess_Pieces_EnumType::PAWN:
		return new Chess_Pawn(aColour);
	case Chess_Pieces_EnumType::QUEEN:
		return new Chess_Queen(aColour);
	case Chess_Pieces_EnumType::ROOK:
		return new Chess_Rook(aColour);
	}

	return nullptr;
}

#include <typeinfo>
const std::vector<Chess_Tile*> Chess_Piece::EvaluateMoves(const Chess_Board* const aChessBoard, const Chess_Source aChessSource) const
{
	std::vector<Chess_Tile*> outMoves;

	for (Chess_Rule* rule : myChessRules)
	{
		const bool shouldEvaluate = rule && (aChessSource != Chess_Source::CHECK_EVALUATION || rule->ShouldEvaluateForCheck());
		if (shouldEvaluate)
		{
			const std::vector<Chess_Tile*>& possibleMoves = rule->Evaluate(myTile, aChessBoard);
			outMoves.insert(outMoves.end(), possibleMoves.begin(), possibleMoves.end());
		}
	}

	if (aChessSource == Chess_Source::MOVE_REQUEST || aChessSource == Chess_Source::GAME_OVER_EVALUATION)
	{
		for (int i = (int)outMoves.size() - 1; i >= 0; --i)
		{
			Chess_Tile* move = outMoves[i];
			if (myCheckRuleHandler.EvaluateTheoretical(aChessBoard, myColour, move, myTile))
			{
				outMoves.erase(outMoves.begin() + i);
			}
		}
	}

	return outMoves;
}