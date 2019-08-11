#pragma once

#include "SFML/Graphics.hpp"

class UI_Highlight_PossibleMove
{
public:
	UI_Highlight_PossibleMove();
	~UI_Highlight_PossibleMove();

	void Draw(sf::RenderWindow& aWindow) { if(myIsActive) aWindow.draw(*myRectangleShape); }

	void Disable() { myIsActive = false; }
	void Enable() { myIsActive = true; }

	void SetPosition(const sf::Vector2f& aPosition) { myRectangleShape->setPosition(aPosition); }

private:
	bool myIsActive;
	sf::RectangleShape* myRectangleShape;
};