#pragma once

#include "Debug_Defines.h"

class Chess_Board;

#define DEBUG_PRINT_AI(fmt, ...) \
do { \
	if (DEBUG_ENABLED_AI) \
	{ \
		fprintf(stderr, "AI: "); \
		DEBUG_PRINT(fmt, __VA_ARGS__); \
	} \
} while (0)

#define DEBUG_PRINT_AI_INTENSIVE(fmt, ...) \
do { \
	if (DEBUG_ENABLED_AI_INTENSIVE) \
	{ \
		fprintf(stderr, "AI: "); \
		DEBUG_PRINT(fmt, __VA_ARGS__); \
	} \
} while (0)

#define DEBUG_PRINT_CHESS(fmt, ...) \
do { \
	if (DEBUG_ENABLED_CHESS) \
	{ \
		fprintf(stderr, "CHESS: "); \
		DEBUG_PRINT(fmt, __VA_ARGS__); \
	} \
} while (0)

#define DEBUG_PRINT(fmt, ...)	\
do { \
	fprintf(stderr, fmt, __VA_ARGS__); \
	fprintf(stderr, "\n"); \
} while (0)

class Debug_Readout
{
public:
	static void WriteBoard(const Chess_Board* aChessBoard);
};

