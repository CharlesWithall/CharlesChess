#include "UI_Tile.h"

#include "UI_Pieces.h"
#include "UI_Model.h"

const sf::Color locWhiteTile = sf::Color(240, 217, 181);
const sf::Color locBlackTile = sf::Color(181, 136, 99);

UI_Tile::UI_Tile(const Chess_Pieces_Colour aColour, const Chess_File aFile, const int aRank)
	: myColour(aColour)
	, myPiece(nullptr)
	, myFile(aFile)
	, myRank(aRank)
{
	myTile = sf::RectangleShape(sf::Vector2f(100.f, 100.f));
	myPosition = sf::Vector2f((aFile - 1) * 100.f, (8 - aRank) * 100.f);
	myTile.setPosition(myPosition);
	sf::Color colour = aColour == Chess_Pieces_Colour::WHITE ? locWhiteTile : locBlackTile;
	myTile.setFillColor(colour);
}

UI_Tile::~UI_Tile()
{
}

void UI_Tile::InitPiece(Chess_Pieces_EnumType aChessPieceType, Chess_Pieces_Colour aColour)
{
	if (sf::Texture* pieceTexture = UI_Model::GetInstance()->GetPieces()->GetPieceTexture(aChessPieceType, aColour))
	{
		myPiece = new sf::Sprite(*pieceTexture);
		myPiece->setPosition(myPosition);
	}	
}
