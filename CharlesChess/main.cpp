#include "SFML/Graphics.hpp"

#include "UI_Model.h"

int main()
{
	UI_Model::GetInstance()->Run();

	return 0;
}

// TO DO:
// - Stale Mate - repeated moves
// - Previous Moves UI
// - Examine moves that take pieces(not pawns?) first
// - Depth search is relative to number of pieces
// - At end of game, keep searching until the end of the game
// - Create Simplified Board for analysis

// BUGS:
// - Disable Clicking During Pawn Promotion Menu and Main Menu
// - En Passant Crashes Game? Handle En Passant/Castling/Pawn Promotion when Evaluating
// - Pawn Promotion Evaluation doesn't work
// - Play as White/Black are reversed for AI games

// OPTIMIZATION:
/////////////////////////////////////////////
//-- - Query Average Times-- -
/////////////////////////////////////////////
//Copy Board = 2.08634ns
//Move Piece = 0.0165923ns
//Possible Moves = 117.082ns
/////////////////////////////////////////////
/////////////////////////////////////////////
//-- - Query Total Times-- -
/////////////////////////////////////////////
//Copy Board = 2.08425s
//Move Piece = 0.0165757s
//Possible Moves = 10.6545s
/////////////////////////////////////////////
/////////////////////////////////////////////
//-- - Query Average Times-- -
/////////////////////////////////////////////
//Copy Board = 2.64468ns
//Move Piece = 0.0173634ns
//Move Rule class Chess_Rule_Bishop_Move = 0.081952ns
//Move Rule class Chess_Rule_King_Castle = 0.0664649ns
//Move Rule class Chess_Rule_King_Move = 0.114761ns
//Move Rule class Chess_Rule_Knight_Move = 0.142135ns
//Move Rule class Chess_Rule_Pawn_EnPassant = 0.0274886ns
//Move Rule class Chess_Rule_Pawn_Move = 0.077013ns
//Move Rule class Chess_Rule_Pawn_Take = 0.049207ns
//Move Rule class Chess_Rule_Queen_Move = 0.124931ns
//Move Rule class Chess_Rule_Rook_Move = 0.0791539ns
/////////////////////////////////////////////
/////////////////////////////////////////////
//-- - Query Total Times-- -
/////////////////////////////////////////////
//Copy Board = 2.64203s
//Move Piece = 0.017346s
//Move Rule class Chess_Rule_Bishop_Move = 0.425659s
//Move Rule class Chess_Rule_King_Castle = 0.0073776s
//Move Rule class Chess_Rule_King_Move = 0.298035s
//Move Rule class Chess_Rule_Knight_Move = 0.73825s
//Move Rule class Chess_Rule_Pawn_EnPassant = 0.570883s
//Move Rule class Chess_Rule_Pawn_Move = 1.59941s
//Move Rule class Chess_Rule_Pawn_Take = 1.02193s
//Move Rule class Chess_Rule_Queen_Move = 0.324447s
//Move Rule class Chess_Rule_Rook_Move = 0.411125s
/////////////////////////////////////////////

// Minimax Depth = 2 - Before Reverse search refactor
// Check Average Time = 2.42055ns
// Check Total Time = 1.59998s
