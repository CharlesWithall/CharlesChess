#include "Debug_Readout.h"

#include "Chess_Board.h"
#include "Chess_Piece.h"
#include "Chess_Tile.h"

#include <iostream>

/*static*/ void Debug_Readout::WriteBoard(const Chess_Board* aChessBoard)
{
	std::array<std::array<std::string, 8>, 8> cb;

	for (std::array<std::string, 8>& ranks : cb)
	{
		ranks.fill("  ");
	}

	for (Chess_Piece* piece : aChessBoard->GetWhitePieces())
	{
		const Chess_RankAndFile& rF = piece->GetTile()->GetRankAndFile();
		std::string writeString = "w";
		writeString += piece->GetConsoleIdentifier();
		cb[rF.myFile][rF.myRank] = writeString;
	}
	
	for (Chess_Piece* piece : aChessBoard->GetBlackPieces())
	{
		const Chess_RankAndFile& rF = piece->GetTile()->GetRankAndFile();
		std::string writeString = "b";
		writeString += piece->GetConsoleIdentifier();
		cb[rF.myFile][rF.myRank] = writeString;
	}

	std::cout << "---------------------------------------------------------" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "|  " << cb[0][7] << "  |  " << cb[1][7] << "  |  " << cb[2][7] << "  |  " << cb[3][7] << "  |  " << cb[4][7] << "  |  " << cb[5][7] << "  |  " << cb[6][7] << "  |  " << cb[7][7] << "  |" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "---------------------------------------------------------" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "|  " << cb[0][6] << "  |  " << cb[1][6] << "  |  " << cb[2][6] << "  |  " << cb[3][6] << "  |  " << cb[4][6] << "  |  " << cb[5][6] << "  |  " << cb[6][6] << "  |  " << cb[7][6] << "  |" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "---------------------------------------------------------" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "|  " << cb[0][5] << "  |  " << cb[1][5] << "  |  " << cb[2][5] << "  |  " << cb[3][5] << "  |  " << cb[4][5] << "  |  " << cb[5][5] << "  |  " << cb[6][5] << "  |  " << cb[7][5] << "  |" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "---------------------------------------------------------" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "|  " << cb[0][4] << "  |  " << cb[1][4] << "  |  " << cb[2][4] << "  |  " << cb[3][4] << "  |  " << cb[4][4] << "  |  " << cb[5][4] << "  |  " << cb[6][4] << "  |  " << cb[7][4] << "  |" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "---------------------------------------------------------" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "|  " << cb[0][3] << "  |  " << cb[1][3] << "  |  " << cb[2][3] << "  |  " << cb[3][3] << "  |  " << cb[4][3] << "  |  " << cb[5][3] << "  |  " << cb[6][3] << "  |  " << cb[7][3] << "  |" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "---------------------------------------------------------" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "|  " << cb[0][2] << "  |  " << cb[1][2] << "  |  " << cb[2][2] << "  |  " << cb[3][2] << "  |  " << cb[4][2] << "  |  " << cb[5][2] << "  |  " << cb[6][2] << "  |  " << cb[7][2] << "  |" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "---------------------------------------------------------" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "|  " << cb[0][1] << "  |  " << cb[1][1] << "  |  " << cb[2][1] << "  |  " << cb[3][1] << "  |  " << cb[4][1] << "  |  " << cb[5][1] << "  |  " << cb[6][1] << "  |  " << cb[7][1] << "  |" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "---------------------------------------------------------" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "|  " << cb[0][0] << "  |  " << cb[1][0] << "  |  " << cb[2][0] << "  |  " << cb[3][0] << "  |  " << cb[4][0] << "  |  " << cb[5][0] << "  |  " << cb[6][0] << "  |  " << cb[7][0] << "  |" << std::endl;
	std::cout << "|      |      |      |      |      |      |      |      |" << std::endl;
	std::cout << "---------------------------------------------------------" << std::endl;
}