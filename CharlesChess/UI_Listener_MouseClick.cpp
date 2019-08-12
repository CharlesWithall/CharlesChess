#include "UI_Listener_MouseClick.h"

#include "UI_Board.h"
#include "UI_Tile.h"

#include "Event_Handler.h"

void UI_Listener_MouseClick::Update(sf::RenderWindow& aWindow)
{
	static bool mouseDownLatch = false;

	if (mouseDownLatch != sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		mouseDownLatch = sf::Mouse::isButtonPressed(sf::Mouse::Left);

		if (!mouseDownLatch)
		{
			return;
		}

		OnMouseClick(aWindow);
	}
}

void UI_Listener_MouseClick::OnMouseClick(sf::RenderWindow& aWindow)
{
	const std::vector<UI_Tile*>& possibleMoves = myChessBoard->GetPossibleMoves();
	UI_Tile* clickedTile = GetClickedTile(aWindow);
	UI_Tile* selectedTile = myChessBoard->GetSelectedTile();

	if (!clickedTile)
	{
		return;
	}

	// Clear Already Selected Tile
	if (selectedTile == clickedTile)
	{
		myChessBoard->ClearPossibleMoves();
		myChessBoard->SetSelectedTile(nullptr);
		return;
	}

	// Select Piece
	if (clickedTile->GetPiece())
	{
		myChessBoard->ClearPossibleMoves();
		myChessBoard->SetSelectedTile(clickedTile);
		Event_Handler::GetInstance()->SendPieceSelectedEvent(clickedTile->GetRankAndFile());
		return;
	}
	
	// Make Move
	if (selectedTile)
	{
		for (UI_Tile* move : myChessBoard->GetPossibleMoves())
		{
			if (move == clickedTile)
			{
				Event_Handler::GetInstance()->SendMovePieceRequestEvent(selectedTile->GetRankAndFile(), clickedTile->GetRankAndFile());
				myChessBoard->SetSelectedTile(nullptr);
				myChessBoard->ClearPossibleMoves();
				return;
			}
		}
	}
	// CPW: TODO: Can't Take pieces

	// If all else fails just clear the board of highlights and selections
	myChessBoard->SetSelectedTile(nullptr);
	myChessBoard->ClearPossibleMoves();
	return;
}

UI_Tile* UI_Listener_MouseClick::GetClickedTile(sf::RenderWindow& aWindow)
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(aWindow));
	const UI_8x82DArray& chessTiles = myChessBoard->GetTiles();
	for (size_t i = 0; i < chessTiles.size(); ++i)
	{
		const UI_RankArray& rank = chessTiles[i];
		for (size_t j = 0; j < rank.size(); ++j)
		{
			const sf::RectangleShape& bgTile = rank[j]->GetBackgroundTile();
			if (bgTile.getGlobalBounds().contains(mousePos))
			{
				return rank[j];
			}
		}
	}

	return nullptr;
}
