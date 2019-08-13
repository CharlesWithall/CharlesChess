#pragma once

#include "Chess_Defines.h"

#include "SFML/Graphics.hpp"

class UI_Piece;

class UI_Tile
{
public:
	UI_Tile(const Chess_Pieces_Colour aColour, const Chess_RankAndFile& aRankAndFile);
	~UI_Tile();

	sf::RectangleShape& GetBackgroundTile() { return myTile; }
	
	UI_Piece* GetPiece() { return myPiece; }
	const Chess_RankAndFile& GetRankAndFile() const { return myRankAndFile; }
	void InitPiece(Chess_Pieces_EnumType aChessPieceType, Chess_Pieces_Colour aColour);
	void SetPiece(UI_Piece* const aPiece);
	
private:

	Chess_Pieces_Colour myColour;
	sf::RectangleShape myTile;
	UI_Piece* myPiece;
	sf::Vector2f myPosition;
	Chess_RankAndFile myRankAndFile;
};

