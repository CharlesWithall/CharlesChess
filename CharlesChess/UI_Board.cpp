#include "UI_Board.h"

#include "UI_Highlight_PossibleMove.h"
#include "UI_Listener_MouseClick.h"
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
			const Chess_File file = Chess_File(j + 1);
			const int rank = i + 1;

			Chess_Pieces_Colour colour = isWhite ? Chess_Pieces_Colour::WHITE : Chess_Pieces_Colour::BLACK;
			rankArray[j] = new UI_Tile(colour, file, rank);

			if (Chess_Tile* tile = aChessBoard->GetTile(file, rank))
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

			if (sf::Sprite* piece = rank[j]->GetPiece())
			{
				aWindow.draw(*piece);
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
	for (int i = 0; i < anEvent.myPossibleMoveIndices.size(); ++i)
	{
		UI_Tile* possibleTile = myChessTiles[anEvent.myPossibleMoveIndices[i].myRankIndex][anEvent.myPossibleMoveIndices[i].myFileIndex];
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

void UI_Board::SetSelectedTile(UI_Tile* aSelectedTile)
{
	mySelectedTile = aSelectedTile;
	mySelectedHighlight->setPosition(aSelectedTile->GetBackgroundTile().getPosition());
}
