#pragma once
class AI_EndGame
{
public:
	AI_EndGame();
	~AI_EndGame();

private:
	// End game condition - points on board (900 + 900 + 90 + 10 + 50 + 50 + 50 + 30 = 2080)
	// New tables
	// Looking to reduce number of moves for opponent

	// Do analysis of improvement times using alpha-beta pruning.
	// Between about level 5 and 6 stockfish approx. 1800 ELO rating
	// https://en.wikipedia.org/wiki/Endgame_tablebase
};

