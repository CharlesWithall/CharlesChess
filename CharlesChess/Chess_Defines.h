#pragma once

#include <string>

enum Chess_Pieces_EnumType
{
	INVALID,
	BISHOP,
	KING,
	KNIGHT,
	PAWN,
	QUEEN,
	ROOK
};

enum Chess_Pieces_Colour
{
	WHITE,
	BLACK
};

enum Chess_Source
{
	MOVE_REQUEST,
	CHECK_EVALUATION,
	GAME_OVER_EVALUATION
};

enum Chess_GameOverResult
{
	STALEMATE,
	WHITEWINS,
	BLACKWINS
};

class Chess_RankAndFile
{
public:
	Chess_RankAndFile(const int aFile, const int aRank)
		: myFile(aFile)
		, myRank(aRank)
		, myReadableName("")
	{	
	}

	const std::string GetReadableName()
	{
		if (myReadableName.empty())
		{
			std::string files = "ABCDEFGH";
			std::string ranks = "12345678";

			if (IsValid())
			{
				myReadableName = std::string() + files[myFile] + ranks[myRank];
			}
			else
			{
				myReadableName = std::string("INVALID");
			}
		}

		return myReadableName;
	}

	Chess_RankAndFile GetOffset(const int aFile, const int aRank) const
	{
		return Chess_RankAndFile(aFile + myFile, aRank + myRank);
	}

	bool IsValid() const
	{
		return myFile >= 0 && myFile < 8 && myRank >= 0 && myRank < 8;
	}

	bool operator==(const Chess_RankAndFile& rhs) const
	{
		if (myFile == rhs.myFile && myRank == rhs.myRank)
		{
			return true;
		}

		return false;
	}

	int myFile;
	int myRank;

private:
	std::string myReadableName;
};