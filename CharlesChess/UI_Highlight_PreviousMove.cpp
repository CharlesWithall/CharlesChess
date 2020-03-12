#include "UI_Highlight_PreviousMove.h"

#include "UI_Defines.h"

UI_Highlight_PreviousMove::UI_Highlight_PreviousMove() : myIsActive(false)
{
	myRectangleShape = new sf::RectangleShape(theNativeTileSize);
	myRectangleShape->setFillColor(thePreviousMoveHighlightColour);
}

UI_Highlight_PreviousMove::~UI_Highlight_PreviousMove()
{
	delete myRectangleShape;
}