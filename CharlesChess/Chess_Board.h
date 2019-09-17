#pragma once

#include <array>
#include <stack>

#include "Chess_Defines.h"

#include "Event_Listeners.h"
#include "Event_Defines.h"

struct Chess_Move;
struct Chess_Move_Simple;
class Chess_Piece;
class Chess_Rule_GameOver;
class Chess_Rule_Special;
class Chess_Tile;

typedef std::array<Chess_Tile*, 8> Chess_RankArray;
typedef std::array<Chess_RankArray, 8> Chess_8x82DArray;

class Chess_Board
	: Event_Listener_PieceSelected
	, Event_Listener_MovePieceRequest
	, Event_Listener_ReplacePieceRequest
{
public:
	Chess_Board();
	~Chess_Board();

	Chess_Board(const Chess_Board& aChessBoard);

	void InitTiles();
	void InitPieces();

	const Chess_8x82DArray& GetArray() const { return myChessTiles; };
	Chess_Piece* GetPiece(const Chess_RankAndFile& aRankAndFile) const;
	Chess_Tile* GetTile(const Chess_RankAndFile& aRankAndFile) const { return myChessTiles[aRankAndFile.myFile][aRankAndFile.myRank]; };
	Chess_Tile* GetRelativeTile(const Chess_Tile* const aTile, const int xOffset, const int yOffset) const;
	const Chess_Pieces_Colour GetActivePlayer() const { return myMoveHistory.size() % 2 == 0 ? Chess_Pieces_Colour::WHITE : Chess_Pieces_Colour::BLACK; }

	const std::vector<Chess_Piece*>& GetWhitePieces() const { return myWhitePieces; }
	const std::vector<Chess_Piece*>& GetBlackPieces() const { return myBlackPieces; }

	void OnPieceSelected(const Event_PieceSelected& anEvent) override;
	void OnMovePieceRequested(const Event_MovePieceRequest& anEvent) override;
	void OnReplacePieceRequested(const Event_ReplacePieceRequest& anEvent) override;

	void MovePiece(Chess_Tile* const aToTile, Chess_Tile* const aFromTile, const Event_Source anEventSource);
	void TakePiece(Chess_Piece* aPiece);

	const Chess_Move* GetLatestMove() const { return !myMoveHistory.empty() ? myMoveHistory.top() : nullptr; }
	const int GetNumberOfMovesSincePawnMoveOrPieceTaken() const { return myNumberOfMovesSincePawnMoveOrPieceTaken; }

	const std::vector<Chess_Move_Simple> EvaluateAllPossibleMoves(const Chess_Pieces_Colour aColour) const;
private:
	Chess_Piece* CreatePiece(const Chess_RankAndFile& aRankAndile);
	void PlacePiece(Chess_Piece* aPiece, Chess_Tile* aTile);

	Chess_8x82DArray myChessTiles;
	std::vector<Chess_Piece*> myWhitePieces;
	std::vector<Chess_Piece*> myBlackPieces;
	std::vector<Chess_Rule_Special*> mySpecialRules;
	std::vector<Chess_Rule_GameOver*> myGameOverRules;
	std::stack<Chess_Move*> myMoveHistory;
	int myNumberOfMovesSincePawnMoveOrPieceTaken = 0;
};

