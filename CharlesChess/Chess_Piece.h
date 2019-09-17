#pragma once

#include "Chess_Check_Rule.h"
#include "Chess_Defines.h"

#include <string>
#include <vector>

class Chess_Rule;
class Chess_Tile;

class Chess_Piece
{
public:
	Chess_Piece(Chess_Pieces_Colour aColour) : myColour(aColour), myHasMoved(false), myCheckRuleHandler(Chess_Check_Rule()) {};
	~Chess_Piece();

	static Chess_Piece* CreatePiece(const Chess_Pieces_Colour aColour, const Chess_Pieces_EnumType aType);

	virtual char GetConsoleIdentifier() const = 0;
	virtual std::string GetName() const = 0;
	virtual Chess_Pieces_EnumType GetType() const = 0;
	virtual int GetScore() const = 0;

	const Chess_Check_Rule& GetCheckRuleHandler() const { return myCheckRuleHandler; }
	Chess_Pieces_Colour GetColour() const { return myColour; }
	bool GetHasMoved() const { return myHasMoved; }
	void SetHasMoved() { myHasMoved = true; }

	const Chess_Tile* const GetTile() const { return myTile; }
	void SetTile(Chess_Tile* const aTile) { myTile = aTile; }

	const std::vector<Chess_Tile*> EvaluateMoves(const Chess_Board* const aChessBoard, const Chess_Source aChessSource) const;

protected:
	std::vector<Chess_Rule*> myChessRules;
	Chess_Tile* myTile;
	Chess_Pieces_Colour myColour;
	Chess_Check_Rule myCheckRuleHandler;
	bool myHasMoved;
};

