#include "Chess_Model.h"

#include "Chess_Board.h"

Chess_Model* Chess_Model::myInstance = nullptr;

Chess_Model::~Chess_Model()
{
	if (myChessBoard) delete myChessBoard;
}

Chess_Model::Chess_Model()
{
	Reset();
}

Chess_Model* Chess_Model::GetInstance()
{
	if (myInstance == nullptr)	myInstance = new Chess_Model();
	return myInstance;
}

void Chess_Model::Reset()
{
	if (myChessBoard) delete myChessBoard;
	myChessBoard = new Chess_Board();
}