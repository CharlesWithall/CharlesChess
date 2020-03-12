#include "UI_Board.h"

#include "UI_Defines.h"
#include "UI_Highlight_PossibleMove.h"
#include "UI_Highlight_PreviousMove.h"
#include "UI_Listener_Keyboard.h"
#include "UI_Listener_MouseClick.h"
#include "UI_Pieces.h"
#include "UI_Tile.h"

#include "Event_Handler.h"

#include "Chess_Board.h"
#include "Chess_Piece.h"
#include "Chess_Tile.h"

#include "AI_Model.h"

UI_Board::~UI_Board()
{
	if (mySelectedTile) delete mySelectedTile;
	if (mySelectedHighlight) delete mySelectedHighlight;

	for (UI_Tile* move : myPossibleMoves)
	{
		delete move;
	}

	for (UI_Highlight_PossibleMove* highlight : myPossibleMoveHighlights)
	{
		delete highlight;
	}

	for (UI_Highlight_PreviousMove* highlight : myPreviousMoveHighlights)
	{
		delete highlight;
	}

	if (myMouseClickListener) delete myMouseClickListener;
	if (myKeyboardListener) delete myKeyboardListener;
		
	Event_Handler::GetInstance()->UnregisterEvaluatedPossibleMovesListener(this);
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
	mySelectedHighlight->setFillColor(theTileSelectedHighlightColour);
	mySelectedHighlight->setSize(theNativeTileSize);

	// Listeners
	Event_Handler::GetInstance()->RegisterEvaluatedPossibleMovesListener(this);
	myMouseClickListener = new UI_Listener_MouseClick(this);
	myKeyboardListener = new UI_Listener_Keyboard(this);
}

void UI_Board::Update(sf::RenderWindow& aWindow)
{
	Draw(aWindow);

	UI_Model* model = UI_Model::GetInstance();
	if (model->IsMenuOpen())
	{
		myKeyboardListener->Update(aWindow);
		return;
	}

	if (model->GetIsPlayingAgainstComputer() && model->GetMyAIColour() == model->GetTurn())
	{
		AI_Model::GetInstance()->RequestAndMakeMove();
		model->Resync();
		model->SetTurnTaken();
	}
	else
	{
		myMouseClickListener->Update(aWindow);
		myKeyboardListener->Update(aWindow);
	}
}

void UI_Board::Draw(sf::RenderWindow& aWindow)
{
	// Draw Background + Selected Piece Highlights + Cache Pieces
	std::vector<UI_Piece*> cachedPieces;
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
				cachedPieces.push_back(piece);
			}
		}
	}

	// Draw Previous Move Highlights
	for (UI_Highlight_PreviousMove* highlight : myPreviousMoveHighlights)
	{
		if (highlight)
			highlight->Draw(aWindow);
	}

	// Draw Pieces
	for (UI_Piece* piece : cachedPieces)
	{
		aWindow.draw(piece->mySprite);
	}

	// Draw Possible Move Highlights
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
	for (int i = 0; i < (int)anEvent.myPossibleRankAndFiles.size(); ++i)
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

void UI_Board::Resync()
{
	const Chess_Board* const chessBoard = Chess_Model::GetInstance()->GetChessBoard();
	for (size_t i = 0; i < myChessTiles.size(); ++i)
	{
		UI_RankArray& rank = myChessTiles[i];
		for (size_t j = 0; j < rank.size(); ++j)
		{
			const Chess_RankAndFile rankAndFile = Chess_RankAndFile(i, j);
			if (Chess_Tile* tile = chessBoard->GetTile(rankAndFile))
			{
				if (Chess_Piece* piece = tile->GetPiece())
				{
					UI_Piece* uiPiece = rank[j]->GetPiece();
					if (!uiPiece || piece->GetType() != uiPiece->myType || piece->GetColour() != uiPiece->myColour)
					{
						if (uiPiece)
							delete uiPiece;

						rank[j]->SetPiece(new UI_Piece(piece->GetType(), piece->GetColour()));
					}
				}
				else
				{
					if (rank[j]->GetPiece())
					{
						delete rank[j]->GetPiece();
						rank[j]->SetPiece(nullptr);
					}
				}
			}
		}
	}

	if (const Chess_Move* move = chessBoard->GetLatestMove())
	{
		auto setHighlight = [&](const int index, const Chess_RankAndFile& rankAndFile)
		{
			const sf::Vector2f& tilePosition = GetTile(rankAndFile)->GetBackgroundTile().getPosition();
			if (myPreviousMoveHighlights[index] == nullptr)
			{
				UI_Highlight_PreviousMove* highlight = new UI_Highlight_PreviousMove();
				highlight->SetPosition(tilePosition);
				highlight->Enable();
				myPreviousMoveHighlights[index] = highlight;
			}
			else
			{
				myPreviousMoveHighlights[index]->SetPosition(tilePosition);
				myPreviousMoveHighlights[index]->Enable();
			}
		};

		setHighlight(0, move->myFromTile->GetRankAndFile());
		setHighlight(1, move->myToTile->GetRankAndFile());
	}
	else
	{
		for (UI_Highlight_PreviousMove* highlight : myPreviousMoveHighlights)
		{
			highlight->Disable();
		}
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
