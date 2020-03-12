#pragma once

#include <array>
#include <stack>

#include "Chess_Defines.h"
#include "Chess_Move.h"
#include "Chess_Tile.h"

#include "Event_Listeners.h"
#include "Event_Defines.h"

class Chess_Pawn;
class Chess_Piece;
class Chess_Rule_GameOver;
class Chess_Rule_Special;

typedef std::array<Chess_Tile*, 8> Chess_RankArray;
typedef std::array<Chess_RankArray, 8> Chess_8x82DArray;

class Chess_Board
	: Event_Listener_PieceSelected
	, Event_Listener_MovePieceRequest
{
public:
	Chess_Board();
	~Chess_Board();

	void InitTiles();
	void InitPieces();

	const Chess_8x82DArray& GetArray() const { return myChessTiles; };
	Chess_Piece* GetPiece(const Chess_RankAndFile& aRankAndFile) const { return GetTile(aRankAndFile)->GetPiece(); }
	Chess_Tile* GetTile(const Chess_RankAndFile& aRankAndFile) const { return myChessTiles[aRankAndFile.myFile][aRankAndFile.myRank]; };
	Chess_Tile* GetRelativeTile(const Chess_Tile* const aTile, const int xOffset, const int yOffset) const;
	const Chess_Pieces_Colour GetActivePlayer() const { return myMoveHistory.size() % 2 == 0 ? Chess_Pieces_Colour::WHITE : Chess_Pieces_Colour::BLACK; }

	const std::array<Chess_Piece*, 16>& GetWhitePieces() const { return myWhitePieces; }
	const std::array<Chess_Piece*, 16>& GetBlackPieces() const { return myBlackPieces; }
	const std::vector<Chess_Piece*>& GetExtraWhitePieces() const { return myExtraWhitePieces; }
	const std::vector<Chess_Piece*>& GetExtraBlackPieces() const { return myExtraBlackPieces; }

	void OnPieceSelected(const Event_PieceSelected& anEvent) override;
	void OnMovePieceRequested(const Event_MovePieceRequest& anEvent) override;
	void PerformMovePieceRequest(Chess_Tile* const aFromTile, Chess_Tile* const aToTile, const bool aShouldPerformGameOverCheck);
	void OnTakeBackPieceRequested();

	void PlacePiece(Chess_Piece* aPiece, Chess_Tile* aTile);
	void PlacePiece(const Chess_RankAndFile& aRankAndFile, const Chess_Pieces_Colour aColour, const Chess_Pieces_EnumType aType);
	void MovePiece(Chess_Tile* const aFromTile, Chess_Tile* const aToTile);
	void RemovePiece(Chess_Piece* aPiece);

	Chess_Move* GetLatestMove() { return !myMoveHistory.empty() ? &myMoveHistory.back() : nullptr; }
	const Chess_Move* GetLatestMove() const { return !myMoveHistory.empty() ? &myMoveHistory.back() : nullptr; }
	const unsigned int GetHistoryIndex() const { return myMoveHistory.size(); }
	const std::vector<Chess_Move>& GetMoveHistory() const { return myMoveHistory; }

	const int GetNumberOfMovesSincePawnMoveOrPieceTaken() const { return myNumberOfMovesSincePawnMoveOrPieceTaken; }
	const bool GetIsPassedPawn(const Chess_Pawn* aPawn) const;

	const std::vector<Chess_Move_Simple> EvaluateAllPossibleMoves(const Chess_Pieces_Colour aColour);

	// DEBUG

	void Init_Debug_Test_Pieces();

	//
private:
	Chess_Piece* CreatePiece(const Chess_RankAndFile& aRankAndFile);

	Chess_Special_Move_Type HandleSpecialMove_MovePiece(Chess_Tile* const aFromTile, Chess_Tile* const aToTile);
	bool HandleSpecialMove_TakeBack(const Chess_Move& aMove);

	void AddMoveToHistory(Chess_Tile* aFromTile, Chess_Tile* aToTile);
	void CheckGameOver();

	Chess_8x82DArray myChessTiles;
	std::array<Chess_Piece*, 16> myWhitePieces;
	std::array<Chess_Piece*, 16> myBlackPieces;
	std::vector<Chess_Piece*> myExtraWhitePieces;
	std::vector<Chess_Piece*> myExtraBlackPieces;

	std::vector<Chess_Rule_Special*> mySpecialRules;
	std::vector<Chess_Rule_GameOver*> myGameOverRules;
	std::vector<Chess_Move> myMoveHistory;
	int myNumberOfMovesSincePawnMoveOrPieceTaken = 0;
};

