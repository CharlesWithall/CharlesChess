#include "UI_Highlight_PossibleMove.h"

#include "UI_Defines.h"

static const sf::Color locPossibleMoveHighlightColour = sf::Color(40, 80, 44, 120);

UI_Highlight_PossibleMove::UI_Highlight_PossibleMove() : myIsActive(false)
{
	myRectangleShape = new sf::RectangleShape(theNativeTileSize);
	myRectangleShape->setFillColor(locPossibleMoveHighlightColour);
}

UI_Highlight_PossibleMove::~UI_Highlight_PossibleMove()
{
	delete myRectangleShape;
}