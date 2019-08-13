#include "UI_Tile.h"

#include "UI_Pieces.h"
#include "UI_Model.h"

const sf::Color locWhiteTile = sf::Color(240, 217, 181);
const sf::Color locBlackTile = sf::Color(181, 136, 99);

UI_Tile::UI_Tile(const Chess_Pieces_Colour aColour, const Chess_RankAndFile& aRankAndFile)
	: myColour(aColour)
	, myPiece(nullptr)
	, myRankAndFile(aRankAndFile)
{
	myTile = sf::RectangleShape(sf::Vector2f(100.f, 100.f));
	myPosition = sf::Vector2f((aRankAndFile.myFile) * 100.f, (7 - aRankAndFile.myRank) * 100.f);
	myTile.setPosition(myPosition);
	sf::Color colour = aColour == Chess_Pieces_Colour::WHITE ? locWhiteTile : locBlackTile;
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
