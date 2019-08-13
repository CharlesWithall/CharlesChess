#pragma once

#include <map>

#include "UI_Model.h"

#include "Chess_Defines.h"

#include "SFML/Graphics.hpp"

typedef std::map<Chess_Pieces_EnumType, sf::Texture> UI_PieceTextureMap;

class UI_Pieces
{
public:
	UI_Pieces();
	~UI_Pieces();

	sf::Texture* GetPieceTexture(Chess_Pieces_EnumType aPieceType, Chess_Pieces_Colour aColour);
	void LoadTextures();

private:
	UI_PieceTextureMap myBlackPieceTextures;
	UI_PieceTextureMap myWhitePieceTextures;
};

class UI_Piece
{
public:
	UI_Piece(const Chess_Pieces_EnumType aChessPiece, const Chess_Pieces_Colour aColour)
		: myColour(aColour)
	{
		if (sf::Texture* pieceTexture = UI_Model::GetInstance()->GetPieces()->GetPieceTexture(aChessPiece, aColour))
		{
			mySprite = sf::Sprite(*pieceTexture);
		}
	}

	sf::Sprite mySprite;
	const Chess_Pieces_Colour myColour;
};

