#include "UI_Board.h"

#include "UI_Highlight_PossibleMove.h"
#include "UI_Listener_MouseClick.h"
#include "UI_Pieces.h"
#include "UI_Tile.h"

#include "Event_Handler.h"

#include "Chess_Board.h"
#include "Chess_Piece.h"
#include "Chess_Tile.h"

UI_Board::~UI_Board()
{
	if (mySelectedHighlight) delete mySelectedHighlight;
	for (UI_Highlight_PossibleMove* highlight : myPossibleMoveHighlights)
	{
		delete highlight;
	}
};

void UI_Board::Init(Chess_Board* aChessBoard)
{
	// Pieces and Tiles
	bool isWhite = false;
	for (size_t i = 0; i < myChessTiles.size(); ++i)
	{
		UI_RankArray& rankArray = myChessTiles[i];
		for (size_t j = 0; j < rankArray.size(); ++j)
		{
			const Chess_RankAndFile rankAndFile = Chess_RankAndFile(i, j);
			Chess_Pieces_Colour colour = isWhite ? Chess_Pieces_Colour::WHITE : Chess_Pieces_Colour::BLACK;
			rankArray[j] = new UI_Tile(colour, rankAndFile);

			if (Chess_Tile* tile = aChessBoard->GetTile(rankAndFile))
			{
				if (Chess_Piece* piece = tile->GetPiece())
				{
					rankArray[j]->InitPiece(piece->GetType(), piece->GetColour());
				}
			}

			isWhite = !isWhite;
		}
		isWhite = !isWhite;
	}

	// Highlights
	mySelectedHighlight = new sf::RectangleShape();
	mySelectedHighlight->setFillColor(sf::Color(60, 118, 67, 120));
	mySelectedHighlight->setSize(sf::Vector2f(100, 100));

	// Listeners
	Event_Handler::GetInstance()->RegisterEvaluatedPossibleMovesListener(this);
	Event_Handler::GetInstance()->RegisterMovePieceRequestListener(this);
	myMouseClickListener = new UI_Listener_MouseClick(this);
}

void UI_Board::Update(sf::RenderWindow& aWindow)
{
	Draw(aWindow);
	myMouseClickListener->Update(aWindow);
}

void UI_Board::Draw(sf::RenderWindow& aWindow)
{
	for (size_t i = 0; i < myChessTiles.size(); ++i)
	{
		UI_RankArray& rank = myChessTiles[i];
		for (size_t j = 0; j < rank.size(); ++j)
		{
			aWindow.draw(rank[j]->GetBackgroundTile());

			if (mySelectedTile == rank[j])
			{
				aWindow.draw(*mySelectedHighlight);
			}

			if (UI_Piece* piece = rank[j]->GetPiece())
			{
				aWindow.draw(piece->mySprite);
			}
		}
	}

	if (mySelectedTile)
	{
		for (UI_Highlight_PossibleMove* highlight : myPossibleMoveHighlights)
		{
			highlight->Draw(aWindow);
		}
	}
}

void UI_Board::ClearPossibleMoves()
{
	for (UI_Highlight_PossibleMove* possibleMoveHighlight : myPossibleMoveHighlights)
	{
		possibleMoveHighlight->Disable();
	}

	myPossibleMoves.clear();
}

void UI_Board::OnMovesEvaluated(const Event_EvaulatedPossibleMoves& anEvent)
{
	const int lastHighlightIndex = myPossibleMoveHighlights.size() - 1;
	for (int i = 0; i < anEvent.myPossibleRankAndFiles.size(); ++i)
	{
		UI_Tile* possibleTile = GetTile(anEvent.myPossibleRankAndFiles[i]);
		myPossibleMoves.push_back(possibleTile);
		const sf::Vector2f tilePosition = possibleTile->GetBackgroundTile().getPosition();

		if (i <= lastHighlightIndex)
		{
			myPossibleMoveHighlights[i]->SetPosition(tilePosition);
			myPossibleMoveHighlights[i]->Enable();			
		}
		else
		{
			UI_Highlight_PossibleMove* highlight = new UI_Highlight_PossibleMove();
			highlight->SetPosition(tilePosition);
			highlight->Enable();
			myPossibleMoveHighlights.push_back(highlight);
		}
	}
}

void UI_Board::OnMovePieceRequested(const Event_MovePieceRequest& anEvent)
{
	UI_Tile* moveToTile = GetTile(anEvent.myToPosition);
	UI_Tile* moveFromTile = GetTile(anEvent.myFromPosition);

	moveToTile->SetPiece(moveFromTile->GetPiece());
	moveFromTile->SetPiece(nullptr);

	if (anEvent.myShouldEndTurn)
	{
		UI_Model::GetInstance()->SetTurnTaken();
	}
}

void UI_Board::SetSelectedTile(UI_Tile* aSelectedTile)
{
	mySelectedTile = aSelectedTile;

	if (mySelectedTile)
	{
		mySelectedHighlight->setPosition(mySelectedTile->GetBackgroundTile().getPosition());
	}
}
