#pragma once

#include <array>

#include "Chess_Defines.h"

#include "Event_Listeners.h"

class Chess_Piece;
class Chess_Tile;

typedef std::array<Chess_Tile*, 8> Chess_RankArray;
typedef std::array<Chess_RankArray, 8> Chess_8x82DArray;

class Chess_Board : public Event_Listener_PieceSelected
{
public:
	Chess_Board();
	~Chess_Board();

	void InitTiles();
	void InitPieces();

	const Chess_8x82DArray& GetArray() const { return myChessTiles; };
	Chess_Piece* GetPiece(const Chess_File aFile, const int aRank) const;
	Chess_Tile* GetTile(const Chess_File aFile, const int aRank) const { return myChessTiles[aFile-1][aRank-1]; };
	Chess_Tile* GetRelativeTile(const Chess_Tile* const aTile, const int xOffset, const int yOffset) const;

	void OnPieceSelected(const Event_PieceSelected& anEvent) override;
private:
	Chess_Piece* CreatePiece(const Chess_File aFile, const int aRank);
	
	Chess_8x82DArray myChessTiles; // Each array of files contains an array of ranks
};

