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

Chess_Piece* Chess_Piece::CreatePiece(const Chess_Pieces_Colour aColour, const Chess_Pieces_EnumType aType, const int aPieceListIndex)
{
	switch (aType)
	{
	case Chess_Pieces_EnumType::BISHOP:
		return new Chess_Bishop(aColour, aPieceListIndex);
	case Chess_Pieces_EnumType::KING:
		return new Chess_King(aColour, aPieceListIndex);
	case Chess_Pieces_EnumType::KNIGHT:
		return new Chess_Knight(aColour, aPieceListIndex);
	case Chess_Pieces_EnumType::PAWN:
		return new Chess_Pawn(aColour, aPieceListIndex);
	case Chess_Pieces_EnumType::QUEEN:
		return new Chess_Queen(aColour, aPieceListIndex);
	case Chess_Pieces_EnumType::ROOK:
		return new Chess_Rook(aColour, aPieceListIndex);
	}

	return nullptr;
}

const std::vector<Chess_Tile*> Chess_Piece::EvaluateMoves(Chess_Board* const aChessBoard) const
{
	std::vector<Chess_Tile*> outMoves;

	for (Chess_Rule* rule : myChessRules)
	{
		const std::vector<Chess_Tile*>& possibleMoves = rule->Evaluate(myTile, aChessBoard);
		outMoves.insert(outMoves.end(), possibleMoves.begin(), possibleMoves.end());
	}

	for (int i = (int)outMoves.size() - 1; i >= 0; --i)
	{
		Chess_Tile* move = outMoves[i];

		if (myCheckRuleHandler.EvaluateTheoretical(aChessBoard, myColour, move, myTile))
		{
			outMoves.erase(outMoves.begin() + i);
		}
	}

	return outMoves;
}

void Chess_Piece::SetHasMoved(const int aMoveReferenceNumber)
{
	if (!myHasMoved)
	{
		myFirstMoveReferenceNumber = aMoveReferenceNumber;
		myHasMoved = true;
	}
}

void Chess_Piece::ResetHasMoved(const int aMoveReferenceNumber)
{
	if (aMoveReferenceNumber == myFirstMoveReferenceNumber)
	{
		myFirstMoveReferenceNumber = 0;
		myHasMoved = false;
	}
}