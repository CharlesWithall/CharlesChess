#include "UI_Listener_MouseClick.h"

#include "UI_Board.h"
#include "UI_Pieces.h"
#include "UI_Tile.h"

#include "Event_Handler.h"

#include "Chess_Model.h" // UN HACK THIS TODO CPW:
#include "Chess_Board.h"

UI_Listener_MouseClick::UI_Listener_MouseClick(UI_Board* aChessBoard)
	: myChessBoard(aChessBoard)
{
}

void UI_Listener_MouseClick::Update(sf::RenderWindow& aWindow)
{
	if (!aWindow.hasFocus())
		return;

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
	const bool isDebugMode = UI_Model::GetInstance()->IsDebugMode();

	if (!clickedTile)
	{
		return;
	}

	if (isDebugMode && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		Chess_Tile* tile = Chess_Model::GetInstance()->GetChessBoard()->GetTile(clickedTile->GetRankAndFile());
		if (tile && tile->GetPiece())
		{
			Chess_Model::GetInstance()->GetChessBoard()->RemovePiece(tile->GetPiece());
			myChessBoard->Resync();
		}
		return;
	}

	// Clear Already Selected Tile
	if (selectedTile == clickedTile)
	{
		myChessBoard->ClearPossibleMoves();
		myChessBoard->SetSelectedTile(nullptr);
		return;
	}

	UI_Piece* clickedPiece = clickedTile->GetPiece();
	UI_Piece* selectedPiece = selectedTile ? selectedTile->GetPiece() : nullptr;
	const bool playerIsTakingPiece = selectedPiece && clickedPiece && selectedPiece->myColour != clickedPiece->myColour;
	const bool clickedPieceOwnedByPlayer = clickedPiece && clickedPiece->myColour == UI_Model::GetInstance()->GetTurn();

	// Select Piece
	if (clickedTile->GetPiece() && !playerIsTakingPiece && clickedPieceOwnedByPlayer)
	{
		myChessBoard->ClearPossibleMoves();
		myChessBoard->SetSelectedTile(clickedTile);
		Event_Handler::GetInstance()->SendPieceSelectedEvent(clickedTile->GetRankAndFile());
		return;
	}
	
	// Make Move
	if (selectedTile)
	{
		if (isDebugMode)
		{
			Event_Handler::GetInstance()->SendMovePieceRequestEvent(selectedTile->GetRankAndFile(), clickedTile->GetRankAndFile());
			myChessBoard->SetSelectedTile(nullptr);
			myChessBoard->ClearPossibleMoves();
			myChessBoard->Resync();
			UI_Model::GetInstance()->SetTurnTaken();

			return;
		}

		for (UI_Tile* move : myChessBoard->GetPossibleMoves())
		{
			if (move == clickedTile)
			{
				const bool isPawnMove = selectedTile->GetPiece()->myType == Chess_Pieces_EnumType::PAWN;
				const bool isMovingToFinalRank = move->GetRankAndFile().myRank == 0 || move->GetRankAndFile().myRank == 7;
				
				if (isPawnMove && isMovingToFinalRank)
				{
					myChessBoard->SetSelectedTile(nullptr);
					myChessBoard->ClearPossibleMoves();
					UI_Model::GetInstance()->OpenPawnPromotionMenu(selectedTile->GetRankAndFile(), clickedTile->GetRankAndFile(), selectedTile->GetPiece()->myColour);
				}
				else
				{
					Event_Handler::GetInstance()->SendMovePieceRequestEvent(selectedTile->GetRankAndFile(), clickedTile->GetRankAndFile());
					myChessBoard->SetSelectedTile(nullptr);
					myChessBoard->ClearPossibleMoves();
					myChessBoard->Resync();
					UI_Model::GetInstance()->SetTurnTaken();
				}

				return;
			}
		}
	}
	
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
