#include "UI_Highlight_PossibleMove.h"

#include "UI_Defines.h"

UI_Highlight_PossibleMove::UI_Highlight_PossibleMove() : myIsActive(false)
{
	myRectangleShape = new sf::RectangleShape(theNativeTileSize);
	myRectangleShape->setFillColor(thePossibleMoveHighlightColour);
}

UI_Highlight_PossibleMove::~UI_Highlight_PossibleMove()
{
	delete myRectangleShape;
}