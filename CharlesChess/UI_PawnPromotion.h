#pragma once

#include "SFML/Graphics.hpp"

#include "Chess_Defines.h"

class UI_PawnPromotion
{
public:
	UI_PawnPromotion(const Chess_RankAndFile& aRankAndFile, const Chess_Pieces_Colour aColour);
	~UI_PawnPromotion();

	void Draw(sf::RenderWindow& aWindow);
private:
	void OnHoverOver(sf::RenderWindow& aWindow);
	void HandleMouseClick(const Chess_Pieces_EnumType aChessPieceType);

	sf::RectangleShape myBackground;

	sf::RectangleShape myQueenBackground;
	sf::RectangleShape myRookBackground;
	sf::RectangleShape myBishopBackground;
	sf::RectangleShape myKnightBackground;

	sf::Sprite myQueenIcon;
	sf::Sprite myRookIcon;
	sf::Sprite myBishopIcon;
	sf::Sprite myKnightIcon;

	Chess_Pieces_Colour myColour;
	Chess_RankAndFile myRankAndFile;

	sf::RectangleShape mySelectedHighlight;

	bool myButtonDownLatch;
	bool myButtonUpLatch;
};

