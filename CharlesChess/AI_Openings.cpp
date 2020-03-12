#include "AI_Openings.h"

#include <fstream>
#include <iterator>
#include <random>

#include "Chess_Board.h"
#include "Chess_Defines.h"
#include "Chess_Helpers.h"

#include "Debug_Readout.h"

#include "json.hpp"

using json = nlohmann::json;

AI_Openings::AI_Openings()
{
	Init();
}


AI_Openings::~AI_Openings()
{
}

bool AI_Openings::SelectMoveFromOpeningCache(const Chess_Board* aBoard, Chess_Move_Simple& aMoveOut) const
{
	if (aBoard->GetHistoryIndex() >= myMaxOpeningLength)
		return false;

	DEBUG_PRINT("Searching for opening...");

	std::vector<std::string> moveHistoryNotation;
	moveHistoryNotation.clear();
	DEBUG_PRINT_AI("Opening: //////////////////////");
	DEBUG_PRINT_AI("Opening: Current Moves:");
	for (const Chess_Move& move : aBoard->GetMoveHistory())
	{
		moveHistoryNotation.push_back(ConvertChessMoveToNotation(move, aBoard));
		DEBUG_PRINT_AI("Opening: %s", moveHistoryNotation.back().c_str());
	}
	DEBUG_PRINT_AI("Opening: //////////////////////");

	std::vector<AI_Opening> possibleOpenings;

	for (const AI_Opening& opening : myOpenings)
	{
		if (aBoard->GetHistoryIndex() >= opening.myMoves.size())
			continue;

		bool openingIsValid = true;
		for (unsigned int i = 0; i < opening.myMoves.size(); ++i)
		{
			if (i >= aBoard->GetHistoryIndex())
				break;

			if (opening.myMoves[i] != moveHistoryNotation[i])
				openingIsValid = false;

			if (!openingIsValid)
				break;
		}

		if (openingIsValid)
		{
			DEBUG_PRINT_AI("Opening: %s selected as valid opening", opening.myName.c_str());
			possibleOpenings.push_back(opening);
		}
	}

	if (possibleOpenings.size() == 0)
	{
		DEBUG_PRINT("No openings found.");
		return false;
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	auto begin = possibleOpenings.begin();
	auto end = possibleOpenings.end();
	std::uniform_int_distribution<> dis(0, std::distance(begin, end) - 1);
	std::advance(begin, dis(gen));

	aMoveOut = ConvertNotationToChessMoveSimple(begin->myMoves[aBoard->GetHistoryIndex()], aBoard);
	if (aMoveOut.myFromTile.IsValid() && aMoveOut.myToTile.IsValid())
	{
		DEBUG_PRINT("Opening Selected: %s", possibleOpenings.begin()->myName.c_str());
		return true;
	}

	return false;
}

void AI_Openings::Init()
{
	std::ifstream i("openings.json");
	json openings = json::parse(i);

	for (auto opening : openings)
	{
		AI_Opening newOpening;
		newOpening.myName = opening["name"].get<std::string>();
		unsigned int i = 0;
		for (auto move : opening["moves"])
		{
			newOpening.myMoves.push_back(move.get<std::string>());
			++i;
		}

		myMaxOpeningLength = std::max(myMaxOpeningLength, i);

		myOpenings.push_back(newOpening);
	}
}

Chess_Move_Simple AI_Openings::ConvertNotationToChessMoveSimple(const std::string& someNotation, const Chess_Board* aBoard) const
{
	Chess_Move_Simple move;

	if (someNotation.rfind("O-O-O", 0) == 0) // Queenside Castle
	{
		if (aBoard->GetActivePlayer() == Chess_Pieces_Colour::WHITE)
		{
			move.myFromTile = Chess_RankAndFile("E1");
			move.myToTile = Chess_RankAndFile("C1");
		}
		else
		{
			move.myFromTile = Chess_RankAndFile("E8");
			move.myToTile = Chess_RankAndFile("C8");
		}

		return move;
	}
	else if (someNotation.rfind("O-O", 0) == 0) // Kingside Castle
	{
		if (aBoard->GetActivePlayer() == Chess_Pieces_Colour::WHITE)
		{
			move.myFromTile = Chess_RankAndFile("E1");
			move.myToTile = Chess_RankAndFile("G1");
		}
		else
		{
			move.myFromTile = Chess_RankAndFile("E8");
			move.myToTile = Chess_RankAndFile("G8");
		}

		return move;
	}

	move.myToTile = Chess_RankAndFile(someNotation.substr(someNotation.length() - 2).c_str());

	if (someNotation.rfind("N", 0) == 0) // Handle Knight Move
	{
		if (const Chess_Piece* piece = Chess_Helpers::GetAttackingKnight(aBoard->GetTile(move.myToTile), aBoard, aBoard->GetActivePlayer()))
			move.myFromTile = piece->GetTile()->GetRankAndFile();
	}
	else if (someNotation.rfind("B", 0) == 0) // Handle Bishop Move
	{
		if (const Chess_Piece* piece = Chess_Helpers::GetAttackingBishop(aBoard->GetTile(move.myToTile), aBoard, aBoard->GetActivePlayer()))
			move.myFromTile = piece->GetTile()->GetRankAndFile();
	}
	else if (someNotation.rfind("R", 0) == 0) // Handle Rook Move
	{
		if (const Chess_Piece* piece = Chess_Helpers::GetAttackingRook(aBoard->GetTile(move.myToTile), aBoard, aBoard->GetActivePlayer()))
			move.myFromTile = piece->GetTile()->GetRankAndFile();
	}
	else if (someNotation.rfind("Q", 0) == 0) // Handle Queen Move
	{
		if (const Chess_Piece* piece = Chess_Helpers::GetAttackingQueen(aBoard->GetTile(move.myToTile), aBoard, aBoard->GetActivePlayer()))
			move.myFromTile = piece->GetTile()->GetRankAndFile();
	}
	else if (someNotation.rfind("K", 0) == 0) // Handle King Move
	{
		// There are no king moves in the openings - I'm going to be lazy and not implement this
	}
	else // Handle Pawn Move
	{
		int multiplier = aBoard->GetActivePlayer() == Chess_Pieces_Colour::WHITE ? -1 : 1;
		if (someNotation.length() == 2)
		{
			for (int i = 1; i <= 2; ++i)
			{
				Chess_Tile* tile = aBoard->GetRelativeTile(aBoard->GetTile(move.myToTile), 0, i * multiplier);
				if (tile->GetPiece())
				{
					move.myFromTile = tile->GetRankAndFile();
					return move;
				}
			}
		}
		else if (someNotation.length() == 4)
		{
			std::string rankAndFile = std::string(1, static_cast<char>(std::toupper(someNotation[0])));
			rankAndFile.append(std::string(1, static_cast<char>(static_cast<int>(move.myToTile.GetReadableName()[1]) - multiplier)));
			move.myFromTile = Chess_RankAndFile(rankAndFile.c_str());
			return move;
		}
	}

	return move;
}

std::string AI_Openings::ConvertChessMoveToNotation(const Chess_Move& aMove, const Chess_Board* aBoard) const
{
	if (aMove.mySpecialMoveType == Chess_Special_Move_Type::CASTLE)
	{
		if (aMove.myToTile->GetRankAndFile().myFile > 5)
			return "O-O";
		else
			return "O-O-O";
	}

	Chess_Piece* takingPiece = aMove.myTakingPiece;
	const bool takingPieceIsPawn = takingPiece->GetType() == Chess_Pieces_EnumType::PAWN;
	std::string piece = takingPieceIsPawn ? "" : std::string(1, takingPiece->GetConsoleIdentifier());

	if (takingPiece->GetType() == Chess_Pieces_EnumType::PAWN && aMove.myTakenPiece)
	{
		piece = static_cast<char>(std::tolower(aMove.myFromTile->GetRankAndFile().GetReadableName()[0]));
	}

	std::string takes = aMove.myTakenPiece ? "x" : "";
	std::string toFile = std::string(1, static_cast<char>(std::tolower(aMove.myToTile->GetRankAndFile().GetReadableName()[0])));
	std::string toRank = std::string(1, static_cast<char>(aMove.myToTile->GetRankAndFile().GetReadableName()[1]));

	return piece + takes + toFile + toRank;
}


// for pawns - work back 1 or 2 depending on opponents turn