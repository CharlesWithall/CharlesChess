#include "UI_MainMenu_Button.h"

#include "UI_MainMenu_Graphics.h"
#include "UI_Model.h"

#include "Chess_Model.h"

UI_MainMenu_Button::UI_MainMenu_Button(UI_MainMenu* aMainMenu, const UI_MainMenu_Graphics* const someMainMenuGraphics, const UI_MainMenu_ButtonType aButtonType, const int aMenuPositionIndex)
	: myButtonType(aButtonType)
	, myButtonDownLatch(false)
	, myButtonUpLatch(false)
	, myMainMenu(aMainMenu)
	, myMainMenuGraphics(someMainMenuGraphics)
{
	const float bgX = theMainMenuFirstButtonPosition.x;
	const float bgY = theMainMenuFirstButtonPosition.y + (theMainMenuButtonSpacing * aMenuPositionIndex);
	
	SetBackgroundColour(myMainMenuGraphics, static_cast<int>(bgX), static_cast<int>(bgY));

	myButtonHighlight = sf::Sprite(myMainMenuGraphics->GetButtonHighlight());
	myButtonHighlight.setPosition(bgX, bgY);

	// CPW: Move the position setting into SetText()
	const float textX = theMainMenuFirstButtonTextPosition.x;
	const float textY = theMainMenuFirstButtonTextPosition.y + (theMainMenuButtonSpacing * aMenuPositionIndex);
	myButtonText.setFont(myMainMenuGraphics->GetFont());
	myButtonText.setCharacterSize(theMainMenuButtonTextFontSize);
	myButtonText.setFillColor(theMainMenuButtonTextColour);
	SetText();
	myButtonText.setPosition(textX, textY);
}

void UI_MainMenu_Button::Draw(sf::RenderWindow& aRenderWindow)
{
	if (!OnHoverOver(aRenderWindow))
	{
		aRenderWindow.draw(myButtonText);
	}	
}

void UI_MainMenu_Button::SetBackgroundColour(const UI_MainMenu_Graphics* const someMainMenuGraphics, const int aBackgroundX, const int aBackgroundY)
{
	const sf::Vector2f bgVector = sf::Vector2f(static_cast<float>(aBackgroundX), static_cast<float>(aBackgroundY));

	switch (myButtonType)
	{
	case UI_MainMenu_ButtonType::RESUME:
	case UI_MainMenu_ButtonType::NEW_GAME:
	case UI_MainMenu_ButtonType::EXIT:
		myButtonBackground = sf::Sprite(someMainMenuGraphics->GetButtonBackground());
		myButtonBackground.setPosition(bgVector);
		break;
	case UI_MainMenu_ButtonType::PLAYER_COLOUR:
	case UI_MainMenu_ButtonType::VS_PLAYER_OR_COMPUTER:
		myButtonBackground = sf::Sprite(someMainMenuGraphics->GetToggleButtonBackground());
		myButtonBackground.setPosition(bgVector);
		break;
	default:
		myButtonBackground = sf::Sprite(someMainMenuGraphics->GetButtonBackground());
		myButtonBackground.setPosition(bgVector);
		break;
	}
}

void UI_MainMenu_Button::SetText()
{
	switch (myButtonType)
	{
	case UI_MainMenu_ButtonType::RESUME:
		myButtonText.setString("Resume");
		break;
	case UI_MainMenu_ButtonType::NEW_GAME:
		myButtonText.setString("New Game");
		break;
	case UI_MainMenu_ButtonType::PLAYER_COLOUR:
	{
		switch (UI_Model::GetInstance()->GetNewGameColour())
		{
		case UI_MainMenu_NewGameColour::RANDOM_PLAYER:
			myButtonText.setString("Play as Random");
			break;
		case UI_MainMenu_NewGameColour::BLACK_PLAYER:
			myButtonText.setString("Play as Black");
			break;
		case UI_MainMenu_NewGameColour::WHITE_PLAYER:
			myButtonText.setString("Play as White");
			break;
		default:
			myButtonText.setString("");
			break;
		}
		break;
	}
	case UI_MainMenu_ButtonType::VS_PLAYER_OR_COMPUTER:
	{
		if (UI_Model::GetInstance()->GetIsPlayingAgainstComputer())
			myButtonText.setString("Against AI");
		else
			myButtonText.setString("Against Player");
		break;
	}
	case UI_MainMenu_ButtonType::EXIT:
		myButtonText.setString("Exit Game");
		break;
	default:
		myButtonText.setString("INVALID");
		break;
	}

	myMainMenuGraphics->CentreAlignText(myButtonText);
}

