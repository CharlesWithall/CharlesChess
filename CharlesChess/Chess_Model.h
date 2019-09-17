#pragma once

class Chess_Board;

class Chess_Model
{
public:
	~Chess_Model();

	static Chess_Model* GetInstance();
	Chess_Board* GetChessBoard() { return myChessBoard; }

	void Reset();
private:
	Chess_Model();

	Chess_Board* myChessBoard;

	static Chess_Model* myInstance;
};

