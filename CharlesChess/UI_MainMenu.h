#pragma once

#include <string>
#include <vector>
#include "SFML/Graphics.hpp"

#include "Chess_Defines.h"

#include "UI_Defines.h"

class UI_MainMenu_Button;
class UI_MainMenu_Graphics;

class UI_MainMenu
{
public:
	UI_MainMenu(const bool aShouldShowResumeButton = false);
	UI_MainMenu(const Chess_GameOverResult aGameOverResult);
	~UI_MainMenu();

	UI_MainMenu_Graphics* const GetMainMenuGraphics() const { return myMainMenuGraphics; }
	void Draw(sf::RenderWindow& aWindow);

	UI_MainMenu_Button* GetButton(const UI_MainMenu_ButtonType aButtonType);
private:
	void Init(const bool aShouldShowResumeButton);

	void InitBackground();
	void InitTitle();
	void InitButtons(const bool aShouldShowResumeButton);

	sf::Text myTitleText;
	sf::Text myTitleHighlight;
	sf::Text myTitleDoubleHighlight;

	sf::RectangleShape myBackground;
	std::vector<UI_MainMenu_Button*> myButtons;
	UI_MainMenu_Graphics* myMainMenuGraphics;
};

