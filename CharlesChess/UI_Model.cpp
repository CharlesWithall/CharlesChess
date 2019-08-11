#include "UI_Model.h"

#include "UI_Pieces.h"

UI_Model* UI_Model::myInstance = nullptr;

UI_Model* UI_Model::GetInstance()
{
	if (myInstance == nullptr)	myInstance = new UI_Model();
	return myInstance;
}

UI_Model::UI_Model()
{
	myPieces = new UI_Pieces();
}

void UI_Model::Run()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Charles Chess!");

	Chess_Board* chessBoard = Chess_Model::GetInstance()->GetChessBoard();
	UI_Board board;
	board.Init(chessBoard);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		board.Update(window);
		window.display();
	}
}
