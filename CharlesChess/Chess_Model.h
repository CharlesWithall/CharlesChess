#pragma once

class Chess_Board;
class Chess_EventHandler;

class Chess_Model
{
public:

	static Chess_Model* GetInstance();
	Chess_Board* GetChessBoard() { return myChessBoard; }

private:
	Chess_Model();

	Chess_Board* myChessBoard;
	Chess_EventHandler* myEventHandler;

	static Chess_Model* myInstance;
};

