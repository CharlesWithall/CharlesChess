#include "Chess_Model.h"

#include "Chess_Board.h"

Chess_Model* Chess_Model::myInstance = nullptr;

Chess_Model::Chess_Model()
{
	myChessBoard = new Chess_Board();
}

Chess_Model* Chess_Model::GetInstance()
{
	if (myInstance == nullptr)	myInstance = new Chess_Model();
	return myInstance;
}