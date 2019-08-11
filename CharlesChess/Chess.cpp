#include <iostream>

#include "Chess_Board.h"
#include "Chess_Piece.h"
#include "Chess_Tile.h"

int main()
{
	Chess_Board chessBoard;

	const Chess_8x82DArray& chessArray = chessBoard.GetArray();

	for (const Chess_RankArray& rank : chessArray)
	{
		for (const Chess_Tile* const tile : rank)
		{
			if (const Chess_Piece* const piece = tile->GetPiece())
			{
				std::cout << piece->GetConsoleIdentifier();
			}
			else
			{
				std::cout << '.';
			}
		}
		std::cout << std::endl;
	}

    std::cout << "Hello World!\n"; 
}