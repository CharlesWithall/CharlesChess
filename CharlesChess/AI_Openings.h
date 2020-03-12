#pragma once

#include <string>
#include <vector>

#include "Chess_Move.h"

class Chess_Board;

struct AI_Opening
{
	std::string myName;
	std::vector<std::string> myMoves;
};

class AI_Openings
{
public:
	AI_Openings();
	~AI_Openings();

	bool SelectMoveFromOpeningCache(const Chess_Board* aBoard, Chess_Move_Simple& aMoveOut) const;

private:
	void Init();

	Chess_Move_Simple ConvertNotationToChessMoveSimple(const std::string& someNotation, const Chess_Board* aBoard) const;
	std::string ConvertChessMoveToNotation(const Chess_Move& aMove, const Chess_Board* aBoard) const;

	std::vector<AI_Opening> myOpenings;
	unsigned int myMaxOpeningLength = 0;
};

