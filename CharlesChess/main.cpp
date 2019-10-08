#include "SFML/Graphics.hpp"

#include "UI_Model.h"

int main()
{
	UI_Model::GetInstance()->Run();

	return 0;
}

// MAIN TO DO:
// Fix Analyse crashes
// Make Pawn Promotion Evaluation right
// Change the delete/re-instantiate of pieces
// Why are the knights over-zealous?

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

//////////////////////////////////
// OPTIMIZATIONS
//////////////////////////////////
// Reverse Search Check Refactor (Minimax Depth = 2)
//////////////////////////////////
// Check Average Time: Before = 2.42055ns -> After = 0.17801ns
// Check Total Time: Before = 1.59998s -> After = 0.117665s
// Average Save = 2.24254ns
// Total Save = 1.482335s
//////////////////////////////////
// Chess Board Copying
//////////////////////////////////
// There's not point in copying a chess board every time for evaluation.
// What we could do is have a 'reverse' move step.
// Each 'move' has an undo function.
// Thus, instead of creating a new chess board for every evaluation we simply call the 'undo' function.
// We have a 'move' history stored in the chess_board which is updated every time a piece is added/deleted/moved.
// We take that history and reapply it.

//////////////////////////////////
// PLAYS STOCKFISH LEVEL 1 - MINIMAX DEPTH 2
//////////////////////////////////
// Very aggressive, not looking far ahead so it sees any like for like capture as a gain?
// After 10 moves each, I'm up a pawn and have forced Stockfish's king from castling.
// Move 9 h5 - peculiar?
// Move 12 nxf2 - takes a pawn and loses the knight - why?
// Move 14 - avoids attack on rook by bishop
// Move 20 - a6 - weird move.
// Move 26 - rook to h1 check - knight is in danger but he chooses to check and trade the rooks
// Moves 32-36 Knight repeats itself? f4 e6 f4 e6 f4
// Move 46 - Stockfish inexplicably doesn't take my knight
// Endgame - I'm not interested in marching my pawns up... I have two passed pawns sitting on the second rank still
// Move 65 - My AI has been messing around for ages and allows the rook and knight to get forked
// Move 72 - But Stockfish allows the bishop to get pinned...
// Stale Mate - Three move repetition - My AI is unwilling to move its pawns...

// Full First Game Against Stockfish 1 Below
//1
//e4
//Nc6
//2
//d3
//e5
//3
//Nf3
//Bb4 +
//4
//Nc3
//Bxc3 +
//5
//bxc3
//d5
//6
//Qd2
//dxe4
//7
//Ng1
//exd3
//8
//Bxd3
//Nf6
//9
//Ne2
//h5
//10
//Bb5
//Qxd2 +
//11
//Kxd2
//Ne4 +
//12
//Ke3
//Nxf2
//13
//Kxf2
//O - O
//14
//Ba3
//Re8
//15
//Bd3
//Be6
//16
//Rab1
//Bxa2
//17
//Rb2
//Bd5
//18
//Rb5
//Be6
//19
//Ng3
//Rad8
//20
//Rxb7
//a6
//21
//Re1
//h4
//22
//Nf5
//Bxf5
//23
//Bxf5
//Rd2 +
//24
//Kf1
//h3
//25
//gxh3
//Rxh2
//26
//Rxc7
//Rh1 +
//27
//Kf2
//Rxe1
//28
//Kxe1
//Nd8
//29
//Be4
//Ne6
//30
//Rb7
//Nf4
//31
//Kf2
//Nxh3 +
//32
//Kg3
//Nf4
//33
//Kf3
//Ne6
//34
//Ke3
//Nf4
//35
//c4
//Ne6
//36
//Bb2
//Nf4
//37
//Bc6
//Rd8
//38
//Bxe5
//Ne6
//39
//Ra7
//Nc5
//40
//Bd4
//Ne6
//41
//Bb2
//Nc5
//42
//Be5
//Ne6
//43
//Bg3
//Nd4
//44
//Rxa6
//Nxc2 +
//45
//Kf4
//Nd4
//46
//Ke4
//Nxc6
//47
//Bf2
//Re8 +
//48
//Kf3
//Ne5 +
//49
//Kg2
//Nxc4
//50
//Bd4
//Re2 +
//51
//Kf1
//Rd2
//52
//Bc5
//Ne5
//53
//Ra1
//Nf3
//54
//Ra3
//Ne5
//55
//Ke1
//Rg2
//56
//Bd4
//Nc4
//57
//Ra7
//Nd6
//58
//Rd7
//Ne4
//59
//Kf1
//Rd2
//60
//Kg1
//Rd1 +
//61
//Kg2
//Rd2 +
//62
//Kf3
//Ng5 +
//63
//Kg3
//Ne4 +
//64
//Kh4
//Rh2 +
//65
//Kg4
//Rg2 +
//66
//Kf3
//Rh2
//67
//Kxe4
//Re2 +
//68
//Kd5
//Re8
//69
//Rc7
//Rd8 +
//70
//Ke5
//Re8 +
//71
//Kd6
//Rd8 +
//72
//Ke7
//Rxd4
//73
//Rb7
//Rf4
//74
//Ra7
//Rf2
//75
//Rc7
//Kh7
//76
//Kf8
//Kh8
//77
//Ra7
//Kh7
//78
//Rd7
//Kh8
//79
//Rb7
//Kh7
//80
//Rc7
//Kh8
//81
//Rd7
//Kh7
//82
//Rc7
//Kh8
//83
//Rd7