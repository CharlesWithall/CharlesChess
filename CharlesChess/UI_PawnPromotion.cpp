#include "UI_PawnPromotion.h"

#include "UI_Defines.h"
#include "UI_Pieces.h"

#include "Event_Handler.h"

UI_PawnPromotion::UI_PawnPromotion(const Chess_RankAndFile& aFromRankAndFile, const Chess_RankAndFile& aToRankAndFile, const Chess_Pieces_Colour aColour)
	: myColour(aColour)
	, myFromRankAndFile(aFromRankAndFile)
	, myToRankAndFile(aToRankAndFile)
	, myButtonDownLatch(false)
	, myButtonUpLatch(false)
{
	const int squareLength = 800;
	const float xOffset = static_cast<float>(aToRankAndFile.myFile * theNativeTileSize.x);

	const float queenYPosition = static_cast<float>(aColour == Chess_Pieces_Colour::WHITE ? theNativeTileSize.y * 0 : theNativeTileSize.y * 7);
	const float rookYPosition = static_cast<float>(aColour == Chess_Pieces_Colour::WHITE ? theNativeTileSize.y * 1 : theNativeTileSize.y * 6);
	const float bishopYPosition = static_cast<float>(aColour == Chess_Pieces_Colour::WHITE ? theNativeTileSize.y * 2 : theNativeTileSize.y * 5);
	const float knightYPosition = static_cast<float>(aColour == Chess_Pieces_Colour::WHITE ? theNativeTileSize.y * 3 : theNativeTileSize.y * 4);

	myBackground = sf::RectangleShape(theNativeScreenSize);
	myBackground.setFillColor(theMenuBackgroundColour);

	myQueenBackground.setFillColor(theBlackTileColour);
	myRookBackground.setFillColor(theWhiteTileColour);
	myBishopBackground.setFillColor(theBlackTileColour);
	myKnightBackground.setFillColor(theWhiteTileColour);

	myQueenBackground.setPosition(sf::Vector2f(xOffset, queenYPosition));
	myRookBackground.setPosition(sf::Vector2f(xOffset, rookYPosition));
	myBishopBackground.setPosition(sf::Vector2f(xOffset, bishopYPosition));
	myKnightBackground.setPosition(sf::Vector2f(xOffset, knightYPosition));

	myQueenBackground.setSize(theNativeTileSize);
	myRookBackground.setSize(theNativeTileSize);
	myBishopBackground.setSize(theNativeTileSize);
	myKnightBackground.setSize(theNativeTileSize);

	if (sf::Texture* pieceTexture = UI_Model::GetInstance()->GetPieces()->GetPieceTexture(Chess_Pieces_EnumType::QUEEN, aColour))
	{
		myQueenIcon = sf::Sprite(*pieceTexture);
		myQueenIcon.setPosition(sf::Vector2f(xOffset, queenYPosition));
	}

	if (sf::Texture* pieceTexture = UI_Model::GetInstance()->GetPieces()->GetPieceTexture(Chess_Pieces_EnumType::ROOK, aColour))
	{
		myRookIcon = sf::Sprite(*pieceTexture);
		myRookIcon.setPosition(sf::Vector2f(xOffset, rookYPosition));
	}

	if (sf::Texture* pieceTexture = UI_Model::GetInstance()->GetPieces()->GetPieceTexture(Chess_Pieces_EnumType::BISHOP, aColour))
	{
		myBishopIcon = sf::Sprite(*pieceTexture);
		myBishopIcon.setPosition(sf::Vector2f(xOffset, bishopYPosition));
	}

	if (sf::Texture* pieceTexture = UI_Model::GetInstance()->GetPieces()->GetPieceTexture(Chess_Pieces_EnumType::KNIGHT, aColour))
	{
		myKnightIcon = sf::Sprite(*pieceTexture);
		myKnightIcon.setPosition(sf::Vector2f(xOffset, knightYPosition));
	}

	mySelectedHighlight = sf::RectangleShape();
	mySelectedHighlight.setFillColor(theTileSelectedHighlightColour);
	mySelectedHighlight.setSize(theNativeTileSize);
}

void UI_PawnPromotion::Draw(sf::RenderWindow& aWindow)
{
	aWindow.draw(myBackground);

	aWindow.draw(myQueenBackground);
	aWindow.draw(myRookBackground);
	aWindow.draw(myBishopBackground);
	aWindow.draw(myKnightBackground);

	aWindow.draw(myQueenIcon);
	aWindow.draw(myRookIcon);
	aWindow.draw(myBishopIcon);
	aWindow.draw(myKnightIcon);

	OnHoverOver(aWindow);
}

void UI_PawnPromotion::OnHoverOver(sf::RenderWindow& aWindow)
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(aWindow));

	if (myQueenBackground.getGlobalBounds().contains(mousePos))
	{
		mySelectedHighlight.setPosition(myQueenBackground.getPosition());
		aWindow.draw(mySelectedHighlight);

		HandleMouseClick(Chess_Pieces_EnumType::QUEEN);
	}
	else if (myRookBackground.getGlobalBounds().contains(mousePos))
	{
		mySelectedHighlight.setPosition(myRookBackground.getPosition());
		aWindow.draw(mySelectedHighlight);

		HandleMouseClick(Chess_Pieces_EnumType::ROOK);
	}
	else if (myBishopBackground.getGlobalBounds().contains(mousePos))
	{
		mySelectedHighlight.setPosition(myBishopBackground.getPosition());
		aWindow.draw(mySelectedHighlight);

		HandleMouseClick(Chess_Pieces_EnumType::BISHOP);
	}
	else if (myKnightBackground.getGlobalBounds().contains(mousePos))
	{
		mySelectedHighlight.setPosition(myKnightBackground.getPosition());
		aWindow.draw(mySelectedHighlight);

		HandleMouseClick(Chess_Pieces_EnumType::KNIGHT);
	}
}

void UI_PawnPromotion::HandleMouseClick(const Chess_Pieces_EnumType aChessPieceType)
{
	if (!myButtonUpLatch && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		myButtonUpLatch = true;
		return;
	}

	if (myButtonUpLatch && !myButtonDownLatch && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		myButtonDownLatch = true;
		return;
	}

	if (myButtonUpLatch && myButtonDownLatch && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//Event_Handler::GetInstance()->SendPawnPromotionRequestEvent(myFromRankAndFile, myToRankAndFile, aChessPieceType);
		Event_Handler::GetInstance()->SendMovePieceRequestEvent(myFromRankAndFile, myToRankAndFile);
		UI_Model::GetInstance()->Resync();
		UI_Model::GetInstance()->SetTurnTaken();
		UI_Model::GetInstance()->ClosePawnPromotionMenu();
	}
}
