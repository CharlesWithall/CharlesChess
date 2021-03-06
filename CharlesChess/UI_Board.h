#pragma once

#include "SFML/Graphics.hpp"

#include <array>

#include "Event_Listeners.h"

class Chess_Board;

class UI_Highlight_PossibleMove;
class UI_Highlight_PreviousMove;
class UI_Listener_Keyboard;
class UI_Listener_MouseClick;
class UI_Tile;

typedef std::array<UI_Tile*, 8> UI_RankArray;
typedef std::array<UI_RankArray, 8> UI_8x82DArray;

class UI_Board
	: Event_Listener_EvaluatedPossibleMoves
{
public:
	~UI_Board();

	void Init(Chess_Board* aChessBoard);
	void Update(sf::RenderWindow& aWindow);

	void ClearPossibleMoves();

	UI_Tile* GetTile(const Chess_RankAndFile& aRankAndFile) const { return myChessTiles[aRankAndFile.myFile][aRankAndFile.myRank]; };
	UI_Tile* GetSelectedTile() { return mySelectedTile; }
	const UI_8x82DArray& GetTiles() { return myChessTiles; }
	const std::vector<UI_Tile*>& GetPossibleMoves() const { return myPossibleMoves; }

	void Resync();
	void SetSelectedTile(UI_Tile* aSelectedTile);

private:
	void Draw(sf::RenderWindow& aWindow);

	void OnMovesEvaluated(const Event_EvaulatedPossibleMoves& anEvent) override;

	UI_8x82DArray myChessTiles; // Each array of files contains an array of ranks
	
	UI_Tile* mySelectedTile;
	sf::RectangleShape* mySelectedHighlight;

	std::vector<UI_Tile*> myPossibleMoves;
	std::vector<UI_Highlight_PossibleMove*> myPossibleMoveHighlights;
	std::array<UI_Highlight_PreviousMove*, 2> myPreviousMoveHighlights;

	UI_Listener_MouseClick* myMouseClickListener;
	UI_Listener_Keyboard* myKeyboardListener;
};

