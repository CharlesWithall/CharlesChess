#pragma once

#include "SFML/Graphics.hpp"
#include "UI_Defines.h"

class UI_MainMenu;
class UI_MainMenu_Graphics;

class UI_MainMenu_Button
{
public:
	UI_MainMenu_Button(UI_MainMenu* aMainMenu, const UI_MainMenu_Graphics* const someMainMenuGraphics, const UI_MainMenu_ButtonType aButtonType, const int aMenuPositionIndex);
	void Draw(sf::RenderWindow& aRenderWindow);

	const UI_MainMenu_ButtonType GetButtonType() const { return myButtonType; }
private:
	void SetBackgroundColour(const UI_MainMenu_Graphics* const someMainMenuGraphics, const int aBackgroundX, const int aBackgroundY);
	void SetText();

	bool OnHoverOver(sf::RenderWindow& aWindow);
	bool HandleMouseClick();
	bool PerformButtonAction();

	sf::Text myButtonText;
	sf::Sprite myButtonBackground;
	sf::Sprite myButtonHighlight;
	UI_MainMenu_ButtonType myButtonType;
	bool myButtonUpLatch;
	bool myButtonDownLatch;

	UI_MainMenu* myMainMenu;
	const UI_MainMenu_Graphics* myMainMenuGraphics;
};

