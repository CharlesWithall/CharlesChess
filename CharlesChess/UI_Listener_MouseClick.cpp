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
	myChessBoard->ClearPossibleMoves();
	UI_Tile* aTile = GetClickedTile(aWindow);

	if (!aTile)
	{
		return;
	}

	if (!aTile->GetPiece() || myChessBoard->GetSelectedTile() == aTile)
	{
		myChessBoard->SetSelectedTile(nullptr);
		return;
	}

	// Standardise all rank and file gets to be less random, either use rank and file or indices.
	// aTile->GetRankAndFile
	// Handle what happens if you click a valid move.
	Event_Handler::GetInstance()->SendPieceSelectedEvent(, ); // Get Indices from Tile here
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
