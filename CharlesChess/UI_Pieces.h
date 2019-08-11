#pragma once

#include <map>

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

