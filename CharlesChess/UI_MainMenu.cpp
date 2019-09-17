#include "UI_MainMenu.h"

#include "UI_Defines.h"
#include "UI_MainMenu_Button.h"
#include "UI_MainMenu_Graphics.h"

UI_MainMenu::UI_MainMenu(const bool aShouldShowResumeButton/* = false*/)
{
	myTitleText.setString("Welcome to Charles Chess!");
	Init(aShouldShowResumeButton);
}

UI_MainMenu::UI_MainMenu(const Chess_GameOverResult aGameOverResult)
{
	switch (aGameOverResult)
	{
	case Chess_GameOverResult::BLACKWINS:
		myTitleText.setString("Check Mate - Black Wins!");
		break;
	case Chess_GameOverResult::WHITEWINS:
		myTitleText.setString("Check Mate - White Wins!");
		break;
	case Chess_GameOverResult::STALEMATE:
		myTitleText.setString("Stale Mate - It's a Draw");
		break;
	default:
		myTitleText.setString("Welcome to Charles Chess!");
		break;
	}

	Init(false);
}

UI_MainMenu::~UI_MainMenu()
{
	delete myMainMenuGraphics;
	for (UI_MainMenu_Button* button : myButtons)
	{
		delete button;
	}
}

void UI_MainMenu::Init(const bool aShouldShowResumeButton/* = false*/)
{
	myMainMenuGraphics = new UI_MainMenu_Graphics();

	InitBackground();
	InitTitle();
	InitButtons(aShouldShowResumeButton);
}

void UI_MainMenu::InitBackground()
{
	myBackground = sf::RectangleShape(theNativeScreenSize);
	myBackground.setFillColor(theMenuBackgroundColour);
}

void UI_MainMenu::InitTitle()
{
	myTitleText.setFont(myMainMenuGraphics->GetFont());
	myTitleText.setCharacterSize(theMainMenuTitleTextFontSize);
	myTitleText.setFillColor(theMainMenuTitleTextColour);
	myMainMenuGraphics->CentreAlignText(myTitleText);
	myTitleText.setPosition(theMainMenuTitleTextPosition);

	myTitleHighlight = myTitleText;
	myTitleHighlight.setPosition(theMainMenuTitleTextPosition.x + 2, theMainMenuTitleTextPosition.y - 2);
	myTitleHighlight.setFillColor(theMainMenuTextHighlight);

	myTitleDoubleHighlight = myTitleText;
	myTitleDoubleHighlight.setPosition(theMainMenuTitleTextPosition.x - 3, theMainMenuTitleTextPosition.y + 3);
	myTitleDoubleHighlight.setFillColor(sf::Color::Black);
}

void UI_MainMenu::InitButtons(const bool aShouldShowResumeButton/* = false*/)
{
	int index = 0;
	if (aShouldShowResumeButton) myButtons.push_back(new UI_MainMenu_Button(this, myMainMenuGraphics, UI_MainMenu_ButtonType::RESUME, index++));
	myButtons.push_back(new UI_MainMenu_Button(this, myMainMenuGraphics, UI_MainMenu_ButtonType::NEW_GAME, index++));
	myButtons.push_back(new UI_MainMenu_Button(this, myMainMenuGraphics, UI_MainMenu_ButtonType::PLAYER_COLOUR, index++));
	myButtons.push_back(new UI_MainMenu_Button(this, myMainMenuGraphics, UI_MainMenu_ButtonType::VS_PLAYER_OR_COMPUTER, index++));
	myButtons.push_back(new UI_MainMenu_Button(this, myMainMenuGraphics, UI_MainMenu_ButtonType::EXIT, index++));
}

void UI_MainMenu::Draw(sf::RenderWindow& aWindow)
{
	aWindow.draw(myBackground);
	aWindow.draw(myTitleDoubleHighlight);
	aWindow.draw(myTitleHighlight);
	aWindow.draw(myTitleText);
	
	for (UI_MainMenu_Button* button : myButtons)
	{
		if (button)	button->Draw(aWindow);
	}
}

UI_MainMenu_Button* UI_MainMenu::GetButton(const UI_MainMenu_ButtonType aButtonType)
{
	for (UI_MainMenu_Button* button : myButtons)
	{
		if (button && button->GetButtonType() == aButtonType)
		{
			return button;
		}
	}

	return nullptr;
}