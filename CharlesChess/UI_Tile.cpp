#include "UI_Tile.h"

#include "UI_Defines.h"
#include "UI_Pieces.h"
#include "UI_Model.h"

UI_Tile::UI_Tile(const Chess_Pieces_Colour aColour, const Chess_RankAndFile& aRankAndFile)
	: myColour(aColour)
	, myPiece(nullptr)
	, myRankAndFile(aRankAndFile)
{
	myTile = sf::RectangleShape(theNativeTileSize);
	myPosition = sf::Vector2f((aRankAndFile.myFile) * 100.f, (7 - aRankAndFile.myRank) * 100.f);
	myTile.setPosition(myPosition);
	sf::Color colour = aColour == Chess_Pieces_Colour::WHITE ? theWhiteTileColour : theBlackTileColour;
	myTile.setFillColor(colour);
}

UI_Tile::~UI_Tile()
{
}

void UI_Tile::InitPiece(Chess_Pieces_EnumType aChessPieceType, Chess_Pieces_Colour aColour)
{
	myPiece = new UI_Piece(aChessPieceType, aColour);
	myPiece->mySprite.setPosition(myPosition);
}

void UI_Tile::SetPiece(UI_Piece* const aPiece)
{
	myPiece = aPiece;

	if (myPiece)
	{
		myPiece->mySprite.setPosition(myPosition);
	}	
}
