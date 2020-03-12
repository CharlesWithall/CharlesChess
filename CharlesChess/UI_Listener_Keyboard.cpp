#include "UI_Listener_Keyboard.h"

#include <iostream>

#include "AI_Model.h"
#include "Chess_Board.h"
#include "Chess_Model.h"
#include "UI_Model.h"

#include "Debug_Defines.h"
#include "Debug_Readout.h"

#include "Event_Handler.h"

// A - Force AI Turn
// S - Print out the static analysis of the current position
// D - Toggle Debug Mode
// P - Print Chess Board
// C - Change Which Colour's Turn It Is
// 1-0 - Set AI Search Depth Override (0 cancels the override)

void UI_Listener_Keyboard::Update(sf::RenderWindow& aWindow)
{
	if (!aWindow.hasFocus())
		return;

	const bool isDebugMode = UI_Model::GetInstance()->IsDebugMode();

	static bool letterAKeyLatch = false;
	static bool letterSKeyLatch = false;
	static bool letterDKeyLatch = false;
	static bool letterCKeyLatch = false;
	static bool letterPKeyLatch = false;

	static bool escapeKeyLatch = false;
	static bool backspaceKeyLatch = false;

	static bool number1KeyLatch = false;
	static bool number2KeyLatch = false;
	static bool number3KeyLatch = false;
	static bool number4KeyLatch = false;
	static bool number5KeyLatch = false;
	static bool number6KeyLatch = false;
	static bool number7KeyLatch = false;
	static bool number8KeyLatch = false;
	static bool number9KeyLatch = false;
	static bool number0KeyLatch = false;

	// Letter A
	if (isDebugMode && letterAKeyLatch != sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		letterAKeyLatch = sf::Keyboard::isKeyPressed(sf::Keyboard::A);

		if (letterAKeyLatch)
		{
			OnAKeyPressed(aWindow);
		}	
	}

	// Letter S
	if (isDebugMode && letterSKeyLatch != sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		letterSKeyLatch = sf::Keyboard::isKeyPressed(sf::Keyboard::S);

		if (letterSKeyLatch)
		{
			OnSKeyPressed(aWindow);
		}
	}

	// Letter D
	if (letterDKeyLatch != sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		letterDKeyLatch = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

		if (letterDKeyLatch)
		{
			OnDKeyPressed(aWindow);
		}
	}

	// Letter P
	if (isDebugMode && letterPKeyLatch != sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		letterPKeyLatch = sf::Keyboard::isKeyPressed(sf::Keyboard::P);

		if (letterPKeyLatch)
		{
			OnPKeyPressed(aWindow);
		}
	}

	// Letter C
	if (isDebugMode && letterCKeyLatch != sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		letterCKeyLatch = sf::Keyboard::isKeyPressed(sf::Keyboard::C);

		if (letterCKeyLatch)
		{
			OnCKeyPressed(aWindow);
		}
	}

	// Escape
	if (escapeKeyLatch != sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		escapeKeyLatch = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

		if (escapeKeyLatch)
		{
			OnEscapeKeyPressed(aWindow);
		}
	}

	// Backspace
	if (isDebugMode && backspaceKeyLatch != sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
	{
		backspaceKeyLatch = sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace);

		if (backspaceKeyLatch)
		{
			OnBackspaceKeyPressed(aWindow);
		}
	}

	if (isDebugMode)
	{
		HandleNumberKeyPress(aWindow, sf::Keyboard::Num1, number1KeyLatch);
		HandleNumberKeyPress(aWindow, sf::Keyboard::Num2, number2KeyLatch);
		HandleNumberKeyPress(aWindow, sf::Keyboard::Num3, number3KeyLatch);
		HandleNumberKeyPress(aWindow, sf::Keyboard::Num4, number4KeyLatch);
		HandleNumberKeyPress(aWindow, sf::Keyboard::Num5, number5KeyLatch);
		HandleNumberKeyPress(aWindow, sf::Keyboard::Num6, number6KeyLatch);
		HandleNumberKeyPress(aWindow, sf::Keyboard::Num7, number7KeyLatch);
		HandleNumberKeyPress(aWindow, sf::Keyboard::Num8, number8KeyLatch);
		HandleNumberKeyPress(aWindow, sf::Keyboard::Num9, number9KeyLatch);
		HandleNumberKeyPress(aWindow, sf::Keyboard::Num0, number0KeyLatch);
	}
}

