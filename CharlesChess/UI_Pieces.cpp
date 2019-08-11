#include "UI_Pieces.h"

UI_Pieces::UI_Pieces()
{
	LoadTextures();
}

UI_Pieces::~UI_Pieces()
{
}

void UI_Pieces::LoadTextures()
{
	sf::Texture newTexture;
	if (newTexture.loadFromFile("BLACK_BISHOP.png")) myBlackPieceTextures[Chess_Pieces_EnumType::BISHOP] = newTexture;
	if (newTexture.loadFromFile("BLACK_KING.png")) myBlackPieceTextures[Chess_Pieces_EnumType::KING] = newTexture;
	if (newTexture.loadFromFile("BLACK_KNIGHT.png")) myBlackPieceTextures[Chess_Pieces_EnumType::KNIGHT] = newTexture;
	if (newTexture.loadFromFile("BLACK_PAWN.png")) myBlackPieceTextures[Chess_Pieces_EnumType::PAWN] = newTexture;
	if (newTexture.loadFromFile("BLACK_QUEEN.png")) myBlackPieceTextures[Chess_Pieces_EnumType::QUEEN] = newTexture;
	if (newTexture.loadFromFile("BLACK_ROOK.png")) myBlackPieceTextures[Chess_Pieces_EnumType::ROOK] = newTexture;

	if (newTexture.loadFromFile("WHITE_BISHOP.png")) myWhitePieceTextures[Chess_Pieces_EnumType::BISHOP] = newTexture;
	if (newTexture.loadFromFile("WHITE_KING.png")) myWhitePieceTextures[Chess_Pieces_EnumType::KING] = newTexture;
	if (newTexture.loadFromFile("WHITE_KNIGHT.png")) myWhitePieceTextures[Chess_Pieces_EnumType::KNIGHT] = newTexture;
	if (newTexture.loadFromFile("WHITE_PAWN.png")) myWhitePieceTextures[Chess_Pieces_EnumType::PAWN] = newTexture;
	if (newTexture.loadFromFile("WHITE_QUEEN.png")) myWhitePieceTextures[Chess_Pieces_EnumType::QUEEN] = newTexture;
	if (newTexture.loadFromFile("WHITE_ROOK.png")) myWhitePieceTextures[Chess_Pieces_EnumType::ROOK] = newTexture;
}

sf::Texture* UI_Pieces::GetPieceTexture(Chess_Pieces_EnumType aPieceType, Chess_Pieces_Colour aColour)
{
	UI_PieceTextureMap& textureMap = aColour == Chess_Pieces_Colour::WHITE ? myWhitePieceTextures : myBlackPieceTextures;

	if (textureMap.find(aPieceType) != textureMap.end())
	{
		return &(textureMap.at(aPieceType));
	}

	return nullptr;
}
