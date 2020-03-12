#pragma once

#include "SFML/Graphics.hpp"

// SIZES
static const sf::Vector2f theNativeScreenSize = sf::Vector2f(800, 800);
static const sf::Vector2f theNativeTileSize = sf::Vector2f(100, 100);

// POSITIONS
static const sf::Vector2f theMainMenuTitleTextPosition = sf::Vector2f(400, 100);
static const sf::Vector2f theMainMenuFirstButtonPosition = sf::Vector2f(250, 200);
static const sf::Vector2f theMainMenuFirstButtonTextPosition = sf::Vector2f(400, 250);
static const int theMainMenuButtonSpacing = 110;

// COLOURS
static const sf::Color theWhiteTileColour = sf::Color(240, 217, 181);
static const sf::Color theBlackTileColour = sf::Color(181, 136, 99);
static const sf::Color theTileSelectedHighlightColour = sf::Color(60, 118, 67, 120);
static const sf::Color thePossibleMoveHighlightColour = sf::Color(40, 80, 44, 120);
static const sf::Color thePreviousMoveHighlightColour = sf::Color(205, 210, 106);
static const sf::Color theMenuBackgroundColour = sf::Color(255, 255, 255, 220);

static const sf::Color theMainMenuTitleTextColour = theBlackTileColour; // Change this depending on winner?!
static const sf::Color theMainMenuButtonTextColour = theWhiteTileColour;
static const sf::Color theMainMenuTextHighlight = sf::Color::Black;

// FONT SIZES
static const int theMainMenuTitleTextFontSize = static_cast<int>(theNativeScreenSize.x / 12);
static const int theMainMenuButtonTextFontSize = static_cast<int>(theNativeScreenSize.x / 24);

enum UI_MainMenu_ButtonType
{
	RESUME,
	NEW_GAME,
	PLAYER_COLOUR,
	VS_PLAYER_OR_COMPUTER,
	EXIT
};

enum UI_MainMenu_NewGameColour
{
	RANDOM_PLAYER,
	WHITE_PLAYER,
	BLACK_PLAYER,
	COUNT_NEWGAMECOLOUR,
};