void UI_Listener_Keyboard::OnAKeyPressed(sf::RenderWindow& aWindow)
{
	AI_Model::GetInstance()->RequestAndMakeMove();
	UI_Model::GetInstance()->Resync();
	UI_Model::GetInstance()->SetTurnTaken();
}

void UI_Listener_Keyboard::OnSKeyPressed(sf::RenderWindow& aWindow)
{
	const float analysisBlack = AI_Model::GetInstance()->RequestStaticAnalysis(Chess_Model::GetInstance()->GetChessBoard(), Chess_Pieces_Colour::BLACK);
	const float analysisWhite = AI_Model::GetInstance()->RequestStaticAnalysis(Chess_Model::GetInstance()->GetChessBoard(), Chess_Pieces_Colour::WHITE);
	std::cout << "Static Analysis of Current Position if Black's Turn = " << analysisBlack << std::endl;
	std::cout << "Static Analysis of Current Position if White's Turn = " << analysisWhite << std::endl;
}

void UI_Listener_Keyboard::OnDKeyPressed(sf::RenderWindow& aWindow)
{
	const bool isEnabled = UI_Model::GetInstance()->ToggleDebugMode();
	const char* debugText = isEnabled ? "ENABLED" : "DISABLED";
	std::cout << "DEBUG MODE " << debugText << std::endl;
}

void UI_Listener_Keyboard::OnPKeyPressed(sf::RenderWindow& aWindow)
{
	Debug_Readout::WriteBoard(Chess_Model::GetInstance()->GetChessBoard());
}

void UI_Listener_Keyboard::OnCKeyPressed(sf::RenderWindow& aWindow)
{
	UI_Model::GetInstance()->SetTurnTaken();
}

void UI_Listener_Keyboard::OnEscapeKeyPressed(sf::RenderWindow& aWindow)
{
	UI_Model::GetInstance()->RequestOpenMenu(true);
}

void UI_Listener_Keyboard::OnBackspaceKeyPressed(sf::RenderWindow& aWindow)
{
	Chess_Model::GetInstance()->GetChessBoard()->OnTakeBackPieceRequested();
	myChessBoard->SetSelectedTile(nullptr);
	myChessBoard->ClearPossibleMoves();
	UI_Model::GetInstance()->SetTurnTaken();
	UI_Model::GetInstance()->Resync();
}

void UI_Listener_Keyboard::HandleNumberKeyPress(sf::RenderWindow& aWindow, sf::Keyboard::Key aKey, bool& aLatch)
{
	if (aLatch != sf::Keyboard::isKeyPressed(aKey))
	{
		aLatch = sf::Keyboard::isKeyPressed(aKey);

		if (aLatch)
		{
			OnNumberKeyPressed(aWindow, aKey);
		}
	}
}

void UI_Listener_Keyboard::OnNumberKeyPressed(sf::RenderWindow& aWindow, sf::Keyboard::Key aKey)
{
	int depth = 0;
	switch (aKey)
	{
	case sf::Keyboard::Num1:
		depth = 1;
		break;
	case sf::Keyboard::Num2:
		depth = 2;
		break;
	case sf::Keyboard::Num3:
		depth = 3;
		break;
	case sf::Keyboard::Num4:
		depth = 4;
		break;
	case sf::Keyboard::Num5:
		depth = 5;
		break;
	case sf::Keyboard::Num6:
		depth = 6;
		break;
	case sf::Keyboard::Num7:
		depth = 7;
		break;
	case sf::Keyboard::Num8:
		depth = 8;
		break;
	case sf::Keyboard::Num9:
		depth = 9;
		break;
	default:
		depth = 0;
	}

	AI_Model::GetInstance()->SetOverrideSearchDepth(depth);
}