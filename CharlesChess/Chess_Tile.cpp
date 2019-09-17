#include "Chess_Tile.h"

#include "Chess_Piece.h"

void Chess_Tile::SetPiece(Chess_Piece* aPiece)
{
	if (aPiece)
	{
		myPiece = aPiece;
		myPiece->SetTile(this);
	}
	else
	{
		myPiece = nullptr;
	}
}

