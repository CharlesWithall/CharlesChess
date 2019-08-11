#pragma once

#include "Chess_Defines.h"

#include "SFML/Graphics.hpp"

class UI_Tile
{
public:
	UI_Tile(const Chess_Pieces_Colour aColour, const Chess_File aFile, const int aRank);
	~UI_Tile();

	sf::RectangleShape& GetBackgroundTile() { return myTile; }
	
	sf::Sprite* GetPiece() { return myPiece; }
	void InitPiece(Chess_Pieces_EnumType aChessPieceType, Chess_Pieces_Colour aColour);

	
private:

	Chess_Pieces_Colour myColour;
	sf::RectangleShape myTile;
	sf::Sprite* myPiece;
	sf::Vector2f myPosition;
	Chess_File myFile;
	int myRank;
};