bool UI_MainMenu_Button::OnHoverOver(sf::RenderWindow& aWindow)
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(aWindow));
	if (myButtonBackground.getGlobalBounds().contains(mousePos))
	{
		aWindow.draw(myButtonHighlight);
		return HandleMouseClick();
	}
	else
	{
		aWindow.draw(myButtonBackground);
		return false;
	}
}

bool UI_MainMenu_Button::HandleMouseClick()
{
	if (!myButtonUpLatch && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		myButtonUpLatch = true;
		return false;
	}

	if (myButtonUpLatch && !myButtonDownLatch && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		myButtonDownLatch = true;
		return false;
	}

	if (myButtonUpLatch && myButtonDownLatch && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		myButtonUpLatch = false;
		myButtonDownLatch = false;
		return PerformButtonAction();
	}

	return false;
}

bool UI_MainMenu_Button::PerformButtonAction()
{
	switch (myButtonType)
	{
	case UI_MainMenu_ButtonType::RESUME:
	{
		UI_Model::GetInstance()->RequestCloseMenu();
		return true;
	}
	case UI_MainMenu_ButtonType::NEW_GAME:
	{
		Chess_Model::GetInstance()->Reset();
		UI_Model::GetInstance()->RequestReset();
		UI_Model::GetInstance()->SetAIColourFromSelection();
		return true;
	}
	case UI_MainMenu_ButtonType::PLAYER_COLOUR:
	{
		if (!UI_Model::GetInstance()->GetIsPlayingAgainstComputer())
		{
			UI_Model::GetInstance()->SetNewGameColour(UI_MainMenu_NewGameColour::COUNT_NEWGAMECOLOUR);
		}
		else
		{
			UI_MainMenu_NewGameColour newGameColour = static_cast<UI_MainMenu_NewGameColour>(static_cast<int>(UI_Model::GetInstance()->GetNewGameColour()) + 1);
			if (newGameColour >= UI_MainMenu_NewGameColour::COUNT_NEWGAMECOLOUR)
			{
				newGameColour = UI_MainMenu_NewGameColour::RANDOM_PLAYER;
			}

			UI_Model::GetInstance()->SetNewGameColour(newGameColour);
		}
		
		SetText();
		return false;
	}
	case UI_MainMenu_ButtonType::VS_PLAYER_OR_COMPUTER:
	{
		UI_Model::GetInstance()->SetIsPlayingAgainstComputer(!UI_Model::GetInstance()->GetIsPlayingAgainstComputer());
		
		if (UI_Model::GetInstance()->GetIsPlayingAgainstComputer())
			UI_Model::GetInstance()->SetNewGameColour(UI_MainMenu_NewGameColour::RANDOM_PLAYER);
		else
			UI_Model::GetInstance()->SetNewGameColour(UI_MainMenu_NewGameColour::COUNT_NEWGAMECOLOUR);
		
		if (UI_MainMenu_Button* button = myMainMenu->GetButton(UI_MainMenu_ButtonType::PLAYER_COLOUR))
		{
			button->SetText();
		}

		SetText();
		return false;
	}
	case UI_MainMenu_ButtonType::EXIT:
		std::exit(0);
		return true;
	}

	return false;
}