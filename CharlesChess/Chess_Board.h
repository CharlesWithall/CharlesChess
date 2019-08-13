#pragma once

#include <array>

#include "Chess_Defines.h"

#include "Event_Listeners.h"

class Chess_Piece;
class Chess_Rule_Special;
class Chess_Tile;

typedef std::array<Chess_Tile*, 8> Chess_RankArray;
typedef std::array<Chess_RankArray, 8> Chess_8x82DArray;

class Chess_Board : Event_Listener_PieceSelected, Event_Listener_MovePieceRequest
{
public:
	Chess_Board();
	~Chess_Board();

	void InitTiles();
	void InitPieces();

	const Chess_8x82DArray& GetArray() const { return myChessTiles; };
	Chess_Piece* GetPiece(const Chess_RankAndFile& aRankAndFile) const;
	Chess_Tile* GetTile(const Chess_RankAndFile& aRankAndFile) const { return myChessTiles[aRankAndFile.myFile][aRankAndFile.myRank]; };
	Chess_Tile* GetRelativeTile(const Chess_Tile* const aTile, const int xOffset, const int yOffset) const;

	void OnPieceSelected(const Event_PieceSelected& anEvent) override;
	void OnMovePieceRequested(const Event_MovePieceRequest& anEvent) override;
private:
	Chess_Piece* CreatePiece(const Chess_RankAndFile& aRankAndile);

	Chess_8x82DArray myChessTiles;
	std::vector<Chess_Rule_Special*> mySpecialRules;
};

