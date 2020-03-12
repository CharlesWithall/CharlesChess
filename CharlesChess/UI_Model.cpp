#include "UI_Model.h"

#include "UI_MainMenu.h"
#include "UI_Pieces.h"

#include "Chess_Board.h"

#include "Event_Handler.h"

UI_Model* UI_Model::myInstance = nullptr;

UI_Model* UI_Model::GetInstance()
{
	if (myInstance == nullptr)	myInstance = new UI_Model();
	return myInstance;
}

UI_Model::UI_Model()
{
	myPieces = new UI_Pieces();
	myTurn = Chess_Pieces_Colour::WHITE;
	myPawnPromotion = nullptr;
	myMainMenu = nullptr;

	myIsPlayingAgainstComputer = false;
	myNewGameColour = UI_MainMenu_NewGameColour::COUNT_NEWGAMECOLOUR;
	myDebugModeEnabled = false;

	Event_Handler::GetInstance()->RegisterGameOverListener(this);
}

UI_Model::~UI_Model()
{	
	if (myPieces) delete myPieces;
	if (myPawnPromotion) delete myPawnPromotion;
	if (myMainMenu) delete myMainMenu;

	Event_Handler::GetInstance()->UnregisterGameOverListener(this);
}

void UI_Model::Run()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Charles Chess!");

	Reset();
	myMainMenu = new UI_MainMenu();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		myUIBoard->Update(window);
		if (myPawnPromotion) myPawnPromotion->Draw(window);
		if (myMainMenu) myMainMenu->Draw(window);
		window.display();

		if (myShouldCloseMenu)
		{
			CloseMenu();
		}

		if (myShouldReset)
		{
			Reset();
		}
	}
}

void UI_Model::Reset()
{
	if (myUIBoard)
	{
		delete myUIBoard;
		myUIBoard = nullptr;
	}

	if (myPawnPromotion)
	{
		delete myPawnPromotion;
		myPawnPromotion = nullptr;
	}

	if (myMainMenu)
	{
		delete myMainMenu;
		myMainMenu = nullptr;
	}

	myShouldReset = false;

	myTurn = Chess_Pieces_Colour::WHITE;
	myUIBoard = new UI_Board();
	Chess_Board* chessBoard = Chess_Model::GetInstance()->GetChessBoard();
	myUIBoard->Init(chessBoard);
}

void UI_Model::OnGameOver(const Event_GameOver& anEvent)
{
	myMainMenu = new UI_MainMenu(anEvent.myGameOverResult);
}

void UI_Model::SetAIColourFromSelection()
{
	switch (myNewGameColour)
	{
	case UI_MainMenu_NewGameColour::BLACK_PLAYER:
		myAIColour = Chess_Pieces_Colour::WHITE;
		break;
	case UI_MainMenu_NewGameColour::WHITE_PLAYER:
		myAIColour = Chess_Pieces_Colour::BLACK;
		break;
	case UI_MainMenu_NewGameColour::RANDOM_PLAYER:
		myAIColour = static_cast<Chess_Pieces_Colour>(std::rand() % 2);
		break;
	default:
		break;
	}
}